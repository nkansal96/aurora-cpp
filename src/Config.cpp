#include "aurora/Config.h"

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

}
