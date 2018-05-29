#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <memory>

namespace aurora {

// forward declarations
class Backend;
struct Credentials;


/// configuration structure for setting API authentication keys
class Config {
public:
  Config();
  ~Config();

  /// DeviceID is the value to send as the `X-Device-ID` header (optional)
  std::string deviceID;

  /// AppToken is the value to send as the `X-Application-Token` header
  std::string appToken;

  /// AppID is the value to send as the `X-Application-ID` header
  std::string appID;

  /// Backend to use for requests (configurable for testing purposes)
  std::unique_ptr<Backend> backend;

  /// bundles up deviceID, appToken, appID into a Credentials object
  Credentials getCredentials();
};

/// global config object
extern Config config;

}
#endif //CONFIG_H
