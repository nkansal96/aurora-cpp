#include <aurora/AudioFile.h>
#include <aurora/Speech.h>
#include <aurora/Text.h>
#include <aurora/Config.h>
#include <aurora/Backend.h>
#include <nlohmann/json.hpp>
#include <memory>

// mocks
#include "mocks/mock_backend.h"

using json = nlohmann::json;
using namespace aurora;
using namespace testing;

namespace {

TEST(SpeechTests, TextTest) {
  MockBackend *backend = new MockBackend();
  config.backend = std::unique_ptr<Backend>(backend);

  json j = {
    {"transcript", "hello world"}
  };

  HTTPResponse mockRes;
  mockRes.response = j.dump();
  mockRes.statusCode = 200;

  // replace call with mocked version
  ON_CALL(*backend, call(_)).WillByDefault(ReturnPointee(&mockRes));

  // API should only invoke call once
  EXPECT_CALL(*backend, call(_)).Times(1);

  Buffer b;
  AudioFile audioFile(b);
  Speech sp(audioFile);

  EXPECT_EQ(sp.text(), "hello world");
}

} // namespace
