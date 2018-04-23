#include "Backend.h"

aurora::Backend::Backend() {
  m_baseURL = baseURL;
  m_curl = curl_easy_init();
}

aurora::Backend::~Backend() {
  curl_easy_cleanup(m_curl);
}

void aurora::Backend::setBaseURL(std::string url) {
  m_baseURL = url;
}
