#include "aurora/AudioFile.h"
#include "portaudiocpp/PortAudioCpp.hxx"

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
  portaudio::AutoSystem autoSys;
  portaudio::System &sys = portaudio::System::instance();

  portaudio::DirectionSpecificStreamParameters inParams = portaudio::DirectionSpecificStreamParameters::null();
  portaudio::DirectionSpecificStreamParameters outParams(sys.defaultOutputDevice(),
                                                         m_audioData.getNumChannels(),
                                                         portaudio::INT32,
                                                         true,
                                                         sys.defaultOutputDevice().defaultLowInputLatency(),
                                                         NULL);
  portaudio::StreamParameters params(inParams,
                                     outParams,
                                     m_audioData.getSampleRate(),
                                     getWAVData().size(),
                                     paNoFlag);

  portaudio::BlockingStream stream(params);

  stream.start();

  //stream.write(getWAVData(), getWAVData().size());
}

Buffer AudioFile::getWAVData() {
  return m_audioData.getData();
}

} // namespace aurora
