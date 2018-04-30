#include <aurora/Backend.h>
#include <cpr/cpr.h>
#include <vector>
#include <iterator>
#include <algorithm>

namespace aurora {

Backend::Backend(const std::string &url) : m_baseURL(url) {}

Backend::~Backend() {}

HTTPResponse Backend::call(CallParams &params) {
  cpr::Url url(m_baseURL + params.path);

  // copy over query parameters from params to cpr object
  cpr::Parameters reqParams;
  for (const auto &q : params.query) reqParams.AddParameter({q.first, q.second});

  // set authentication header fields
  cpr::Header credentials {{"X-Application-ID", params.credentials.appID},
                           {"X-Application-Token", params.credentials.appToken},
                           {"X-Device-ID", params.credentials.deviceID}};

  // execute request
  auto res = cpr::Get(url, reqParams, credentials);

  return {res.text, res.header, res.status_code};
}

void Backend::setBaseURL(const std::string &url) {
  m_baseURL = url;
}

} // namespace aurora
