// File:           config.cpp
// Description:    Implementation of the config class
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Fri Jun 01 2018 04:31
// Last Modified:  Fri Jun 01 2018 04:31

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <glog/logging.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "config.hpp"

void Config::ParseArguments(int num_options, const char *options[]) {
  if (num_options < 1)
    return;

  if (options == nullptr)
    return;

  std::vector<std::string> _options;
  _options.reserve(8);

  for (int k = 0; k < num_options; k++) {
    LOG(INFO) << "cmd line arg#" << k << " = " << options[k] << "\n";
    _options.push_back(options[k]);
  }

  // Search for config option -c <file location>
  std::string::size_type sz = CMD_OPTION.size();
  for (auto &opt : _options) {
    LOG(INFO) << "opt.compare = "
              << opt.compare(0, CMD_OPTION.size(), CMD_OPTION) << " \n";

    if ((!opt.empty()) && (opt.size() > sz) &&
        (opt.compare(0, sz, CMD_OPTION) == 0)) {
      LOG(INFO) << "Found config option: " << opt << "\n";
      FileLocation = opt.substr(sz + 1);
      break;
    }
  }

  if (FileLocation.empty())
    LOG(WARNING) << "Configuration option (-c) not provided, using default\n";
}

int Config::LoadConfigFile(void) {
  if (FileLocation.empty())
    return -1;

  std::ostringstream file_content;

  {
    std::ifstream ifs(FileLocation);

    if (ifs.is_open() == false) {
      LOG(ERROR) << "Failed to open file: " << FileLocation << '\n';
      return -2;
    }

    LOG(INFO) << "FileLocation = " << FileLocation.c_str() << '\n';

    file_content << ifs.rdbuf();
  } // Close ifs

  rapidjson::ParseResult rc = File.Parse(file_content.str().c_str());

  if (rc.IsError() == true) {
    LOG(ERROR) << "Error (" << static_cast<unsigned>(rc.Offset())
               << "): " << rapidjson::GetParseError_En(rc.Code()) << '\n';
    return -3;
  }

  return 1;
}

/// @brief Handle the case where no command options are sent
/// @param num_options Number of arguments passed on to the executable
/// @param arguments Array of arguments
/// @return <0 in case of error
int Config::ParseConfigFile(int num_options, const char **arguments) {
  if (num_options < 1)
    return -1;

  if (num_options > 1)
    ParseArguments(num_options, arguments);
  return LoadConfigFile();
}

int Config::IsThereObjectWithMember(const char *object,
                                    const char *member) const {
  if ((object == nullptr) || (object[0] == 0)) {
    LOG(ERROR) << "[Config::GetValue]: Bad function input: 'object'";
    return -1;
  }

  if ((member == nullptr) || (member[0] == 0)) {
    LOG(ERROR) << "[Config::GetValue]: Bad function input: 'member'";
    return -2;
  }

  if (File.HasMember(object) == false) {
    LOG(ERROR) << "[Config::GetValue]: There is no configuration: '" << object
               << "'";
    return -3;
  }

  if (File[object].IsObject() == false) {
    LOG(ERROR) << "[Config::GetValue]: '" << object
               << "'"
                  "is not a rapidjson::Object";
    return -4;
  }

  if (File[object].HasMember(member) == false) {
    LOG(ERROR) << "[Config::GetValue]: Object '" << object << "' "
               << "doesn't have member '" << member << "'";
    return -5;
  }

  return 1;
}

int Config::GetValue(const char *object, const char *member,
                     std::string &value) const {
  if (IsThereObjectWithMember(object, member) < 1)
    return -1;

  if (File[object][member].IsString() == false) {
    LOG(ERROR) << "[Config::GetValue]: member '" << member
               << "' is not a string";
    return -2;
  }

  value = File[object][member].GetString();
  return 1;
}

int Config::GetValue(const char *object, const char *member,
                     double &value) const {
  if (IsThereObjectWithMember(object, member) < 1)
    return -1;

  if (File[object][member].IsDouble() == false) {
    LOG(ERROR) << "[Config::GetValue]: member '" << member
               << "' is not a string";
    return -2;
  }

  value = File[object][member].GetDouble();
  return 1;
}

int Config::GetValue(const char *object, const char *member, int &value) const {
  if (IsThereObjectWithMember(object, member) < 1)
    return -1;

  if (File[object][member].IsInt() == false) {
    LOG(ERROR) << "[Config::GetValue]: member '" << member
               << "' is not a string";
    return -2;
  }

  value = File[object][member].GetInt();
  return 1;
}

int Config::GetValue(const char *object, const char *member,
                     bool &value) const {
  if (IsThereObjectWithMember(object, member) < 1)
    return -1;

  if (File[object][member].IsBool() == false) {
    LOG(ERROR) << "[Config::GetValue]: member '" << member
               << "' is not a string";
    return -2;
  }

  value = File[object][member].GetBool();
  return 1;
}
