#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>
#include "aurora/Backend.h"

namespace aurora {

class Config {
public:
  Config();
  /// Config will handle memory management of the argument Backend
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

  /// will clean up resources used by old backend before assigning new value
  void setBackend(Backend *b);
};

/// global config object
extern Config config;

}
#endif //CONFIG_H
