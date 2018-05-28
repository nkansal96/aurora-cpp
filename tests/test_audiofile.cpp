#include <fstream>
#include <aurora/AudioFile.h>
#include <aurora/WAV.h>
#include <aurora/utils.h>
#include <aurora/errors/AuroraError.h>
#include <aurora/utils.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <cstdio>

#ifndef TESTS_ASSETS_PATH
  #define TESTS_ASSETS_PATH ""
#endif

const int WAV_HEADER_SIZE = 44;

using namespace aurora;
using namespace testing;

namespace {

TEST(AudioFileTests, PadRightTest) {
  int numChannels = 1;
  int sampleRate = 16000;
  int bitsPerSample = 16;
  int bytesPerSample = bitsPerSample / 8;
  int audioFormat = 1;
  float secondsToPad = 1.0;

  Buffer emptyBuffer;
  WAV wav(emptyBuffer, numChannels, sampleRate, audioFormat, bitsPerSample);
  AudioFile file(wav);
  file.padRight(secondsToPad);

  Buffer paddedWAVBuffer = file.getWAVData();
  WAV paddedWAV(paddedWAVBuffer);

  int paddingSize = numChannels * bytesPerSample * (int)((float)sampleRate * secondsToPad);

  EXPECT_EQ(paddedWAV.audioData().size(), paddingSize);
}

TEST(AudioFileTests, PadLeftTest) {
  int numChannels = 1;
  int sampleRate = 16000;
  int bitsPerSample = 16;
  int bytesPerSample = bitsPerSample / 8;
  int audioFormat = 1;
  float secondsToPad = 1.0;

  Buffer emptyBuffer;
  WAV wav(emptyBuffer, numChannels, sampleRate, audioFormat, bitsPerSample);
  AudioFile file(wav);
  file.padLeft(secondsToPad);

  Buffer paddedWAVBuffer = file.getWAVData();
  WAV paddedWAV(paddedWAVBuffer);

  int paddingSize = numChannels * bytesPerSample * (int)((float)sampleRate * secondsToPad);

  EXPECT_EQ(paddedWAV.audioData().size(), paddingSize);
}

TEST(AudioFileTests, PadTest) {
  int numChannels = 1;
  int sampleRate = 16000;
  int bitsPerSample = 16;
  int bytesPerSample = bitsPerSample / 8;
  int audioFormat = 1;
  float secondsToPad = 1.0;

  Buffer emptyBuffer;
  WAV wav(emptyBuffer, numChannels, sampleRate, audioFormat, bitsPerSample);
  AudioFile file(wav);
  file.pad(secondsToPad);

  Buffer paddedWAVBuffer = file.getWAVData();
  WAV paddedWAV(paddedWAVBuffer);

  int paddingSize = 2 * numChannels * bytesPerSample * (int)((float)sampleRate * secondsToPad);

  EXPECT_EQ(paddedWAV.audioData().size(), paddingSize);
}

TEST(AudioFileTests, OpenFileTest) {
  ASSERT_NO_THROW({
      AudioFile file(std::string(TESTS_ASSETS_PATH) + "hw.wav");
      Buffer wavData = file.getWAVData();

      // wav data should have header + raw audio
      EXPECT_GT(wavData.size(), WAV_HEADER_SIZE);
    });
}

TEST(AudioFileTests, WriteFileTest) {
  std::string copyFilePath = std::string(TESTS_ASSETS_PATH) + "hw-copy.wav";

  int numChannels = 1;
  int sampleRate = 16000;
  int bitsPerSample = 16;
  int bytesPerSample = bitsPerSample / 8;
  int audioFormat = 1;
  float secondsToPad = 1.0;

  Buffer emptyBuffer;
  WAV wav(emptyBuffer, numChannels, sampleRate, audioFormat, bitsPerSample);
  int wavSize = wav.data().size();

  ASSERT_NO_THROW({
      AudioFile file(wav);
      file.writeToFile(copyFilePath);
    });

  std::ifstream fileStream(copyFilePath, std::ifstream::ate | std::ifstream::binary);

  EXPECT_EQ(fileStream.tellg(), wavSize);

  // clean up copied file
  std::remove(copyFilePath.c_str());
}

}
