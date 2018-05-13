#ifndef WAV_H
#define WAV_H

#include "aurora/Backend.h"

namespace aurora {

/// DefaultNumChannels is 1 (mono audio)
const int defaultNumChannels = 1;
/// DefaultSampleRate is 16KHz
const int defaultSampleRate = 16000;
/// DefaultAudioFormat is 1 (raw, uncompressed PCM waveforms)
const int defaultAudioFormat = 1;
/// DefaultBitsPerSample is 16 (2 bytes per sample).
const int defaultBitsPerSample = 16;


class WAV {
public:
  WAV();
  WAV(Buffer &audioData);
  WAV(int numChannels, int sampleRate, int audioFormat, int bitsPerSample, Buffer &audioData);
  virtual ~WAV();

  virtual int getSampleRate();
  virtual int getNumChannels();

  virtual void setData(Buffer &audioData);
  virtual Buffer getData();
private:
  int m_numChannels;
  int m_sampleRate;
  int m_audioFormat;
  int m_bitsPerSample;
  Buffer m_audioData;
};

} // namespace aurora

#endif
