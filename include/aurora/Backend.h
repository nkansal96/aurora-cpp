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
struct MultipartFile {
  /// Name is the form field name
  std::string name;
  /// Filename is the file name to upload the file as
  std::string filename;
  /// Data is the file data
  std::vector<char> data;
};

/// Credentials for the API request.
struct Credentials {
  /// AppID is the appliacation ID (sent for 'X-Application-ID' header)
  std::string appID;
  /// AppToken is the application token (sent for 'X-Application-Token' header)
  std::string appToken;
  /// DeviceID is the device ID (sent for 'X-Device-ID' header)
  std::string deviceID;
};

struct CallParams {
  /// Method is one of GET, POST, PATCH, DELETE, etc.
  std::string method;
  /// Path is the relative path of the query
  std::string path;
  /// Headers are any additional headers to send in the request
  std::unordered_map<std::string, std::vector<std::string>> headers;
  /// Body is any data to send in the body
  std::vector<char> body;
  /// Form is ignored for non-multipart calls. It is multipart form data
  std::unordered_map<std::string, std::vector<std::string>> form;
  /// Query is querystring parameters
  std::unordered_map<std::string, std::vector<std::string>> query;
  /// Files is ignored for non-multipart calls. It is multipart file data
  std::vector<MultipartFile> files;
  /// Credentials are the AppID, AppToken, etc. required to make the call
  Credentials credentials;
};

/// returned result of a successful call
struct HTTPResponse {
  std::string response;
  std::string header;
  long code;
};

class Backend
{
public:
  /**
   * @param url the base url string that all requests will use
   */
  explicit Backend(const std::string &url = baseURL);
  ~Backend();

  /// execute an HTTP request
  HTTPResponse call(const CallParams &params);

  void setBaseURL(const std::string &url);

private:
  /// the base url string that all requests will use
  std::string m_baseURL;

  CURL *m_curl;

  /// convert map of query params to a URL query string, for example "?key1=value1&key2=value2"
  std::string buildQueryString(const std::unordered_map<std::string, std::vector<std::string>> &query);

  /// builds the full request URL with base + path + query
  std::string buildRequestURL(const CallParams &params);

  /// converts string to URL encoded version
  std::string URLEncode(const std::string &str);

  /**
   * configures curl user headers, authentication headers, and HTTP method
   * @returns curl_slist* that MUST BE FREED with curl_slist_free_all(curl_slist*) after request completes
   */
  struct curl_slist* configureHeaders(const CallParams &params);

  /// appends header fields to a curl header list
  struct curl_slist* buildCurlHeaderList(struct curl_slist *headerList,
                                          const std::unordered_map<std::string, std::vector<std::string>> &headers);
};

} // namespace aurora

#endif // BACKEND_H
