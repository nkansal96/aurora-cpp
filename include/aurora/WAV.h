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
  explicit WAV(Buffer &audioData);
  WAV(int numChannels, int sampleRate, int audioFormat, int bitsPerSample, Buffer &audioData);
  virtual ~WAV();

  virtual int getSampleRate();
  virtual int getNumChannels();

  /// returns raw audio data
  virtual Buffer& audioData();

  /// returns a fully formatted WAV file (headers + raw audio data)
  virtual Buffer data();
private:
  int m_numChannels;
  int m_sampleRate;
  int m_audioFormat;
  int m_bitsPerSample;

  /// raw audio data stored in the WAV file (no headers)
  Buffer m_audioData;
};

} // namespace aurora

#endif
