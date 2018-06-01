// File:           config.hpp
// Description:    Used to load configuration from json file
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Fri Jun 01 2018 03:55
// Last Modified:  Fri Jun 01 2018 03:55

#ifndef CONFIG_HPP
#define CONFIG_HPP

class config 
{
	const std::string_view config_cmd_option = "-c";
	const std::string_view config_default_name = "config.json";

	rapidjson::Document config_file;
	std::string config_file_location;

public:
	config() : config_file_location(config_default_name) {}

	static config& get_config(void)
	{
		static config rc;
		return rc;
	}

	void parse_arguments(int num_options, char *options[]);
	int parse_config_file(void);
};

#endif
