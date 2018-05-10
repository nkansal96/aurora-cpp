#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <aurora/Backend.h>
#include <aurora/Aurora.h>
#include <nlohmann/json.hpp>
#include <aurora/Interpret.h>
#include <memory>


// mocks
#include "mocks/mock_backend.h"

using json = nlohmann::json;
using namespace aurora;
using namespace testing;

namespace {

// The fixture for testing class Foo.
class TextTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  TextTest() {
    // You can do set-up work for each test here.
  }

  virtual ~TextTest() {
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

TEST(TextTest, InterpretTest) {
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

  Text tx = Text("hello world");
  Interpret res = tx.interpret();
  // test intent field
  EXPECT_EQ(res.intent, "greeting");

  // test entites field
  std::string person = res.entities["person"];
  EXPECT_EQ(person, "bob");
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
