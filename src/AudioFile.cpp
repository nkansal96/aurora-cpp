#include "aurora/AudioFile.h"
#include "portaudio.h"

namespace aurora {

AudioFile::AudioFile(Buffer &b) : m_audioData(b), m_playing(false), m_shouldStop(false) {}

AudioFile::~AudioFile() {}

void AudioFile::writeToFile(std::string &filename) {}

void AudioFile::pad(float seconds) {}

void AudioFile::padLeft(float seconds) {}

void AudioFile::padRight(float seconds) {}

void AudioFile::trimSilence() {}

void AudioFile::stop() {}

void AudioFile::play() {

}

Buffer AudioFile::getWAVData() {
  return m_audioData.data();
}

} // namespace aurora
