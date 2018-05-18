#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <aurora/WAV.h>
#include <aurora/errors/AuroraError.h>
#include <memory>
#include <vector>
#include <cinttypes>


// mocks
#include "mocks/mock_backend.h"

using namespace aurora;
using namespace testing;

using Buffer = std::vector<char>;

namespace {

TEST(WAVtests, default_constructor) {
  WAV to_test;
  EXPECT_EQ(to_test.getSampleRate(), 16000);
  EXPECT_EQ(to_test.getNumChannels(), 1);
}

TEST(WAVtests, one_arg_constructor) {
  Buffer buf;
  for(char i=0; i<100; i++){
    buf.push_back(i);
  }
  WAV to_test(buf);
  EXPECT_EQ(to_test.getSampleRate(), 16000);
  EXPECT_EQ(to_test.getNumChannels(), 1);
  EXPECT_EQ(to_test.audioData(), buf);
}

TEST(WAVtests, five_arg_constructor) {
  int numChannels=12512;
  int sampleRate=34134;
  int audioFormat=3134;
  int bitsPerSample=47456;
  Buffer buf;
  for(char i=0; i<100; i++){
    buf.push_back(i);
  }
  WAV to_test(numChannels, sampleRate, audioFormat, bitsPerSample, buf);
  EXPECT_EQ(to_test.getSampleRate(), sampleRate);
  EXPECT_EQ(to_test.getNumChannels(), numChannels);
  EXPECT_EQ(to_test.audioData(), buf);
}

//eventually make the wav array longer so the buffer is bigger -- tests data 5-7
TEST(WAVtests, data) {
  Buffer buf;
  for(char i=0; i<100; i++){
    buf.push_back(i);
  }
  WAV to_test(buf);
  Buffer data = to_test.data();
  EXPECT_EQ(data[0], 'R');
  EXPECT_EQ(data[1], 'I');
  EXPECT_EQ(data[2], 'F');
  EXPECT_EQ(data[3], 'F');
  EXPECT_EQ(uint8_t(data[4]), uint8_t(136));
  EXPECT_EQ(uint8_t(data[5]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[6]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[7]), uint8_t(0));
  EXPECT_EQ(data[8], 'W');
  EXPECT_EQ(data[9], 'A');
  EXPECT_EQ(data[10], 'V');
  EXPECT_EQ(data[11], 'E');
  EXPECT_EQ(data[12], 'f');
  EXPECT_EQ(data[13], 'm');
  EXPECT_EQ(data[14], 't');
  EXPECT_EQ(data[15], ' ');
  EXPECT_EQ(uint8_t(data[16]), uint8_t(16));
  EXPECT_EQ(uint8_t(data[17]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[18]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[19]), uint8_t(0));

}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
