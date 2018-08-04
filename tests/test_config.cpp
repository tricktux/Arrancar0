#include <gtest/gtest.h>
#include <glog/logging.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "config.hpp"

TEST(Config, DefaultConfigFile) {
	int argc = 1;
	const char *cmd_opts[] = 
	{
		"DefaultConfigFile",
	};

	config &cfg = config::get_config();

	int ret = cfg.parse_config_file(1, cmd_opts);

	EXPECT_EQ(1,ret);
}

int main(int argc, char *argv[])
{
	// Logging flags
	FLAGS_logtostderr = 1;
	FLAGS_colorlogtostderr = 1;

	::google::InitGoogleLogging(argv[0]);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}