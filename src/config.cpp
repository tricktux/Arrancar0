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
#include <fstream>
#include <map>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <glog/logging.h>

#include "../inc/config.hpp"

void config::parse_arguments(int num_options, char* options[])
{
	std::vector<std::string> _options;
	_options.reserve(8); 

	for (int k=0; k<num_options; k++)
	{
		LOG(INFO) << "cmd line arg#" << k << " = " << options[k] << "\n";
		_options.push_back(options[k]);
	}

	// Search for config option -c <file location>
	bool found = false;
	std::string::size_type sz = config_cmd_option.size();
	for (auto& opt : _options)
	{
		LOG(INFO) << "opt.compare = "
			<< opt.compare(0, config_cmd_option.size(), config_cmd_option)
			<< " \n";

		if ((!opt.empty()) && (opt.size() > sz) &&
				(opt.compare(0, sz, config_cmd_option) == 0))
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
	{
		std::ifstream ifs(config_file_location);

		if (ifs.is_open() == false)
		{
			LOG(ERROR) << "Failed to open file: "
				<< config_file_location
				<< '\n';
			return -1;
		}
	} // Close ifs

	config_file.Parse(config_file_location.c_str());

	if (config_file.HasParseError())
	{
		LOG(ERROR)	<< "Error ("
					<< static_cast<unsigned>(config_file.GetErrorOffset())
					<< "): "
					<< rapidjson::GetParseError_En(config_file.GetParseError())
					<< '\n';
		return -2;
	}
	return 1;
}
