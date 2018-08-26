#include "super_bot.hpp"

#include <sc2api/sc2_api.h>
#include <sc2renderer/sc2_renderer.h>

#include <algorithm>
#include <limits>

#include "config.hpp"
#include "coordinator.hpp"

const char *CustomRenderer::CONFIG_INT_MEMBERS[] = {"map_x", "map_y",
                                                    "mini_map_x", "mini_map_y"};
const int CustomRenderer::CONFIG_INT_MEMBERS_NUM =
    sizeof(CustomRenderer::CONFIG_INT_MEMBERS) / sizeof(char *);

const char *SuperBot::CONFIG_INT_MEMBERS[] = {"max_num_workers"};
const int SuperBot::CONFIG_INT_MEMBERS_NUM =
    sizeof(SuperBot::CONFIG_INT_MEMBERS) / sizeof(char *);

void CustomRenderer::Render(const SC2APIProtocol::Observation *observation) {
  if (On == false)
    return;

  if (observation == nullptr) {
    LOG(ERROR) << "[CustomRenderer::Render]: Bad function input";
    return;
  }

  const SC2APIProtocol::ObservationRender &render = observation->render_data();

  const SC2APIProtocol::ImageData &minimap = render.minimap();
  sc2::renderer::ImageRGB(
      &minimap.data().data()[0], minimap.size().x(), minimap.size().y(), 0,
      std::max(IntOpts[MINI_MAP_Y], IntOpts[MAP_Y]) - IntOpts[MINI_MAP_Y]);

  const SC2APIProtocol::ImageData &map = render.map();
  sc2::renderer::ImageRGB(&map.data().data()[0], map.size().x(), map.size().y(),
                          IntOpts[MINI_MAP_X], 0);

  sc2::renderer::Render();
}

bool CustomRenderer::LoadOpts(void) {
  const Config &cfg = Config::GetConfig();

  cfg.GetValue(CONFIG_OBJECT, CONFIG_BOOL_MEMBER, On);

  if (On == false)
    return false;

  for (int k = 0; k < IntOptions::MAX; k++) {
    cfg.GetValue(CONFIG_OBJECT, CONFIG_INT_MEMBERS[k], IntOpts[k]);
    LOG(INFO) << "[CustomRenderer::LoadOpts]: Got: '" << CONFIG_INT_MEMBERS[k]
              << "' = " << IntOpts[k];
  }

  return true;
}

void CustomRenderer::Init(void) {
  if (On == false)
    return;

  sc2::renderer::Initialize("Rendered", 50, 50, IntOpts[MAP_X] + IntOpts[MAP_Y],
                            std::max(IntOpts[MINI_MAP_Y], IntOpts[MAP_Y]));
}

void SuperBot::OnUnitIdle(const sc2::Unit *unit) {
  if (unit == nullptr) {
    LOG(ERROR) << "[SuperBot::OnUnitIdle]: Bad function input";
    return;
  }

  switch (unit->unit_type.ToType()) {
  case sc2::UNIT_TYPEID::ZERG_HATCHERY:
  case sc2::UNIT_TYPEID::PROTOSS_NEXUS:
  case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER: {
    BuildMoreWorkers(unit);
    break;
  }

  case sc2::UNIT_TYPEID::PROTOSS_PROBE:
  case sc2::UNIT_TYPEID::ZERG_DRONE:
  case sc2::UNIT_TYPEID::TERRAN_SCV: {
		SendIdleWorkerToMine(unit);
    break;
  }

  default: { break; }
  }
}

void SuperBot::OnGameStart() {
  // TODO-[RM]-(Sun Aug 26 2018 14:19): Race should be under "bot", not
  // "coordinator"
  MyRace = Coordinator::GetCoordinator().GetBotRace();
  CustRender.Init();
}

void SuperBot::OnStep() {
  CustRender.Render(Observation()->GetRawObservation());
  TryBuildSupplyDepot();
  // TODO-Sun Aug 26 2018 16:27:  Get vespine gas.
}

void SuperBot::BuildMoreWorkers(const sc2::Unit *unit) {
  if ((unit == nullptr) ||
      (unit->unit_type.ToType() != sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER)) {
    LOG(ERROR) << "[SuperBot::OnUnitIdle]: Bad function input";
    return;
  }

  if (Observation()->GetFoodWorkers() < IntOpts[MAX_NUM_WORKERS]) {
    Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_SCV);
    LOG(INFO) << "[SuperBot::BuildMoreWorkers]: Building more scvs";
  }
}

