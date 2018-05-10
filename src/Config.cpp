#include "aurora/Config.h"
#include "aurora/Backend.h"
#include <memory>

namespace aurora {

/// global config object
Config config;

Config::Config() : backend(new Backend()) {}

Config::~Config() {}

Credentials Config::getCredentials() {
  return {config.appID, config.appToken, config.deviceID};
}

}
