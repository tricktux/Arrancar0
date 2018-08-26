#include <glog/logging.h>
#include <gtest/gtest.h>
#include <sc2utils/sc2_manage_process.h>

#include "config.hpp"
#include "coordinator.hpp"

static int gargc;
static const char **gargv;

int argc = 3;
const char *cmd_opts[] = {"./test_coordinator", "-e",
                          "../../../StarCraftII/Versions/Base60321/SC2_x64"};

TEST(Coordinator, LoadsElgPathProperly) {
  Config &cfg = Config::GetConfig();

  int ret = cfg.ParseConfigFile(argc, cmd_opts);

  ASSERT_EQ(ret, 1);

  Coordinator &sc2_coordinator = Coordinator::GetCoordinator();
  sc2_coordinator.LoadMyConfiguration(argc, cmd_opts);

  sc2_coordinator.SetMyRenderer();

  sc2_coordinator.AddMyCommandLineOpts();

  sc2_coordinator.SetMyParticipants();

  ASSERT_EQ(sc2_coordinator.LaunchGame(), true);

  while (sc2_coordinator.Update()) {
    if (sc2::PollKeyPress()) {
      break;
    }
  }
}

TEST(Coordinator, DISABLED_FullRun) {
  Config &cfg = Config::GetConfig();

  int ret = cfg.ParseConfigFile(argc, cmd_opts);

  ASSERT_EQ(ret, 1);

  Coordinator &sc2_coordinator = Coordinator::GetCoordinator();
  sc2_coordinator.LoadMyConfiguration(argc, cmd_opts);

  sc2_coordinator.SetMyRenderer();

  sc2_coordinator.AddMyCommandLineOpts();

  sc2_coordinator.SetMyParticipants();

  ASSERT_EQ(sc2_coordinator.LaunchGame(), true);

  while (sc2_coordinator.Update()) {
    if (sc2::PollKeyPress()) {
      break;
    }
  }
}

int main(int argc, const char *argv[]) {
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
