#include <gmock/gmock.h>
#include <aurora/Backend.h>

class MockBackend : public aurora::Backend {
public:
  MOCK_METHOD1(call, aurora::HTTPResponse(aurora::CallParams &params));
  MOCK_METHOD4(get, cpr::Response(cpr::Url &url, cpr::Parameters &params, cpr::Header &header, cpr::Body &body));
  MOCK_METHOD4(post, cpr::Response(cpr::Url &url, cpr::Parameters &params, cpr::Header &header, cpr::Body &body));

  // call original Backend::call for partial mock
  aurora::HTTPResponse BackendCall(aurora::CallParams &params) { return Backend::call(params); }
};
