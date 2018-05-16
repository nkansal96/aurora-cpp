#include "aurora/AudioFile.h"
#include "portaudio.h"
#include <sndfile.hh>
#include <portaudio.h>
#include <iostream>

namespace aurora {

AudioFile::AudioFile(Buffer &b) : m_audioData(b), m_playing(false), m_shouldStop(false) {}

AudioFile::AudioFile(const std::string &filename) {
  SndfileHandle fileHandle(filename);

  Buffer fileBuffer;

  const int CHUNK_SIZE = 32;
  short tempBuffer[CHUNK_SIZE];

  sf_count_t numItemsRead = 0;

  while ((numItemsRead = fileHandle.read(tempBuffer, CHUNK_SIZE))) {
    // append short buffer to end of fileBuffer (char array)
    char *charTempBuffer = (char*) tempBuffer;
    unsigned tempBufferSize = numItemsRead * (sizeof(short)/sizeof(char));
    fileBuffer.insert(fileBuffer.end(), charTempBuffer, charTempBuffer + tempBufferSize);
  }

  // TODO: set correct channels / smaple rate / format / bps
  m_audioData = WAV(fileBuffer);

  m_playing = false;
  m_shouldStop = false;
}

AudioFile::~AudioFile() {}

void AudioFile::writeToFile(const std::string &filename) {}

void AudioFile::pad(float seconds) {}

void AudioFile::padLeft(float seconds) {}

void AudioFile::padRight(float seconds) {}

void AudioFile::trimSilence() {}

void AudioFile::stop() {}

void AudioFile::play() {
  // TODO: handle portaudio errors

  auto error = Pa_Initialize();

  PaStream *stream;
  Pa_OpenDefaultStream(&stream,
                       0,
                       m_audioData.getNumChannels(),
                       paInt16, // shorts?
                       m_audioData.getSampleRate(),
                       paFramesPerBufferUnspecified,
                       nullptr,
                       nullptr);
  Pa_StartStream(stream);

  Pa_WriteStream(stream, m_audioData.audioData().data(), m_audioData.audioData().size() / 2);

  Pa_CloseStream(stream);
  Pa_Terminate();
}

Buffer AudioFile::getWAVData() {
  return m_audioData.data();
}

} // namespace aurora
