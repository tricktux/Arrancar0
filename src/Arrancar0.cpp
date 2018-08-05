// File:           Arrancar0.cpp
// Description:    Main entry point for the SC2Bot
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Created:        Fri Jun 01 2018 03:58
// Last Modified:  Fri Jun 01 2018 03:58

#include <sc2api/sc2_api.h>

#include <glog/logging.h>

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <map>

#include <rapidjson/document.h>

#include "config.hpp"
#include "coordinator.hpp"


// TODO-[RM]-(Sun Jun 03 2018 17:07):  Move this to its own class
// - Also make it singleton
class Bot : public sc2::Agent {
public:
    virtual void OnGameStart() final { LOG(INFO) << "Hello Word!!\n"; }

    virtual void OnStep() final
	{
		LOG(INFO) << "Game Loop = " << Observation()->GetGameLoop();
		LOG(INFO) << "Minerals = " << Observation()->GetMinerals();
		LOG(INFO) << "Vespene Gas = " << Observation()->GetVespene();
    }
};

int main(int argc, const char* argv[]) {
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);

	config &cfg = config::get_config();

	cfg.parse_config_file(argc, argv);

	coordinator &sc2_coordinator = coordinator::get_coordinator();
	sc2_coordinator.load_configurations(argc, argv);

    Bot bot;
    sc2_coordinator.set_participants();

    sc2_coordinator.LaunchStarcraft();
    sc2_coordinator.StartGame(sc2::kMapBelShirVestigeLE);

    while (sc2_coordinator.Update()) {}

    return 0;
}
