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
#include <sc2api/sc2_api.h>

class coordinator : public sc2::Coordinator
{
	char const *CONFIG_OBJECT = "extra_cmd_line_arguments";
	/// Thu Aug 02 2018 20:35 left here tryng to code the 
	/// options that the coordinator loads in the config.json
	// char const *MEMBERS[] = {
		// "map",
		// "executable_path"
	// };

	coordinator() : sc2::Coordinator() {}
public:

	static coordinator& get_coordinator(void)
	{
		static coordinator rc;
		return rc;
	}

	coordinator(coordinator const&) = delete;
	void operator= (coordinator const&) = delete;

	void load_configurations(int argc, const char** argv);
};

#endif
