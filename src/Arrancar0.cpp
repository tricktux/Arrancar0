// File:           Arrancar0.cpp
// Description:    Main entry point for the SC2Bot
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Created:        Fri Jun 01 2018 03:58
// Last Modified:  Fri Jun 01 2018 03:58

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <map>

#include <rapidjson/document.h>
#include <sc2api/sc2_api.h>
#include <glog/logging.h>

#include "config.hpp"
#include "coordinator.hpp"

int main(int argc, const char* argv[]) {
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);

	Config &cfg = Config::GetConfig();

	cfg.ParseConfigFile(argc, argv);

	Coordinator &sc2_coordinator = Coordinator::GetCoordinator();
	sc2_coordinator.LoadMyConfiguration(argc, argv);

    // Bot bot;
    sc2_coordinator.SetMyParticipants();

    sc2_coordinator.LaunchStarcraft();
    sc2_coordinator.StartGame(sc2::kMapBelShirVestigeLE);

    while (sc2_coordinator.Update()) {}

    return 0;
}
