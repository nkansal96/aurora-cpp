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

TEST(TextTests, InterpretTest) {
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
