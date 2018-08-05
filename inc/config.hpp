// File:           config.hpp
// Description:    Used to load configuration from json file
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Fri Jun 01 2018 03:55
// Last Modified:  Fri Jun 01 2018 03:55

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <string_view>
#include <rapidjson/document.h>

class Config
{
	const std::string_view CMD_OPTION = "-c";
	const std::string_view CMD_DEFAULT = "config.json";

	rapidjson::Document File;
	std::string FileLocation;

	// Search for CMD_OPTION inside arguments. Called fromm ParseConfigFile
	void ParseArguments(int num_options, const char *options[]);

	// If CMD_OPTION found in ParseArguments attempt to load json file using rapidjson
	int LoadConfigFile(void);

	// Helping function used by get_value functions
	int IsThereObjectWithMember(const char *object, const char *member) const;
public:
	Config() : FileLocation(CMD_DEFAULT) {}

	static Config& GetConfig(void)
	{
		static Config rc;
		return rc;
	}

	Config(Config const&) = delete;
	void operator=(Config const&) = delete;

	int ParseConfigFile(int num_options, const char **arguments);

	int GetValue(const char *object, const char *member, std::string &value) const;
	int GetValue(const char *object, const char *member, double &value) const;
	int GetValue(const char *object, const char *member, int &value) const;
	int GetValue(const char *object, const char *member, bool &value) const;
};

#endif
