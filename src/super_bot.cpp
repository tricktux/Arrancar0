#include "super_bot.hpp"

#include <sc2api/sc2_api.h>
#include <sc2renderer/sc2_renderer.h>

#include <algorithm>
#include <limits>
#include <string>

#include "config.hpp"
#include "coordinator.hpp"

const char *CustomRenderer::CONFIG_INT_MEMBERS[] = {"map_x", "map_y",
                                                    "mini_map_x", "mini_map_y"};
const int CustomRenderer::CONFIG_INT_MEMBERS_NUM =
    sizeof(CustomRenderer::CONFIG_INT_MEMBERS) / sizeof(char *);

const char *SuperBot::CONFIG_INT_MEMBERS[] = {"max_num_workers", "max_food_cap",
                                              "food_gap"};
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
    SendWorkerToMine(unit);
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

// TODO-[RM]-(Sat Oct 06 2018 17:42):  We are not actually building structures.
// Not sure why
void SuperBot::BuildStructure(const sc2::Unit *unit_to_build,
                              sc2::ABILITY_ID ability_type_for_structure,
                              const sc2::Point2D &location) {
  if (IsWorker(unit_to_build) == false) {
    LOG(ERROR)
        << "[SuperBot::BuildStructure]: Unit provided is not really a worker";
    return;
  }

	LOG(INFO) << "[SuperBot::BuildStructure]: Building "
		<< "at location (" << location.x << "," << location.y << ")";

  Actions()->UnitCommand(unit_to_build, ability_type_for_structure, location);
}

bool SuperBot::TryBuildSupplyDepot() {
  const sc2::ObservationInterface *observation = Observation();
  int supp_in_constr;
  auto food_used = observation->GetFoodUsed();
  auto food_cap = observation->GetFoodCap();

  if (food_cap >= IntOpts[MAX_FOOD_CAP])
    return false;

	if ((food_used > (food_cap - IntOpts[FOOD_GAP])) && (food_used < food_cap))
		return false;

	LOG(INFO) << "[SuperBot::TryBuildSupplyDepot]: Current " << food_used << "/"
		<< food_cap << " supply.";

	supp_in_constr =
		GetNumOfBuildingUnderConstr(sc2::ABILITY_ID::BUILD_SUPPLYDEPOT);
	LOG(INFO) << "[SuperBot::TryBuildSupplyDepot]: Currently there are "
		<< supp_in_constr << " supply depots in construction";
	food_cap += supp_in_constr * 8;

	const sc2::Unit *worker = GetUnit(sc2::UNIT_TYPEID::TERRAN_SCV);
	if (worker == nullptr) {
		LOG(ERROR) << "[SuperBot::TryBuildSupplyDepot]: Failed to get unit_type";
		return false;
	}

	BuildStructure(worker, sc2::ABILITY_ID::BUILD_SUPPLYDEPOT,
								 GetRandom2dPointNear(worker->pos));

  // Find an SCV
  // const sc2::Unit *worker = GetUnit(sc2::UNIT_TYPEID::TERRAN_SCV);
  // if (worker == nullptr) {
    // LOG(ERROR) << "[SuperBot::TryBuildSupplyDepot]: Failed to get unit_type";
    // return false;
  // }

  // // Are we already building a supply depot?
  // if (GetNumOfBuildingUnderConstr(sc2::ABILITY_ID::BUILD_SUPPLYDEPOT) > 0)
    // return false;

  // // Order scv to build a supply depot at random location
  // LOG(INFO) << "[SuperBot::TryBuildSupplyDepot]: Building more supply depots";
  // BuildStructure(worker, sc2::ABILITY_ID::BUILD_SUPPLYDEPOT,
                 // GetRandom2dPointNear(worker->pos));
  return true;
}

/// @brief Find a unit of type @p unit_type nearest to @p start
/// @param start Starting point to look for unit
/// @param unit_type Type of unit to look for
/// @return Pointer to unit of this type if provided
const sc2::Unit *SuperBot::FindNearestUnit(const sc2::Point2D &start,
                                           sc2::UNIT_TYPEID unit_type) {
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

/// @brief Returns a pointer to a unit of the specified type
/// @param unit_type Specific type of unit requested
/// @return Pointer to request unit if it exists. nullptr otherwise
const sc2::Unit *SuperBot::GetUnit(sc2::UNIT_TYPEID unit_type) {
	sc2::Units units = Observation()->GetUnits();
  const sc2::Unit *target = nullptr;
  for (const auto &u : units) {
    if (u->unit_type == unit_type) {
      target = u;
    }
  }
  return target;
}

/// @brief Provided a @p worker sends it to the nearst mineral field
/// @param worker Pointer to worker that needs to be sent to mine
void SuperBot::SendWorkerToMine(const sc2::Unit *worker) {
  if (IsWorker(worker) == false) {
    LOG(ERROR)
        << "[SuperBot::SendWorkerToMine]: Unit provided is not really a worker";
    return;
  }

  LOG(INFO) << "[SuperBot::SendWorkerToMine]: Found worker hanging out";
  const sc2::Unit *mineral_target =
      FindNearestUnit(worker->pos, sc2::UNIT_TYPEID::NEUTRAL_MINERALFIELD);
  if (mineral_target == nullptr) {
    LOG(ERROR) << "[SuperBot::SendWorkerToMine]: Failed to find close by "
                  "mineral field.";
    return;
  }

  LOG(INFO) << "[SuperBot::SendWorkerToMine]: Sending worker to mine";
  Actions()->UnitCommand(worker, sc2::ABILITY_ID::SMART, mineral_target);
}

/// @brief Determine if unit @p worker is a worker of any race
/// @param worker Pointer to unit
/// @return True if its a worker, false otherwise
bool SuperBot::IsWorker(const sc2::Unit *worker) {
  if (worker == nullptr) {
    return false;
  }

  if ((worker->unit_type.ToType() == sc2::UNIT_TYPEID::PROTOSS_PROBE) ||
      (worker->unit_type.ToType() == sc2::UNIT_TYPEID::ZERG_DRONE) ||
      (worker->unit_type.ToType() == sc2::UNIT_TYPEID::TERRAN_SCV)) {
    return true;
  }

  return false;
}

sc2::Point2D SuperBot::GetRandom2dPointNear(const sc2::Point2D &location) {
  float rx = sc2::GetRandomScalar();
  float ry = sc2::GetRandomScalar();

  return sc2::Point2D(location.x + rx * 15.0f, location.y + ry * 15.0f);
}

/// @brief Returns number of buildings of @p ability_type_for_structure type
/// that are under construction right now
int SuperBot::GetNumOfBuildingUnderConstr(
    sc2::ABILITY_ID ability_type_for_structure) {
  sc2::Units units = Observation()->GetUnits(sc2::Unit::Alliance::Self);
  int num = 0;
  for (const auto &unit : units) {
    for (const auto &order : unit->orders) {
      if (order.ability_id == ability_type_for_structure) {
        num++;
      }
    }
  }

  return num;
}

const sc2::Unit *SuperBot::GetFreeWorker(sc2::UNIT_TYPEID unit_type) {
	sc2::Units units = Observation()->GetUnits();
	const sc2::Unit *target = nullptr;
	for (const auto &u : units) {
		if (u->unit_type == unit_type) {
			LOG(INFO) << "[SuperBot::GetFreeWorker]: Unit has "
				<< u->orders.size() << " orders";
			for (const auto &order : u->orders) {
				if (order.ability_id == 0) {
					target = u;
					// break;
				}
			}
		}
	}
	return target;
}
