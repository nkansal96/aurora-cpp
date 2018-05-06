#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <aurora/Backend.h>
#include <aurora/API.h>
#include <aurora/Aurora.h>
#include <nlohmann/json.hpp>
#include <aurora/errors/AuroraError.h>
#include <iostream>

// mocks
#include "mocks/mock_backend.h"

using json = nlohmann::json;
using namespace aurora;
using namespace testing;

namespace {

// The fixture for testing class Foo.
class APITest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  APITest() {
    // You can do set-up work for each test here.
  }

  virtual ~APITest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.

};

TEST(APITest, InterpretTest) {
  MockBackend *backend = new MockBackend();
  config.setBackend(backend);

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
  config.setBackend(backend);

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

TEST(APITest, InterpretMissingIntentFieldException) {
  // missing intent field
  json j = {
    {"entities", {
        {"person", "bob"}
      }
    }
  };

  missingFieldTests(j);
}

TEST(APITest, InterpretMissingEntitiesFieldException) {
  // missing entities field
  json j = { {"intent", "greeting"} };

  missingFieldTests(j);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
