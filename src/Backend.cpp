#include <aurora/Backend.h>
#include <aurora/HTTPClient.h>
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
  m_client = new HTTPClient();
}

Backend::Backend(const std::string &url, HTTPClient *client)
  : m_baseURL(url), m_client(client) {}

Backend::~Backend() {
  delete m_client;
}

HTTPResponse Backend::call(CallParams &params) {
  // reset all options back to the defaults
  m_client->reset();

  // HTTP response body
  Buffer responseBody;
  // HTTP headers
  Buffer responseHeader;
  // build full request URL
  std::string requestURL = buildRequestURL(params);

  // set HTTP request URL
  m_client->setOpt(CURLOPT_URL, requestURL.c_str());
  // set libcurl to call writeFunction when data is received
  m_client->setOpt(CURLOPT_WRITEFUNCTION, writeFunction);
  // write the response body
  m_client->setOpt(CURLOPT_WRITEDATA, &responseBody);
  // write the response header
  m_client->setOpt(CURLOPT_HEADERDATA, &responseHeader);

  // build headers
  // curl_slist MUST BE FREED after request
  struct curl_slist *reqHeaderList = configureHeaders(params);

  // perform HTTP request
  CURLcode res = m_client->perform();

  // free headers
  m_client->freeSlist(reqHeaderList);

  // check if HTTP request failed
  if (res != CURLE_OK) {
    throw AuroraError("1", "curl request failed", curl_easy_strerror(res));
  }

  // get http response code
  long responseCode = m_client->getInfo(CURLINFO_RESPONSE_CODE);

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

      queryString.append(m_client->escape(key)).append("=").append(m_client->escape(value));
    }
  }

  return queryString;
}

std::string Backend::buildRequestURL(const CallParams &params) {
  std::string requestURL = m_baseURL + params.path + std::string("?") + buildQueryString(params.query);
  return requestURL;
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
  m_client->setOpt(CURLOPT_HTTPHEADER, reqHeaderList);

  // set HTTP method (GET, POST, etc.)
  m_client->setOpt(CURLOPT_CUSTOMREQUEST, params.method.c_str());

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
      m_client->slistAppend(headerList, field.c_str());
    }
  }

  return headerList;
}

} // namespace aurora
