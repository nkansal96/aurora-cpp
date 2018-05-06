#include <gmock/gmock.h>
#include <aurora/Backend.h>

class MockBackend : public aurora::Backend {
public:
  MOCK_METHOD1(call, aurora::HTTPResponse(aurora::CallParams &params));
};
