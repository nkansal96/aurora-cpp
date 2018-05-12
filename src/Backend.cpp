#include <aurora/Backend.h>
#include <cpr/cpr.h>
#include <vector>
#include <string>
#include <utility>
#include <iostream>

namespace aurora {

Backend::Backend(const std::string &url) : m_baseURL(url) {}

Backend::~Backend() {}

HTTPResponse Backend::call(CallParams &params) {
  cpr::Url url(m_baseURL + params.path);

  // copy over query parameters from params to cpr object
  cpr::Parameters reqParams;
  for (const auto &q : params.query)
    reqParams.AddParameter({q.first, q.second});

  // set authentication header fields
  cpr::Header credentials {{"X-Application-ID", params.credentials.appID},
                           {"X-Application-Token", params.credentials.appToken},
                           {"X-Device-ID", params.credentials.deviceID}};

  cpr::Body body(params.body.data(), params.body.size());

  // execute request
  cpr::Response res;
  switch (params.method) {
  case GET:
    res =  get(url, reqParams, credentials, body);
    break;
  case POST:
    res = post(url, reqParams, credentials, body);
    break;
  default:
    // should be impossible to reach, unless we forget a case in the switch statement
    throw AuroraError("BackendUnknownHTTPMethod", "Unrecognized HTTP method", "Backend::call");
  }

  // check for CPR/libcurl errors
  if (res.error) {
    throw AuroraError("CPRError", "CPR response error", res.error.message);
  }

  // convert to HTTPResponse
  // 'move' prevents making a copy of string data (reuses data in heap)
  return {std::move(res.text), res.header, res.status_code};
}

void Backend::setBaseURL(const std::string &url) {
  m_baseURL = url;
}

cpr::Response Backend::get(cpr::Url &url, cpr::Parameters &params, cpr::Header &header, cpr::Body &body) {
  return std::move(cpr::Get(url, params, header, body));
}

cpr::Response Backend::post(cpr::Url &url, cpr::Parameters &params, cpr::Header &header, cpr::Body &body) {
  return std::move(cpr::Post(url, params, header, body));
}

} // namespace aurora
