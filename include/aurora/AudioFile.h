#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include "aurora/WAV.h"
#include <string>

namespace aurora {

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
  /**
   * Create a new audio file by recording from the default input
   * @param length specifies the max length of the recording in seconds
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

  /// stop the audio playback immediately
  virtual void stop();

  /// stop the audio playback immediately
  virtual void play();

  /// returns the wav data contained in the audio file
  // TODO: avoid returning a copy
  virtual Buffer getWAVData();
private:
  WAV m_audioData;
  bool m_playing;
  bool m_shouldStop;
};

} // namespace aurora

#endif // AUDIOFILE_H
