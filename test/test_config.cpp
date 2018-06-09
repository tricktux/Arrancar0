#include <gtest/gtest.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "config.hpp"

TEST(Config, GoodConfigFile) {
	int argc = 5;
	char *argv[] = 
	{
		"-c /home/reinaldo/Documents/ML_SC2/Arrancar0/test/config/config.json",
		"-c /home/reinaldo/Documents/ML_SC2/Arrancar0/test/config/config.json",
		"-c /home/reinaldo/Documents/ML_SC2/Arrancar0/test/config/config.json",
		"-c /home/reinaldo/Documents/ML_SC2/Arrancar0/test/config/config.json",
		"-c /home/reinaldo/Documents/ML_SC2/Arrancar0/test/config/config.json"
	};

	config &cfg = config::get_config();

	cfg.parse_arguments(argc, argv);
	int ret = cfg.parse_config_file();

	EXPECT_EQ(1,ret);
}
