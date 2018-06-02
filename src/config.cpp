// File:           config.cpp
// Description:    Implementation of the config class
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Fri Jun 01 2018 04:31
// Last Modified:  Fri Jun 01 2018 04:31

#include <string>
#include <string_view>
#include <vector>
#include <map>

#include <rapidjson/document.h>
#include <glog/logging.h>
#include "../inc/config.hpp"

void config::parse_arguments(int num_options, char* options[])
{
	std::vector<std::string> _options;
	_options.reserve(8); 

	for (int k=0; k<num_options; k++)
	{
		LOG(INFO) << "cmd line arg#" << k << " = " << options[k] << "\n";
	}

	// Search for config option -c <file location>
	bool found = false;
	for (auto&& opt : _options)
	{
		if ((!opt.empty()) && (opt.size() > 3) && (opt.compare(config_cmd_option) == 0))
		{

			LOG(INFO) << "Found config option: " << opt << "\n";
			config_file_location = opt.substr(3);
			found = true;
			break;
		}
	}

	if (found == false)
		LOG(WARNING) << "configuration option (-c) not provided, using default\n";
}

int config::parse_config_file(void)
{
	config_file.Parse(config_file_location.c_str());

	if (config_file.HasParseError())
	{
		LOG(ERROR)	<< "Error ("
					<< static_cast<unsigned>(config_file.GetErrorOffset())
					<< "): Need to figure out how to get error"
					<< '\n';
		return -1;
	}
	return 1;
}
