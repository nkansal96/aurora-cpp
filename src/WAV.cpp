#include "aurora/WAV.h"
#include "aurora/errors/AuroraError.h"
#include "sndfile.hh"
#include <cmath>
#include <iostream>
#include "aurora/utils.h"
#include <memory>

namespace aurora {

WAV::WAV()
  : m_numChannels(defaultNumChannels),
    m_sampleRate(defaultSampleRate),
    m_audioFormat(defaultAudioFormat),
    m_bitsPerSample(defaultBitsPerSample) {}

WAV::WAV(Buffer &audioData, int numChannels, int sampleRate, int audioFormat, int bitsPerSample)
  : m_numChannels(numChannels),
    m_sampleRate(sampleRate),
    m_audioFormat(audioFormat),
    m_bitsPerSample(bitsPerSample),
    m_audioData(audioData) {}

WAV::WAV(Buffer &data){
  if (data.size() == 0) {
    throw AuroraError("WavConstructorEmptyBuffer", "Tried to construct WAV object with empty data buffer", "no info");
  }

  VirtualSoundFileUserData userData(data);

  SndfileHandle fileHandle(VirtualSoundFile, &userData);
  if (fileHandle.error() != 0) {
    throw AuroraError("LibsndfileError", "Libsndfile encountered an error in opening a virtual file", fileHandle.strError());
  }

  m_audioData.resize(data.size());
  fileHandle.read(reinterpret_cast<short*>(m_audioData.data()), data.size() / sizeof(short));

  m_numChannels = fileHandle.channels();
  m_sampleRate = fileHandle.samplerate();
  m_bitsPerSample = 16;
  m_audioFormat = fileHandle.format();
}


WAV::~WAV() {}

int WAV::getSampleRate() {
  return m_sampleRate;
}

int WAV::getNumChannels() {
  return m_numChannels;
}

int WAV::getBitsPerSample() {
  return m_bitsPerSample;
}

void WAV::trimSilent(double threshold, double padding) {
  int sampleSize = m_numChannels * (m_bitsPerSample/8);
  int step = 1024;
  double max_amplitude = std::pow(2.0, double(m_bitsPerSample)) / 2.0;
  double silence_threshold = threshold * max_amplitude;

  int n1=0;
  //identify beginning range below threshold
  while (n1 < m_audioData.size()) {
    std::vector<char>::const_iterator first = m_audioData.begin() + n1;
    std::vector<char>::const_iterator last = m_audioData.begin() + n1 + (sampleSize * step);
    std::vector<char> subVec(first, last);
    double sampleRMS = rms(sampleSize, subVec);
    if (sampleRMS > silence_threshold) break;
    n1 += sampleSize * step;
  }
  //identify end range below threshold
  int n2 = m_audioData.size();
  while (n2 >= 0) {
    std::vector<char>::const_iterator first = m_audioData.begin() + n2 - (sampleSize * step);
    std::vector<char>::const_iterator last = m_audioData.begin() + n2;
    std::vector<char> subVec(first, last);
    double sampleRMS = rms(sampleSize, subVec);
    if (sampleRMS > silence_threshold) break;
    n1 -= sampleSize * step;
  }

  //trim, taking padding into account
  int paddingSize = int(padding * m_sampleRate * sampleSize);
  std::vector<char>::const_iterator first = m_audioData.begin() + n1 - paddingSize;
  m_audioData.erase(m_audioData.begin(), first);
  std::vector<char>::const_iterator last = m_audioData.end() - n2 + paddingSize;
  m_audioData.erase(last, m_audioData.end());
}


Buffer& WAV::audioData() {
  return m_audioData;
}

Buffer WAV::data() {
  VirtualSoundFileUserData userData;
  SndfileHandle fileHandle(VirtualSoundFile,
                           &userData,
                           SFM_WRITE,
                           SF_FORMAT_WAV | SF_FORMAT_PCM_16,
                           m_numChannels,
                           m_numChannels);
  if (fileHandle.error() != 0) {
    throw AuroraError("LibsndfileError", "Libsndfile encountered an error in opening a virtual file", fileHandle.strError());
  }

  // 'short' assumes bitsPerSample is 16
  fileHandle.write(reinterpret_cast<short*>(m_audioData.data()), m_audioData.size() / m_bitsPerSample);

  return userData.buffer;
}

} // namespace aurora
