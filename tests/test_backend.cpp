#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <aurora/Backend.h>

#include <iostream>

namespace {

// The fixture for testing class Foo.
class BackendTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  BackendTest() {
    // You can do set-up work for each test here.
  }

  virtual ~BackendTest() {
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

TEST(BackendTest, DummyTest) {
  aurora::Backend b;
  aurora::CallParams cp;
  cp.query = {{"key1", {"value1", "value2"}}};
  auto res = b.call(cp);
  std::string body(res.response.begin(), res.response.end());
  std::cout << "CODE: " << res.code << std::endl
            << "BODY: " << body << std::endl;
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
