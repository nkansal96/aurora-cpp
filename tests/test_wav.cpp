#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <aurora/WAV.h>
#include <aurora/errors/AuroraError.h>
#include <vector>
#include <cinttypes>

using namespace aurora;
using namespace testing;

namespace {

TEST(WAVtests, DefaultConstructor) {
  WAV to_test;
  EXPECT_EQ(to_test.getSampleRate(), 16000);
  EXPECT_EQ(to_test.getNumChannels(), 1);
}

TEST(WAVtests, RawAudioConstructor) {
  int numChannels=12512;
  int sampleRate=34134;
  int audioFormat=3134;
  int bitsPerSample=47456;
  Buffer buf;
  for(char i=0; i<100; i++){
    buf.push_back(i);
  }
  WAV to_test(buf, numChannels, sampleRate, audioFormat, bitsPerSample);
  EXPECT_EQ(to_test.getSampleRate(), sampleRate);
  EXPECT_EQ(to_test.getNumChannels(), numChannels);
  EXPECT_EQ(to_test.audioData(), buf);
}

TEST(WAVtests, FormattedAudioConstructor) {
  Buffer raw_buf;
  for(char i=0; i<100; i++){
    raw_buf.push_back(i);
  }
  WAV RawAudioWav(raw_buf, defaultNumChannels, defaultSampleRate, defaultAudioFormat, defaultBitsPerSample);
  Buffer formatted_buf = RawAudioWav.data();
  WAV FormattedAudioWav(formatted_buf);

  EXPECT_EQ(RawAudioWav.getSampleRate(), FormattedAudioWav.getSampleRate());
  EXPECT_EQ(RawAudioWav.getNumChannels(), FormattedAudioWav.getNumChannels());
  EXPECT_EQ(RawAudioWav.data(), FormattedAudioWav.data());

}

//eventually make the wav array longer so the buffer is bigger, to test the values stored in data[5]- data[7]
TEST(WAVtests, DataTest) {
  int data_len = 100;
  Buffer buf;
  for(char i=0; i<data_len; i++){
    buf.push_back(i);
  }
  WAV to_test(buf, defaultNumChannels, defaultSampleRate, defaultAudioFormat, defaultBitsPerSample);
  Buffer data = to_test.data();
  EXPECT_EQ(data[0], 'R');
  EXPECT_EQ(data[1], 'I');
  EXPECT_EQ(data[2], 'F');
  EXPECT_EQ(data[3], 'F');
  //data length should be buf length + 36
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
  //metadata subchunk size (not sure what this means, but it's supposed to be set to 16)
  EXPECT_EQ(uint8_t(data[16]), uint8_t(16));
  EXPECT_EQ(uint8_t(data[17]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[18]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[19]), uint8_t(0));
  //audio_format (1 by default)
  EXPECT_EQ(uint8_t(data[20]), uint8_t(1));
  EXPECT_EQ(uint8_t(data[21]), uint8_t(0));
  //num_channels (1 by default)
  EXPECT_EQ(uint8_t(data[22]), uint8_t(1));
  EXPECT_EQ(uint8_t(data[23]), uint8_t(0));
  // Sample Rate (16000 Hz)
  EXPECT_EQ(uint8_t(data[24]), uint8_t(128));
  EXPECT_EQ(uint8_t(data[25]), uint8_t(62));
  EXPECT_EQ(uint8_t(data[26]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[27]), uint8_t(0));
  // Byte Rate (32000)
  EXPECT_EQ(uint8_t(data[28]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[29]), uint8_t(125));
  EXPECT_EQ(uint8_t(data[30]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[31]), uint8_t(0));
  // Block Align = NumChannels * BitsPerSample/8. 2 by default
  EXPECT_EQ(uint8_t(data[32]), uint8_t(2));
  EXPECT_EQ(uint8_t(data[33]), uint8_t(0));
  // BitsPerSample. 16 by default
  EXPECT_EQ(uint8_t(data[34]), uint8_t(16));
  EXPECT_EQ(uint8_t(data[35]), uint8_t(0));

  EXPECT_EQ(data[36], 'd');
  EXPECT_EQ(data[37], 'a');
  EXPECT_EQ(data[38], 't');
  EXPECT_EQ(data[39], 'a');

  //data len. Will also change on a bigger test case
  EXPECT_EQ(uint8_t(data[40]), uint8_t(100));
  EXPECT_EQ(uint8_t(data[41]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[42]), uint8_t(0));
  EXPECT_EQ(uint8_t(data[43]), uint8_t(0));

}

TEST(WAVtests, trimSilent){
  Buffer raw_buf;
  for(int i=0; i<2000; i++){
    raw_buf.push_back(0);
  }
  for(int i=0; i<2000; i++){
    raw_buf.push_back(127);
  }
  for(int i=0; i<2000; i++){
    raw_buf.push_back(0);
  }

  WAV test1(raw_buf, defaultNumChannels, defaultSampleRate, defaultAudioFormat, defaultBitsPerSample);
  test1.trimSilent(1, 0);
  EXPECT_EQ(test1.audioData().size(), 0);

  WAV test2(raw_buf, defaultNumChannels, defaultSampleRate, defaultAudioFormat, defaultBitsPerSample);
  test2.trimSilent(0.5, 0);
  EXPECT_GT(4000, test2.audioData().size());
  EXPECT_GT(test2.audioData().size(), 0);

  //check what happens when padding is too big. before fix, this would lead to infinite looping
  WAV test3(raw_buf, defaultNumChannels, defaultSampleRate, defaultAudioFormat, defaultBitsPerSample);
  test3.trimSilent(0.5, 0.25);
  EXPECT_EQ(6000, test3.audioData().size());

  //realistic use of trimSilent
  raw_buf.clear();
  for(int i=0; i<10000; i++){
    raw_buf.push_back(0);
  }
  for(int i=0; i<2000; i++){
    raw_buf.push_back(127);
  }
  for(int i=0; i<10000; i++){
    raw_buf.push_back(0);
  }

  WAV test4(raw_buf, defaultNumChannels, defaultSampleRate, defaultAudioFormat, defaultBitsPerSample);
  test4.trimSilent(0.03, 0.25);
  EXPECT_GT(4000, test4.audioData().size());
  EXPECT_GT(test4.audioData().size(), 0);

}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
