#ifndef BACKEND_H
#define BACKEND_H

#include <string>
#include <unordered_map>
#include <vector>
#include <curl/curl.h>
#include "aurora/errors/APIError.h"

namespace aurora {

const std::string baseURL = "https://api.auroraapi.com";

/// MultipartFile is an in-memory representation of a file to upload.
struct MultipartFile
{
  /// Name is the form field name
  std::string name;
  /// Filename is the file name to upload the file as
  std::string filename;
  /// Data is the file data
  std::vector<char> data;
};

/// Credentials for the API request.
struct Credentials
{
  /// AppID is the appliacation ID (sent for 'X-Application-ID' header)
  std::string appID;
  /// AppToken is the application token (sent for 'X-Application-Token' header)
  std::string appToken;
  /// DeviceID is the device ID (sent for 'X-Device-ID' header)
  std::string deviceID;
};

struct CallParams
{
  std::string method;
  std::string path;
  std::unordered_map<std::string, std::vector<std::string>> headers;
  std::vector<char> body;
  std::unordered_map<std::string, std::vector<std::string>> form;
  std::unordered_map<std::string, std::vector<std::string>> query;
  std::vector<MultipartFile> files;
  Credentials credential;
};

class Backend
{
public:
  Backend();
  ~Backend();

  void call(CallParams &params);

  void setBaseURL(std::string url);

private:
  std::string m_baseURL;
};

} // namespace aurora

#endif // BACKEND_H
