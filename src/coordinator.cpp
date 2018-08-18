// File:           coordinator.cpp
// Description:     Code Description
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Thu Aug 02 2018 20:15
// Last Modified:  Thu Aug 02 2018 20:15

#include "coordinator.hpp"

#include <glog/logging.h>

#include <map>
#include <string>
#include <vector>
#include <unordered_map>

#include "config.hpp"
#include "super_bot.hpp"

const char *Coordinator::CONFIG_STRING_MEMBERS[] = {
    "map",
    "bot_race",
    "opponent_race",
};
const int Coordinator::CONFIG_STRING_MEMBERS_NUM =
    sizeof(Coordinator::CONFIG_STRING_MEMBERS) / sizeof(char *);

const char *Coordinator::CONFIG_STRING_DEFAULT_MEMBERS[] = {
    "/home/reinaldo/Documents/ML_SC2/StarCraftII/Maps/(2)16-BitLE.SC2Map",
    "Terran", "Random"};
const int Coordinator::CONFIG_STRING_DEFAULT_MEMBERS_NUM =
    sizeof(Coordinator::CONFIG_STRING_DEFAULT_MEMBERS) / sizeof(char *);

const std::unordered_map<std::string, sc2::Race> Coordinator::CONFIG_RACE_MAP = {
    {"Terran", sc2::Race::Terran},
    {"Zerg", sc2::Race::Zerg},
    {"Protoss", sc2::Race::Protoss},
    {"Random", sc2::Race::Random}};

void Coordinator::LoadMyConfiguration(int argc, const char **argv) {
  std::string buff, buff1 = std::string();
  // Overwrite settings if they were passed through the cli
  char **argv_buff = const_cast<char **>(argv); // Remove const from argv
  const Config &cfg = Config::GetConfig();

  for (int k = 0; k < StringOptions::MAX; k++) {
    cfg.GetValue(CONFIG_OBJECT, CONFIG_STRING_MEMBERS[k], StrOpts[k]);
    LOG(INFO) << "[Coordinator::LoadMyConfiguration]: Got: '"
              << CONFIG_STRING_MEMBERS[k] << ": " << StrOpts[k] << "'";
  }

  if (LoadSettings(argc, argv_buff) == false) {
    LOG(WARNING)
        << "[Coordinator::LoadMyConfiguration]: Failed to LoadSettings";
    LOG(INFO) << "[Coordinator::LoadMyConfiguration]: argc = " << argc;

    for (int k = 0; k < argc; k++) {
      LOG(INFO) << "[Coordinator::LoadMyConfiguration]: argv[" << k
                << "] = " << argv[k];
    }
  }

  for (int k = 0; k < CLI_OPTIONS_MAX; k++, buff1 = std::string()) {
    buff = std::string(CONFIG_CLI_MEMBER) + std::to_string(k);
    cfg.GetValue(CONFIG_OBJECT, buff.c_str(), buff1);
    if (buff1.empty() == true)
      break;

    AddCommandLine(buff1);
    LOG(INFO) << "[Coordinator::LoadMyConfiguration]: AddCommandLine(" << buff1
              << ")";
  }
}

void Coordinator::SetMyParticipants(void) {
  std::vector<sc2::PlayerSetup> player_setup;
  SuperBot &bot = SuperBot::GetSuperBot();

  // Set a default option for the opponent race.
  auto search = CONFIG_RACE_MAP.find(StrOpts[StringOptions::BOT_RACE]);
  if (search != CONFIG_RACE_MAP.end()) {
    LOG(INFO) << "[Coordinator::SetMyParticipants]: Provided race: '"
              << StrOpts[StringOptions::BOT_RACE] << "'";
    BotRace = search->second;
  } else {
    LOG(WARNING) << "[Coordinator::SetMyParticipants]: Your race wasnt valid. "
                    "Using default.";
  }
  player_setup.emplace_back(sc2::CreateParticipant(BotRace, &bot));

  // Decode opponent race
  search = CONFIG_RACE_MAP.find(StrOpts[StringOptions::OPPONENT_RACE]);

  if (search != CONFIG_RACE_MAP.end()) {
    LOG(INFO) << "[Coordinator::SetMyParticipants]: Provided race: '"
              << StrOpts[StringOptions::OPPONENT_RACE] << "'";
    OpponentRace = search->second;
  } else {
    LOG(WARNING) << "[Coordinator::SetMyParticipants]: Your race wasnt valid";
  }
  player_setup.emplace_back(sc2::CreateComputer(OpponentRace));

  SetParticipants(player_setup);
}

bool Coordinator::LaunchGame() {
  // Get map
  if (StrOpts[StringOptions::MAP].empty() == true) {
    LOG(ERROR) << "[Coordinator::LaunchGame]: Map options was not provided";
    return false;
  }

  LaunchStarcraft();
  return StartGame(StrOpts[StringOptions::MAP]);
}

void Coordinator::SetMyRenderer() {
  SuperBot &bot = SuperBot::GetSuperBot();

  sc2::RenderSettings settings;

  if (bot.LoadRendererConfigAndSettings(settings) > 0)
    SetRender(settings);
}
