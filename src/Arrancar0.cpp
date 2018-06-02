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

#include "../inc/config.hpp"
#include "../inc/coordinator.hpp"


class Bot : public sc2::Agent {
public:
    virtual void OnGameStart() final { LOG(INFO) << "Hello Word!!\n"; }

    virtual void OnStep() final
	{
		LOG(INFO) << "Game Loop = " << Observation()->GetGameLoop() << "\n";
		LOG(INFO) << "Minerals = " << Observation()->GetMinerals() << "\n";
		LOG(INFO) << "Vespene Gas = " << Observation()->GetVespene() << "\n";
    }
};

int main(int argc, char* argv[]) {
	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);

	sc2::Coordinator &sc2_coordinator = coordinator::get_coordinator();

	config &cfg = config::get_config();

	cfg.parse_arguments(argc, argv);
	if (cfg.parse_config_file() < 1)
		sc2_coordinator.LoadSettings(argc, argv);

    Bot bot;
    sc2_coordinator.SetParticipants({
		sc2::CreateParticipant(sc2::Race::Terran, &bot),
        sc2::CreateComputer(sc2::Race::Zerg)
    });

    sc2_coordinator.LaunchStarcraft();
    sc2_coordinator.StartGame();

    while (sc2_coordinator.Update()) {}

    return 0;
}