void SuperBot::LoadConfig(void) {
  const Config &cfg = Config::GetConfig();

  for (int k = 0; k < IntOptions::MAX; k++) {
    cfg.GetValue(CONFIG_OBJECT, CONFIG_INT_MEMBERS[k], IntOpts[k]);
    LOG(INFO) << "[SuperBot::LoadConfig]: Got: '" << CONFIG_INT_MEMBERS[k]
              << "' = " << IntOpts[k];
  }
}

bool SuperBot::TryBuildStructure(sc2::ABILITY_ID ability_type_for_structure,
                                 sc2::UNIT_TYPEID unit_type) {
  const sc2::ObservationInterface *observation = Observation();

  // If a unit already is building a supply structure of this type, do nothing.
  // Also get an scv to build the structure.
  const sc2::Unit *unit_to_build = nullptr;
  sc2::Units units = observation->GetUnits(sc2::Unit::Alliance::Self);
  for (const auto &unit : units) {
    for (const auto &order : unit->orders) {
      if (order.ability_id == ability_type_for_structure) {
        return false;
      }
    }

    if (unit->unit_type == unit_type) {
      unit_to_build = unit;
    }
  }

  if (unit_to_build == nullptr) {
    LOG(ERROR) << "[SuperBot::TryBuildStructure]: Bad pointer unit_to_build";
    return false;
  }

  float rx = sc2::GetRandomScalar();
  float ry = sc2::GetRandomScalar();

  LOG(INFO) << "[SuperBot::TryBuildStructure]: Building more supply depots";
  Actions()->UnitCommand(unit_to_build, ability_type_for_structure,
                         sc2::Point2D(unit_to_build->pos.x + rx * 15.0f,
                                      unit_to_build->pos.y + ry * 15.0f));

  return true;
}

bool SuperBot::TryBuildSupplyDepot() {
  const sc2::ObservationInterface *observation = Observation();

  // If we are not supply capped, don't build a supply depot.
  if (observation->GetFoodUsed() <= observation->GetFoodCap() - 10)
    return false;

  LOG(INFO) << "[SuperBot::TryBuildSupplyDepot]: " << observation->GetFoodUsed()
            << "/" << observation->GetFoodCap() << " supply.";
  // Try and build a depot. Find a random SCV and give it the order.
  return TryBuildStructure(sc2::ABILITY_ID::BUILD_SUPPLYDEPOT);
}

const sc2::Unit *SuperBot::FindNearestUnit(const sc2::Point2D &start, sc2::UNIT_TYPEID unit_type) {
  sc2::Units units = Observation()->GetUnits();
  float distance = std::numeric_limits<float>::max();
  const sc2::Unit *target = nullptr;
  for (const auto &u : units) {
    if (u->unit_type == unit_type) {
      float d = sc2::DistanceSquared2D(u->pos, start);
      if (d < distance) {
        distance = d;
        target = u;
      }
    }
  }
  return target;
}

const sc2::Unit* SuperBot::GetUnit(sc2::UNIT_TYPEID unit_type) {
	sc2::Units units = Observation()->GetUnits();
	const sc2::Unit *target = nullptr;
	for (const auto &u : units) {
		if (u->unit_type == unit_type) {
			target = u;
		}
	}
	return target;
}

void SuperBot::SendIdleWorkerToMine(const sc2::Unit *worker) {
	if (worker == nullptr) {
		LOG(ERROR) << "[SuperBot::SendIdleWorkerToMine]: Bad function input";
		return;
	}

	LOG(INFO) << "[SuperBot::OnUnitIdle]: Found worker hanging out";
	const sc2::Unit *mineral_target =
		FindNearestUnit(worker->pos, sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD);
	if (mineral_target == nullptr) {
		LOG(ERROR) << "[SuperBot::OnUnitIdle]: Failed to find close by mineral field.";
		return;;
	}

	LOG(INFO) << "[SuperBot::OnUnitIdle]: Sending worker to mine";
	Actions()->UnitCommand(worker, sc2::ABILITY_ID::SMART, mineral_target);
}
