#include "aurora/Config.h"
#include "aurora/Backend.h"

namespace aurora {

/// global config object
Config config;

Config::Config() {
  backend = new Backend();
}

Config::Config(Backend *b) {
  backend = b;
}

Config::~Config() {
  delete backend;
}

void Config::setBackend(Backend *b) {
  delete backend;
  backend = b;
}

Credentials Config::getCredentials() {
  return {config.appID, config.appToken, config.deviceID};
}

}
