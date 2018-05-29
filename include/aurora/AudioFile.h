#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include "aurora/WAV.h"
#include <string>
#include <portaudio.h>

namespace aurora {

// audio file write defaults
const int AUDIO_FILE_FORMAT = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
const PaSampleFormat PORTAUDIO_SAMPLE_FORMAT = paInt16; // 16 bits
using AudioSampleType = short; // short = 2 bytes = 16 bits
const int BITS_PER_BYTE = 8;
const int BITS_PER_SAMPLE = sizeof(AudioSampleType) * BITS_PER_BYTE;
const int BYTES_PER_SAMPLE = sizeof(AudioSampleType);


/**
 * AudioFile wraps the raw WAV data and allows you
 * to operate with it using high-level operations, such as padding, trimming,
 * playback, writing to file, recording, etc.
 */
class AudioFile {
public:
  /// create an audio file from a byte buffer containing WAV data
  explicit AudioFile(Buffer &b);
  /// ceate an AudioFile by specifying the file name
  explicit AudioFile(const std::string &filename);
  /// construct an audio file from a WAV object
  explicit AudioFile(const WAV &wav);
  /**
   * Create a new audio file by recording from the default input
   * @param length specifies the length of the recording in seconds
   * @param silenceLen specifies in seconds how much consecutive silence
   * to wait before ending the recording
   */
  AudioFile(float length, float silenceLen);

  virtual ~AudioFile();

  /// writes the audio data to a file
  virtual void writeToFile(const std::string &filename);

  /**
   * Pad adds silence to both the beginning and end of the audio data
   * @param seconds the duration of silence to pad with, in seconds
   */
  virtual void pad(float seconds);

  /// add silence to the beginning of the audio data
  virtual void padLeft(float seconds);

  /// add silence to the end of the audio data
  virtual void padRight(float seconds);

  /// trim silence from both ends of the audio file
  virtual void trimSilence();

  /// play the audio
  virtual void play();

  /// stop the audio playback immediately
  virtual void stop();

  /// returns the full WAV file data (headers + raw audio) contained in the audio file
  // TODO: return a reference
  virtual Buffer getWAVData() const;
private:
  WAV m_audioData;
  bool m_shouldStop;
  bool m_playing;

  void checkPortAudioError(PaError &error);

  /**
   * pad audio with silence (on left, right, or both)
   * @param seconds seconds of silence to pad with
   * @param padLeft if true, prepend padding
   * @param padright if true, append padding
   */
  void pad(float seconds, bool padLeft, bool padRight);
};

} // namespace aurora

#endif // AUDIOFILE_H
