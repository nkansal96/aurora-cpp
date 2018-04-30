#include <aurora/HTTPClient.h>
#include <aurora/errors/AuroraError.h>
#include <string>
#include <curl/curl.h>

namespace aurora {

HTTPClient::HTTPClient() {
  setup();
}

HTTPClient::~HTTPClient() {
  cleanup();
}

void HTTPClient::setup() {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  m_curl = curl_easy_init();

  if (!m_curl) {
    throw AuroraError("0", "curl failed to initialize", __FUNCTION__);
  }
}
void HTTPClient::cleanup() {
  curl_easy_cleanup(m_curl);
  curl_global_cleanup();
}

void HTTPClient::reset() {
  // reset all options back to defaults
  curl_easy_reset(m_curl);
}

long HTTPClient::getInfo(CURLINFO flag) {
  long info = 0;
  curl_easy_getinfo(m_curl, flag, &info);
  return info;
}

/// execute HTTP request
CURLcode HTTPClient::perform() {
  return curl_easy_perform(m_curl);
}

std::string HTTPClient::stringFromError(CURLcode code) {
  return curl_easy_strerror(code);
}

std::string HTTPClient::escape(const std::string &str) {
  char *output = curl_easy_escape(m_curl, str.c_str(), 0);
  if(output) {
    std::string temp = output;
    // need to free resources allocated by curl
    curl_free(output);
    return temp;
  }

  throw AuroraError("0", "curl failed to url encode string", __FUNCTION__);
}

/// add chunk to slist, will be copied
void HTTPClient::slistAppend(struct curl_slist *ls, const char *chunk) {
  curl_slist_append(ls, chunk);
}

/// free resources allocated by an slist
void HTTPClient::freeSlist(struct curl_slist *ls) {
  curl_slist_free_all(ls);
}

} // namespace
