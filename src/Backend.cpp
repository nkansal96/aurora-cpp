#include "aurora/Backend.h"
#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include <iostream>

namespace aurora {

using json = nlohmann::json;

Backend::Backend() {
  m_baseURL = baseURL;

  curl_global_init(CURL_GLOBAL_DEFAULT);
}

Backend::~Backend() {
  curl_global_cleanup();
}

void Backend::call(CallParams &params) {
  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cout << "curl init failed" << std::endl;
  }

  curl_easy_setopt(curl, CURLOPT_URL, m_baseURL.c_str());

  CURLcode res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    std::cout << "curl req failed: " << curl_easy_strerror(res) << std::endl;
  }

  std::cout << "curl finished" << std::endl;
  curl_easy_cleanup(curl);
}

void Backend::setBaseURL(std::string url) {
  m_baseURL = url;
}

} // namespace aurora
