#include <string>

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

	Config &cfg = Config::GetConfig();

	int ret = cfg.ParseConfigFile(1, cmd_opts);

	ASSERT_EQ(ret, 1);

	std::string s;

	EXPECT_NE(cfg.GetValue("render", "on", s), 1);
	EXPECT_NE(cfg.GetValue("replay", "on", s), 1);

	EXPECT_EQ(cfg.GetValue("replay", "name", s), 1);
	LOG(INFO) << "s = '" << s << "'";
	EXPECT_EQ(cfg.GetValue("coordinator", "map", s), 1);
	LOG(INFO) << "s = '" << s << "'";
	EXPECT_EQ(cfg.GetValue("coordinator", "your_race", s), 1);
	LOG(INFO) << "s = '" << s << "'";
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
