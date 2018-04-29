#include <aurora/Backend.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <aurora/errors/AuroraError.h>
#include <vector>
#include <iterator>
#include <algorithm>

namespace aurora {

using json = nlohmann::json;

///libcurl callback when data is received
size_t writeFunction(void *ptr, size_t size, size_t nmemb, Buffer *data) {
  // add bytes to data string buffer
  //data->append((char*) ptr, size * nmemb);
  char *buffer = (char *) ptr;
  std::copy(buffer, buffer + (size * nmemb), std::back_inserter(*data));

  // return number of bytes taken care of
  // if different than amount given, causes curl error
  return size * nmemb;
}

Backend::Backend(const std::string &url) : m_baseURL(url) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  m_curl = curl_easy_init();

  if (!m_curl) {
    throw AuroraError("0", "curl failed to initialize", "no info");
  }
}

Backend::~Backend() {
  curl_easy_cleanup(m_curl);
  curl_global_cleanup();
}

HTTPResponse Backend::call(const CallParams &params) {
  // reset all curl options back to the defaults
  curl_easy_reset(m_curl);

  // HTTP response body
  Buffer responseBody;
  // HTTP headers
  Buffer responseHeader;
  // build full request URL
  std::string requestURL = buildRequestURL(params);

  // set HTTP request URL
  curl_easy_setopt(m_curl, CURLOPT_URL, requestURL.c_str());
  // set libcurl to call writeFunction when data is received
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeFunction);
  // write the response body
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &responseBody);
  // write the response header
  curl_easy_setopt(m_curl, CURLOPT_HEADERDATA, &responseHeader);

  // build headers
  // curl_slist MUST BE FREED after request
  struct curl_slist *reqHeaderList = configureHeaders(params);

  // perform HTTP request
  CURLcode res = curl_easy_perform(m_curl);

  // free headers
  curl_slist_free_all(reqHeaderList);

  // check if HTTP request failed
  if (res != CURLE_OK) {
    throw AuroraError("1", "curl request failed", curl_easy_strerror(res));
  }

  // get http response code
  long responseCode;
  curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &responseCode);

  return {responseBody, responseHeader, responseCode};
}

void Backend::setBaseURL(const std::string &url) {
  m_baseURL = url;
}

std::string Backend::buildQueryString(const std::unordered_map<std::string, std::vector<std::string>> &query) {
  std::string queryString;

  bool isFirst = true;
  for (const auto &keyValuesPair : query) {
    auto &key = keyValuesPair.first;
    auto &values = keyValuesPair.second;

    for (const auto &value : values) {
      // prepend seperator only if not first query item
      if (isFirst) isFirst = false;
      else queryString += "&";

      queryString.append(URLEncode(key)).append("=").append(URLEncode(value));
    }
  }

  return queryString;
}

std::string Backend::buildRequestURL(const CallParams &params) {
  std::string requestURL = m_baseURL + params.path + std::string("?") + buildQueryString(params.query);
  return requestURL;
}

std::string Backend::URLEncode(const std::string &str) {
  char *output = curl_easy_escape(m_curl, str.c_str(), 0);

  if(output) {
    std::string temp = output;
    // need to free resources allocated by curl
    curl_free(output);
    return temp;
  }

  throw AuroraError("0", "curl failed to url encode string", str);
}

struct curl_slist* Backend::configureHeaders(const CallParams &params) {
  // reqHeaderlist MUST BE FREED later
  struct curl_slist *reqHeaderList = NULL;
  // add header fields from params
  buildCurlHeaderList(reqHeaderList, params.headers);
  // add authentication headers
  buildCurlHeaderList(reqHeaderList, {{"X-Application-ID", {params.credentials.appID}},
                                      {"X-Application-Token", {params.credentials.appToken}},
                                      {"X-Device-ID", {params.credentials.deviceID}}});
  // tell curl to use our headers
  curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, reqHeaderList);

  // set HTTP method (GET, POST, etc.)
  curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, params.method.c_str());

  return reqHeaderList;
}

 struct curl_slist* Backend::buildCurlHeaderList(struct curl_slist *headerList,
                                        const std::unordered_map<std::string, std::vector<std::string>> &headers) {
  for (const auto &keyValuesPair : headers) {
    auto &key = keyValuesPair.first;
    auto &values = keyValuesPair.second;

    for (const auto &value : values) {
      // must not be CRLF terminated, curl does it for us
      std::string field = key + std::string(": ") +value; // "Key: value"
      // curl_slist_append copies the string, so okay to give it underlying c-string
      curl_slist_append(headerList, field.c_str());
    }
  }

  return headerList;
}

} // namespace aurora
