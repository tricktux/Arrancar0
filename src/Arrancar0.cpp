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

	config &cfg = config::get_config();

	sc2::Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

    Bot bot;
    coordinator.SetParticipants({
		sc2::CreateParticipant(sc2::Race::Terran, &bot),
        sc2::CreateComputer(sc2::Race::Zerg)
    });

    coordinator.LaunchStarcraft();
    coordinator.StartGame();

    while (coordinator.Update()) {}

    return 0;
}
