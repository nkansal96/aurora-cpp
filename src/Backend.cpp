#include <aurora/Backend.h>
#include <iostream>
#include <cpr/cpr.h>
#include <vector>
#include <iterator>
#include <algorithm>

namespace aurora {

Backend::Backend(const std::string &url) : m_baseURL(url) {}

Backend::~Backend() {}

HTTPResponse Backend::call(CallParams &params) {
  cpr::Url url(m_baseURL + params.path);
  cpr::Parameters query = convertParameters(params.query);
  cpr::Header credentials {{"X-Application-ID", params.credentials.appID},
                           {"X-Application-Token", params.credentials.appToken},
                           {"X-Device-ID", params.credentials.deviceID}};

  // execute request
  auto r = cpr::Get(url, query, credentials);

  // convert to format not used by library
  std::unordered_map<std::string, std::string> header(r.header.begin(), r.header.end());

  return {r.text, header, r.status_code};
}

cpr::Parameters Backend::convertParameters(std::unordered_map<std::string, std::vector<std::string>> query) {
  cpr::Parameters params;

  for (const auto &keyValuesPair : query) {
    auto &key = keyValuesPair.first;
    auto &values = keyValuesPair.second;

    for (const auto &value : values) {
      params.AddParameter({key, value});
    }
  }

  return params;
}

void Backend::setBaseURL(const std::string &url) {
  m_baseURL = url;
}

} // namespace aurora
