// File:           coordinator.hpp
// Description:    Wrapper around sc2::Coordinator
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Fri Jun 01 2018 19:25
// Last Modified:  Fri Jun 01 2018 19:25

#ifndef COORDINATOR_HPP
#define COORDINATOR_HPP

#include <string_view>
#include <map>
#include <sc2api/sc2_api.h>

class coordinator : public sc2::Coordinator
{
	enum StringOptions
	{
		MAP = 0,
		YOUR_RACE,
		OPPONENT_RACE,
		MAX
	};
	char const *CONFIG_OBJECT = "coordinator";

	static const char *CONFIG_STRING_MEMBERS[];
	static const int CONFIG_STRING_MEMBERS_NUM;

	std::string str_options[StringOptions::MAX];
	static const std::map<std::string, sc2::Race> CONFIG_RACE_MAP;

	coordinator() : sc2::Coordinator() 
	{
		for (int k=0; k<StringOptions::MAX; k++)
			str_options[k] = std::string();
	}
public:

	static coordinator& get_coordinator(void)
	{
		static coordinator rc;
		return rc;
	}

	coordinator(coordinator const&) = delete;
	void operator= (coordinator const&) = delete;

	void load_configurations(int argc, const char** argv);
	void set_participants();
};

#endif
