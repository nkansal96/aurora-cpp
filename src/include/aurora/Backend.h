#ifndef BACKEND_H
#define BACKEND_H

#include <string>
#include <unordered_map>
#include <vector>
#include <curl/curl.h>

namespace aurora {
  const std::string baseURL = "http://api.auroraapi.com"; // TODO: https

  typedef struct {
    std::string name;
    std::string filename;
    std::vector<char> data;
  } MultipartFile;

  // Credentials for the API request.
  typedef struct {
    // AppID is the appliacation ID (sent for 'X-Application-ID' header)
    std::string appID;
    // AppToken is the application token (sent for 'X-Application-Token' header)
    std::string appToken;
    // DeviceID is the device ID (sent for 'X-Device-ID' header)
    std::string deviceID;
  } Credentials;

  typedef struct {
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::vector<std::string> > headers;
    std::vector<char> body;
    std::unordered_map<std::string, std::vector<std::string> > form;
    std::unordered_map<std::string, std::vector<std::string> > query;
    std::vector<MultipartFile> files;
    Credentials credential;
  } CallParams;

  class Backend {
  public:
    Backend();
    ~Backend();

    void setBaseURL(std::string url);
  private:
    std::string m_baseURL;
    CURL *m_curl;
  };
}

#endif // BACKEND_H
