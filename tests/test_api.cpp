#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <aurora/Backend.h>
#include <aurora/API.h>
#include <aurora/Aurora.h>
#include <nlohmann/json.hpp>
#include <aurora/errors/AuroraError.h>
#include <aurora/errors/APIError.h>
#include <aurora/AudioFile.h>
#include <memory>

// mocks
#include "mocks/mock_backend.h"

using json = nlohmann::json;
using namespace aurora;
using namespace testing;

namespace {

TEST(APITests, InterpretTest) {
  MockBackend *backend = new MockBackend();
  config.backend = std::unique_ptr<Backend>(backend);

  json j = {
    {"intent", "greeting"},
    {"entities", {
        {"person", "bob"}
      }
    }
  };

  HTTPResponse mockRes;
  mockRes.response = j.dump();
  mockRes.statusCode = 200;

  auto mockedCall = [=]() { return mockRes; };

  // replace call with mocked version
  ON_CALL(*backend, call(_)).WillByDefault(InvokeWithoutArgs(mockedCall));

  // API should only invoke call once
  EXPECT_CALL(*backend, call(_)).Times(1);

  InterpretResponse res = API::getInterpret("hello world");
  // test intent field
  EXPECT_EQ(res.intent, "greeting");

  // test entites field
  EXPECT_EQ(res.entities["person"], "bob");
}

void missingFieldTests(json &j) {
  MockBackend *backend = new MockBackend();
  config.backend = std::unique_ptr<Backend>(backend);

  HTTPResponse mockRes;
  mockRes.response = j.dump();
  mockRes.statusCode = 200;

  auto mockedCall = [=]() { return mockRes; };

  // replace call with mocked version
  ON_CALL(*backend, call(_)).WillByDefault(InvokeWithoutArgs(mockedCall));

  // API should only invoke call once
  EXPECT_CALL(*backend, call(_)).Times(1);

  // exception thrown for missing field
  ASSERT_THROW(API::getInterpret("hello world"), AuroraError);
}

TEST(APITests, InterpretMissingIntentFieldException) {
  // missing intent field
  json j = {
    {"entities", {
        {"person", "bob"}
      }
    }
  };

  missingFieldTests(j);
}

TEST(APITests, InterpretMissingEntitiesFieldException) {
  // missing entities field
  json j = { {"intent", "greeting"} };

  missingFieldTests(j);
}

TEST(APITests, MissingCredentialsException) {
  MockBackend *backend = new MockBackend();
  config.backend = std::unique_ptr<Backend>(backend);

  json j = {
    {"id", "123"},
    {"status", 400},
    {"code", "MissingApplicationID"},
    {"type", "BadRequest"},
    {"message", "Your request is missing the 'X-Application-ID header."}
  };

  HTTPResponse mockRes;
  mockRes.response = j.dump();
  mockRes.statusCode = 400;

  auto mockedCall = [=]() { return mockRes; };

  // replace call with mocked version
  ON_CALL(*backend, call(_)).WillByDefault(InvokeWithoutArgs(mockedCall));

  // API should only invoke call once
  EXPECT_CALL(*backend, call(_)).Times(1);

  // exception thrown for 400 status code
  ASSERT_THROW(API::getInterpret("hello world"), APIError);
}

// test API::getSTT with normal server response
TEST(APITests, GetSTTTest) {
  MockBackend *backend = new MockBackend();
  config.backend = std::unique_ptr<Backend>(backend);

  json j = {
    {"transcript", "hello world"}
  };

  HTTPResponse mockRes;
  mockRes.response = j.dump();
  mockRes.statusCode = 200;

  // backend will return mockRes when called
  ON_CALL(*backend, call(_)).WillByDefault(ReturnPointee(&mockRes));

  EXPECT_CALL(*backend, call(_)).Times(1);

  Buffer b;
  AudioFile audioFile(b);
  std::string textFromSpeech = API::getSTT(audioFile);

  EXPECT_EQ(textFromSpeech, "hello world");
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
