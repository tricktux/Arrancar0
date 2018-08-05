#include <gtest/gtest.h>
#include <glog/logging.h>

#include "config.hpp"
#include "coordinator.hpp"

static int gargc;
static const char **gargv;

TEST(Coordinator, LoadingRaces) {
	int argc = 3;
	const char *cmd_opts[] = 
	{
		"./test_coordinator",
		"-e",
		"../../../StarCraftII/Versions/Base60321/SC2_x64"
		// "/home/reinaldo/Documents/ML_SC2/StarCraftII/Versions/Base60321/SC2_x64"
	};

	config &cfg = config::get_config();

	int ret = cfg.parse_config_file(argc, cmd_opts);

	ASSERT_EQ(ret,1);

	coordinator &sc2_coordinator = coordinator::get_coordinator();
	sc2_coordinator.load_configurations(argc, cmd_opts);

    sc2_coordinator.set_participants();

	sc2_coordinator.LaunchStarcraft();
	sc2_coordinator.StartGame(sc2::kMapBelShirVestigeLE);

	while (sc2_coordinator.Update()) {
	}
}

int main(int argc, const char *argv[])
{
	// Logging flags
	FLAGS_logtostderr = 1;
	FLAGS_colorlogtostderr = 1;

	char **argv_buff = const_cast<char **>(argv); // Remove const from argv
	::google::InitGoogleLogging(argv[0]);
	::testing::InitGoogleTest(&argc, argv_buff);

	gargc = argc;
	gargv = argv;
	return RUN_ALL_TESTS();
}
