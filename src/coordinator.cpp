// File:           coordinator.cpp
// Description:     Code Description
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Thu Aug 02 2018 20:15
// Last Modified:  Thu Aug 02 2018 20:15

#include <glog/logging.h>

#include "config.hpp"
#include "coordinator.hpp"

const char *coordinator::CONFIG_STRING_MEMBERS[] =
{
	"-m",
	"-e",
	"your_race",
	"opponent_race"
};
const int coordinator::CONFIG_STRING_MEMBERS_NUM =
	sizeof(coordinator::CONFIG_STRING_MEMBERS)/sizeof(char *);


const std::map<std::string, sc2::Race> coordinator::CONFIG_RACE_MAP =
{
	{"Terran", sc2::Race::Terran},
	{"Zerg", sc2::Race::Zerg},
	{"Protoss", sc2::Race::Protoss},
	{"Random", sc2::Race::Random}
};

void coordinator::load_configurations(int argc, const char** argv)
{
	std::string buff;
	// Overwrite settings if they were passed through the cli
	char **argv_buff = const_cast<char **>(argv); // Remove const from argv

	// Sun Aug 05 2018 10:37: No use for this right now. It has no effect. 
	// const config &cfg = config::get_config();
	// for (int k=StringOptions::MAP; k<=StringOptions::EXECUTABLE_PATH; k++)
	// {
		// cfg.get_value(CONFIG_OBJECT, CONFIG_STRING_MEMBERS[k], str_options[k]);
		// buff = CONFIG_STRING_MEMBERS[k] + std::string(" ") + str_options[k];
		// LOG(INFO) << "[coordinator::load_configurations]: Adding Command Line: '"
			// << buff << "'";
		// AddCommandLine(CONFIG_STRING_MEMBERS[k]);
		// AddCommandLine(str_options[k]);
	// }

	if (LoadSettings(argc, argv_buff) == false)
	{
		LOG(WARNING) << "[coordinator::load_configurations]: Failed to LoadSettings";
		LOG(INFO) << "[coordinator::load_configurations]: argc = " << argc;
		for (int k=0; k < argc; k++)
		{
			LOG(INFO) << "[coordinator::load_configurations]: argv[" << k << "] = "
				<< argv[k];
		}
	}
}

void coordinator::set_participants(void)
{
	sc2::Race buff;
	std::vector<sc2::PlayerSetup> player_setup;

	// Set a default option for the opponent race.
	if (str_options[StringOptions::OPPONENT_RACE].empty() == true)
		str_options[StringOptions::OPPONENT_RACE] = "Random";
	if (str_options[StringOptions::YOUR_RACE].empty() == true)
		str_options[StringOptions::YOUR_RACE] = "Terran";

	// Decode opponent race
	for (int k=StringOptions::YOUR_RACE; k<=StringOptions::OPPONENT_RACE; k++)
	{
		auto search = CONFIG_RACE_MAP.find(str_options[k]);
		if (search != CONFIG_RACE_MAP.end())
		{
			LOG(INFO) << "[coordinator::set_participants]: Detected race: '"
				<< str_options[k] << "'";
			buff = search->second;
		}
		else
		{
			LOG(WARNING) << "[coordinator::set_participants]: Failed to decode race: '"
				<< str_options[k] << "'";
			buff = sc2::Race::Random;
		}

		player_setup.emplace_back(sc2::CreateComputer(buff));
	}

	// TODO-[RM]-(Sat Aug 04 2018 23:58): Pass this on
	// sc2::CreateParticipant(sc2::Race::Terran, &bot),
    SetParticipants(player_setup);
}
