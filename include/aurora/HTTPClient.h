#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <string>
#include <curl/curl.h>
#include <aurora/errors/AuroraError.h>

namespace aurora {

typedef void (*funcptr)(void);

/// wraps the curl library so that methods can be mocked for unit tests
class HTTPClient {
public:
  HTTPClient();
  ~HTTPClient();

  virtual void setup();
  virtual void cleanup();

  /// clear all options
  virtual void reset();
  /// T can be several different types: object ptr, function pointer, long...
  /// TODO: can't make virtual template method... workaround?
  template <typename T>
  void setOpt(CURLoption opt, T arg);
  /// @returns info for curl flag
  virtual long getInfo(CURLINFO flag);
  /// execute HTTP request
  virtual CURLcode perform();
  virtual std::string stringFromError(CURLcode code);
  virtual std::string escape(const std::string &str);

  /// add chunk to slist, will be copied
  virtual void slistAppend(struct curl_slist *ls, const char *chunk);
  /// free resources allocated by an slist
  virtual void freeSlist(struct curl_slist *ls);
private:
  /// underlying curl handle
  CURL *m_curl;
};

// template implementations

template <typename T>
void HTTPClient::setOpt(CURLoption opt, T arg) {
  const CURLcode code = curl_easy_setopt(m_curl, opt, arg);

  if (code != CURLE_OK) {
    throw AuroraError("1", "curl setopt failed", __FUNCTION__);
  }
}

} // namespace aurora

#endif // HTTPCLIENT_H
