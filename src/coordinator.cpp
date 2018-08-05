// File:           coordinator.cpp
// Description:     Code Description
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Thu Aug 02 2018 20:15
// Last Modified:  Thu Aug 02 2018 20:15

#include "coordinator.hpp"

#include <glog/logging.h>

#include <map>
#include <string>
#include <vector>

#include "config.hpp"
#include "super_bot.hpp"


const char *Coordinator::CONFIG_STRING_MEMBERS[] = {
	"map",
	"your_race",
	"opponent_race"
};
const int Coordinator::CONFIG_STRING_MEMBERS_NUM =
	sizeof(Coordinator::CONFIG_STRING_MEMBERS)/sizeof(char *);


const std::map<std::string, sc2::Race> Coordinator::CONFIG_RACE_MAP = {
	{"Terran", sc2::Race::Terran},
	{"Zerg", sc2::Race::Zerg},
	{"Protoss", sc2::Race::Protoss},
	{"Random", sc2::Race::Random}
};

void Coordinator::LoadMyConfiguration(int argc, const char** argv) {
	std::string buff;
	// Overwrite settings if they were passed through the cli
	char **argv_buff = const_cast<char **>(argv); // Remove const from argv
	const Config &cfg = Config::GetConfig();

	for (int k=0; k < StringOptions::MAX; k++) {
		cfg.GetValue(CONFIG_OBJECT, CONFIG_STRING_MEMBERS[k], StrOpts[k]);
		buff = CONFIG_STRING_MEMBERS[k] + std::string(" ") + StrOpts[k];
		LOG(INFO) << "[coordinator::LoadMyConfiguration]: Got: '"
			<< buff << "'";
	}

	if (LoadSettings(argc, argv_buff) == false) {
		LOG(WARNING) << "[Coordinator::LoadMyConfiguration]: Failed to LoadSettings";
		LOG(INFO) << "[Coordinator::LoadMyConfiguration]: argc = " << argc;

		for (int k=0; k < argc; k++) {
			LOG(INFO) << "[Coordinator::LoadMyConfiguration]: argv[" << k << "] = "
				<< argv[k];
		}
	}
}

void Coordinator::SetMyParticipants(void) {
	int k = StringOptions::YOUR_RACE;
	sc2::Race buff = sc2::Race::Terran;
	std::vector<sc2::PlayerSetup> player_setup;
	SuperBot& bot = SuperBot::GetSuperBot();

	// Set a default option for the opponent race.
	if (StrOpts[k].empty() == true)
		StrOpts[k] = "Terran";
	auto search = CONFIG_RACE_MAP.find(StrOpts[k]);

	if (search != CONFIG_RACE_MAP.end()) {
		LOG(INFO) << "[Coordinator::SetMyParticipants]: Detected race: '"
			<< StrOpts[k] << "'";
		buff = search->second;
	} else {
		LOG(WARNING) << "[Coordinator::SetMyParticipants]: Your race wasnt detected";
	}

	player_setup.emplace_back(sc2::CreateParticipant(buff, &bot));

	// Decode opponent race
	k = StringOptions::OPPONENT_RACE;
	buff = sc2::Race::Random;
	if (StrOpts[k].empty() == true)
		StrOpts[k] = "Random";
	search = CONFIG_RACE_MAP.find(StrOpts[k]);

	if (search != CONFIG_RACE_MAP.end()) {
		LOG(INFO) << "[Coordinator::SetMyParticipants]: Detected race: '"
			<< StrOpts[k] << "'";
		buff = search->second;
	} else {
		LOG(WARNING) << "[Coordinator::SetMyParticipants]: Your race wasnt detected";
	}
	player_setup.emplace_back(sc2::CreateComputer(buff));

	// TODO-[RM]-(Sat Aug 04 2018 23:58): Pass this on
	// sc2::CreateParticipant(sc2::Race::Terran, &bot),
    SetParticipants(player_setup);
}

bool Coordinator::LaunchGame() {
	// Get map
	if (StrOpts[StringOptions::MAP].empty() == true) {
		LOG(ERROR) << "[Coordinator::LaunchGame]: Map options was not provided";
		return false;
	}

	LaunchStarcraft();
	return StartGame(StrOpts[StringOptions::MAP]);
}
