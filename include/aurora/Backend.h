#ifndef BACKEND_H
#define BACKEND_H

#include <string>
#include <unordered_map>
#include <vector>
#include <cpr/cpr.h>
#include <aurora/errors/AuroraError.h>
#include "aurora/utils.h"

namespace aurora {

/// default baseURL
const std::string baseURL = "https://api.auroraapi.com/v1";

// type aliases for convenience & readability
using Query = std::unordered_map<std::string, std::string>;
using Headers = cpr::Header; // map<string, string>
using Form = std::unordered_map<std::string, std::string>;

/// MultipartFile is an in-memory representation of a file to upload.
struct MultipartFile {
  /// Name is the form field name
  std::string name;
  /// Filename is the file name to upload the file as
  std::string filename;
  /// Data is the file data
  Buffer data;
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

/// supported HTTP methods
enum HTTPMethod {GET, POST};

/// parameter structure for call method
struct CallParams {
  /// Method is one of GET, POST
  HTTPMethod method;
  /// Path is the relative path of the query
  std::string path;
  /// Headers are any additional headers to send in the request
  Headers headers;
  /// Body is any data to send in the body
  Buffer body;
  /// Form is ignored for non-multipart calls. It is multipart form data
  Form form;
  /// Query is querystring parameters
  Query query;
  /// Files is ignored for non-multipart calls. It is multipart file data
  std::vector<MultipartFile> files;
  /// Credentials are the AppID, AppToken, etc. required to make the call
  Credentials credentials;
};

/// returned result of a successful call
struct HTTPResponse {
  /// response payload (body)
  std::string response;
  /// map of header keys to values
  Headers header;
  /// HTTP status code
  int statusCode;
};

/// Abstraction of HTTP request library
class Backend {
public:
  /**
   * @param url the base url string that all requests will use
   */
  explicit Backend(const std::string &url = baseURL);
  virtual ~Backend();

  /**
   * execute an HTTP request
   */
  virtual HTTPResponse call(CallParams &params);

  /// set base request url, will be suffixed by CallParams.path
  virtual void setBaseURL(const std::string &url);

private:
  /// the base url string that all requests will use
  std::string m_baseURL;

  /// wrapper around cpr::Get
  virtual cpr::Response get(cpr::Url &url, cpr::Parameters &params, cpr::Header &header, cpr::Body &body);

  /// wrapper around cpr::Post
  virtual cpr::Response post(cpr::Url &url, cpr::Parameters &params, cpr::Header &header, cpr::Body &body);
};

} // namespace aurora

#endif // BACKEND_H
