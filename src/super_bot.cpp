
#include "super_bot.hpp"

#include <sc2api/sc2_api.h>
#include <sc2renderer/sc2_renderer.h>

#include <algorithm>

#include "config.hpp"
#include "coordinator.hpp"

const char *CustomRenderer::CONFIG_INT_MEMBERS[] = {
	"map_x",
	"map_y",
	"mini_map_x",
	"mini_map_y"
};
const int CustomRenderer::CONFIG_INT_MEMBERS_NUM =
	sizeof(CustomRenderer::CONFIG_INT_MEMBERS)/sizeof(char *);

const char *SuperBot::CONFIG_INT_MEMBERS[] = {
	"max_num_workers"
};
const int SuperBot::CONFIG_INT_MEMBERS_NUM =
	sizeof(SuperBot::CONFIG_INT_MEMBERS)/sizeof(char *);

void CustomRenderer::Render(const SC2APIProtocol::Observation* observation) {
	if (On == false) return;

	if (observation == nullptr) {
		LOG(ERROR) << "[CustomRenderer::Render]: Bad function input";
		return;
	}

	const SC2APIProtocol::ObservationRender& render =  observation->render_data();

	const SC2APIProtocol::ImageData& minimap = render.minimap();
	sc2::renderer::ImageRGB(&minimap.data().data()[0], minimap.size().x(),
			minimap.size().y(), 0,
			std::max(IntOpts[MINI_MAP_Y], IntOpts[MAP_Y]) - IntOpts[MINI_MAP_Y]);

	const SC2APIProtocol::ImageData& map = render.map();
	sc2::renderer::ImageRGB(&map.data().data()[0], map.size().x(), map.size().y(),
			IntOpts[MINI_MAP_X], 0);

	sc2::renderer::Render();
}

bool CustomRenderer::LoadOpts(void) {
	const Config &cfg = Config::GetConfig();

	cfg.GetValue(CONFIG_OBJECT, CONFIG_BOOL_MEMBER, On);

	if (On == false) return false;

	for (int k=0; k < IntOptions::MAX; k++) {
		cfg.GetValue(CONFIG_OBJECT, CONFIG_INT_MEMBERS[k], IntOpts[k]);
		LOG(INFO) << "[CustomRenderer::LoadOpts]: Got: '"
			<< CONFIG_INT_MEMBERS[k] << "' = " << IntOpts[k];
	}

	return true;
}

void CustomRenderer::Init(void) {
	if (On == false) return;

	sc2::renderer::Initialize("Rendered", 50, 50, IntOpts[MAP_X] + IntOpts[MAP_Y],
			std::max(IntOpts[MINI_MAP_Y], IntOpts[MAP_Y]));
}

void SuperBot::OnUnitIdle(const sc2::Unit* unit) {
	if (unit == nullptr) {
		LOG(ERROR) << "[SuperBot::OnUnitIdle]: Bad function input";
		return;
	}

	switch (unit->unit_type.ToType()) {
		case sc2::UNIT_TYPEID::ZERG_HATCHERY:
		case sc2::UNIT_TYPEID::PROTOSS_NEXUS:
		case sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER:
			BuildMoreWorkers(unit);
			break;

		case sc2::UNIT_TYPEID::PROTOSS_PROBE:
		case sc2::UNIT_TYPEID::ZERG_DRONE:
		case sc2::UNIT_TYPEID::TERRAN_SCV:
			 Actions()->UnitCommand(unit, sc2::ABILITY_ID::HARVEST_GATHER);
			 break;

		default: { break; }
	}
}

void SuperBot::OnGameStart() {
	MyRace = Coordinator::GetCoordinator().GetBotRace();
	CustRender.Init();
}

void SuperBot::OnStep() {
	CustRender.Render(Observation()->GetRawObservation());
}

void SuperBot::BuildMoreWorkers(const sc2::Unit* unit) {
	if ((unit == nullptr) ||
			(unit->unit_type.ToType() != sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER)) {
		LOG(ERROR) << "[SuperBot::OnUnitIdle]: Bad function input";
		return;
	}

	int workers = Observation()->GetFoodWorkers();

	if (workers < IntOpts[MAX_NUM_WORKERS])
		Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_SCV);
}

void SuperBot::LoadConfig(void) {
	const Config &cfg = Config::GetConfig();

	for (int k=0; k < IntOptions::MAX; k++) {
		cfg.GetValue(CONFIG_OBJECT, CONFIG_INT_MEMBERS[k], IntOpts[k]);
		LOG(INFO) << "[SuperBot::LoadConfig]: Got: '"
			<< CONFIG_INT_MEMBERS[k] << "' = " << IntOpts[k];
	}
}
