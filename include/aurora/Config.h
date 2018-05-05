#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>
#include "aurora/Backend.h"

namespace aurora {

struct Config {
  Config();
  explicit Config(Backend *b);
  ~Config();

  /// DeviceID is the value to send as the `X-Device-ID` header (optional)
  std::string deviceID;

  /// AppToken is the value to send as the `X-Application-Token` header
  std::string appToken;

  /// AppID is the value to send as the `X-Application-ID` header
  std::string appID;

  /// Backend to use for requests (configurable for testing purposes)
  Backend *backend;
};

/// global config object
extern Config config;

}
#endif //CONFIG_H
