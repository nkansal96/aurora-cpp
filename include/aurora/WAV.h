#ifndef WAV_H
#define WAV_H

#include "aurora/utils.h"

namespace aurora {

/// DefaultNumChannels is 1 (mono audio)
const uint16_t defaultNumChannels = 1;
/// DefaultSampleRate is 16KHz
const uint32_t defaultSampleRate = 16000;
/// DefaultAudioFormat is 1 (raw, uncompressed PCM waveforms)
const uint16_t defaultAudioFormat = 1;
/// DefaultBitsPerSample is 16 (2 bytes per sample).
const uint16_t defaultBitsPerSample = 16;

/**
 * WAV represents a PCM audio file in the WAV container format. It keeps
 * a high-level description of the parameters of the file, along with the
 * raw audio bytes, until it needs to be written to a file, stream, or array.
 */
class WAV {
public:
  WAV();
  explicit WAV(Buffer &data);
  WAV(Buffer &audioData, int numChannels, int sampleRate, int audioFormat, int bitsPerSample);
  virtual ~WAV();

  virtual int getSampleRate();
  virtual int getNumChannels();
  virtual int getBitsPerSample();

  /// calculates bytes used per frame of WAV audio data (channels * sample size)
  virtual int bytesPerFrame();

  /// trims silence from both ends of the audio data
  virtual void trimSilent(double threshold, double padding);

  /// append the passed-in audio bytes to the raw audio data
  virtual void appendAudioData(Buffer &b);

  /// prepend the passed-in audio bytes to the raw audio data
  virtual void prependAudioData(Buffer &b);

  /// returns raw audio data
  virtual Buffer& audioData();

  /// returns a fully formatted WAV file (headers + raw audio data)
  virtual Buffer data() const;
private:
  uint16_t m_numChannels;
  uint32_t m_sampleRate;
  uint16_t m_audioFormat;
  uint16_t m_bitsPerSample;

  /// raw audio data stored in the WAV file (no headers)
  Buffer m_audioData;
};

} // namespace aurora

#endif
