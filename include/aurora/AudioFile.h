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
  AudioFile(Buffer &b);
  AudioFile(std::string &filename);
  ~AudioFile();

  /// writes the audio data to a file
  void writeToFile(std::string &filename);

  /**
   * Pad adds silence to both the beginning and end of the audio data
   * @param seconds the duration of silence to pad with, in seconds
   */
  void pad(float seconds);

  /// add silence to the beginning of the audio data
  void padLeft(float seconds);

  /// add silence to the end of the audio data
  void padRight(float seconds);

  /// trim silence from both ends of the audio file
  void trimSilence();

  /// stop the audio playback immediately
  void stop();

  /// stop the audio playback immediately
  void play();

  /// returns the wav data contained in the audio file
  // TODO: avoid returning a copy
  Buffer getWAVData();
private:
  WAV m_audioData;
  bool m_playing;
  bool m_shouldStop;
};

} // namespace aurora

#endif // AUDIOFILE_H
