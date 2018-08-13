// File:           coordinator.hpp
// Description:    Wrapper around sc2::Coordinator
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Fri Jun 01 2018 19:25
// Last Modified:  Fri Jun 01 2018 19:25

#ifndef COORDINATOR_HPP
#define COORDINATOR_HPP

#include <sc2api/sc2_api.h>

#include <map>
#include <string>
#include <vector>

class Coordinator : public sc2::Coordinator {
	enum StringOptions {
		MAP = 0,
		YOUR_RACE,
		OPPONENT_RACE,
		MAX
	};
	char const *CONFIG_OBJECT = "coordinator";
	char const *CONFIG_CLI_MEMBER = "cli_option_";
	static const std::map<std::string, sc2::Race> CONFIG_RACE_MAP;
	static const char *CONFIG_STRING_MEMBERS[];
	static const int CONFIG_STRING_MEMBERS_NUM;

	const int CLI_OPTIONS_MAX = 10;

	std::string StrOpts[StringOptions::MAX];

	Coordinator() : sc2::Coordinator() {
		for (int k=0; k<StringOptions::MAX; k++)
			StrOpts[k] = std::string();
	}

public:

	static Coordinator& GetCoordinator(void) {
		static Coordinator rc;
		return rc;
	}

	Coordinator(Coordinator const&) = delete;
	void operator= (Coordinator const&) = delete;

	void LoadMyConfiguration(int argc, const char** argv);
	void SetMyParticipants();
	void SetMyRenderer();

	bool LaunchGame();
	// TODO-[RM]-(Sun Aug 12 2018 22:16):  
	// - Implement this function
	sc2::Race GetPlayersRace();
};

#endif
