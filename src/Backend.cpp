#include <aurora/Backend.h>
#include <cpr/cpr.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <future>

namespace aurora {

Backend::Backend(const std::string &url) : m_baseURL(url) {}

Backend::~Backend() {}

std::future<HTTPResponse> Backend::call(CallParams &params) {
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

  // wrapper lambda that converts cpr::Response to HTTPResponse
  auto callback = [=](cpr::Response res) {
    return HTTPResponse{res.text, res.header, res.status_code};
  };

  // execute request
  // TODO: destrucors of futures cuase blocking
  if (params.method == "GET")
    return cpr::GetCallback(callback, url, reqParams, credentials, body);
  else if (params.method == "POST")
    return cpr::PostCallback(callback, url, reqParams, credentials, body);
  else
    throw AuroraError("0", "Unrecognized HTTP method", params.method);
}

void Backend::setBaseURL(const std::string &url) {
  m_baseURL = url;
}

} // namespace aurora
