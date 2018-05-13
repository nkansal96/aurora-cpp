#include "aurora/WAV.h"

namespace aurora {

WAV::WAV()
  : m_numChannels(defaultNumChannels),
    m_sampleRate(defaultSampleRate),
    m_audioFormat(defaultAudioFormat),
    m_bitsPerSample(defaultBitsPerSample) {}

WAV::WAV(Buffer &audioData)
  : m_numChannels(defaultNumChannels),
    m_sampleRate(defaultSampleRate),
    m_audioFormat(defaultAudioFormat),
    m_bitsPerSample(defaultBitsPerSample),
    m_audioData(audioData) {}


WAV::WAV(int numChannels, int sampleRate, int audioFormat, int bitsPerSample, Buffer &audioData)
  : m_numChannels(numChannels),
    m_sampleRate(sampleRate),
    m_audioFormat(audioFormat),
    m_bitsPerSample(bitsPerSample),
    m_audioData(audioData) {}

WAV::~WAV() {}

void WAV::setData(Buffer &audioData) {
  m_audioData = audioData;
}

Buffer WAV::getData() {
  return m_audioData;
}

}
