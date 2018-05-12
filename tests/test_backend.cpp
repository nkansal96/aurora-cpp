#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <aurora/Backend.h>
#include <aurora/errors/AuroraError.h>
#include <memory>


// mocks
#include "mocks/mock_backend.h"

using namespace aurora;
using namespace testing;

namespace {

// matches cpr::Parameters with content string
MATCHER_P(EqParamsContent, content, "") { return arg.content == content; }

TEST(BackendTests, CPRParamTest) {
  MockBackend backend;
  backend.setBaseURL("abc");

  CallParams params;
  params.method = GET;
  params.path = "123";
  params.body  = {'0', '1'};
  params.query = { {"key", "value"} };

  cpr::Body expectBody("01");
  cpr::Url expectUrl("abc123");
  cpr::Parameters expectParams { {"key", "value"} };

  cpr::Response fakeRes;
  ON_CALL(backend, get(_, _, _, _)).WillByDefault(ReturnPointee(&fakeRes));
  EXPECT_CALL(backend, get(Eq(expectUrl), EqParamsContent(expectParams.content), _, Eq(expectBody))).Times(1);

  HTTPResponse res = backend.BackendCall(params);
}

TEST(BackendTests, CPRExceptionTest) {
  MockBackend backend;

  CallParams params;
  params.method = GET;

  cpr::Response fakeRes;
  fakeRes.error.code = cpr::ErrorCode::INVALID_URL_FORMAT;
  ON_CALL(backend, get(_, _, _, _)).WillByDefault(ReturnPointee(&fakeRes));

  // get should be called 1 time because params.method is GET
  EXPECT_CALL(backend, get(_, _, _, _)).Times(1);

  // post should not be called
  EXPECT_CALL(backend, post(_, _, _, _)).Times(0);

  // exception thrown because of CPR error (bad url)
  ASSERT_THROW(backend.BackendCall(params), AuroraError);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
