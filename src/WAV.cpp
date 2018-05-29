#include "aurora/WAV.h"
#include "aurora/errors/AuroraError.h"
#include "sndfile.hh"
#include <cmath>
#include "aurora/utils.h"
#include "aurora/AudioFile.h"
#include <memory>
#include <iostream>

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

  SndfileHandle fileHandle(VirtualSoundFile, &userData, SFM_READ);
  if (fileHandle.error() != 0) {
    throw AuroraError("LibsndfileError", "Libsndfile encountered an error in opening a virtual file", fileHandle.strError());
  }

  int numBytes = fileHandle.channels() * fileHandle.frames() * sizeof(short);
  m_audioData.resize(numBytes);
  fileHandle.read(reinterpret_cast<short*>(m_audioData.data()), fileHandle.channels() * fileHandle.frames());

  m_numChannels = fileHandle.channels();
  m_sampleRate = fileHandle.samplerate();
  m_bitsPerSample = sizeof(short) * BITS_PER_BYTE;
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

int WAV::bytesPerFrame() {
  int bytesPerSample = m_bitsPerSample / 8;
  return m_numChannels * bytesPerSample;
}

void WAV::trimSilent(double threshold, double padding) {
  int sampleSize = m_numChannels * (m_bitsPerSample/8);
  int step = 10;
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
    n2 -= sampleSize * step;
  }

  int beginning_of_end_silence = m_audioData.size() - n2;

  //trim, taking padding into account
  int paddingSize = int(padding * m_sampleRate * sampleSize);

  std::vector<char>::const_iterator first = m_audioData.begin() + (n1 - paddingSize);
  //if entire vector is silent
  if(first == m_audioData.end()) {
    m_audioData.erase(m_audioData.begin(), m_audioData.end());
    return;
  }
  else{
    m_audioData.erase(m_audioData.begin(), first);
  }

  std::vector<char>::const_iterator last = m_audioData.end() - (beginning_of_end_silence + paddingSize);

  //if entire vector is silent
  if(last == m_audioData.begin()) {
    m_audioData.erase(m_audioData.begin(), m_audioData.end());
    return;
  }
  else{
    m_audioData.erase(last, m_audioData.end());
  }

}

void WAV::appendAudioData(Buffer &b) {
  m_audioData.insert(m_audioData.end(), b.begin(), b.end());
}

void WAV::prependAudioData(Buffer &b) {
  m_audioData.insert(m_audioData.begin(), b.begin(), b.end());
}

Buffer& WAV::audioData() {
  return m_audioData;
}

Buffer WAV::data() const {
  VirtualSoundFileUserData userData;
  SndfileHandle *fileHandle = new SndfileHandle(VirtualSoundFile,
                                                &userData,
                                                SFM_WRITE,
                                                SF_FORMAT_WAV | SF_FORMAT_PCM_16,
                                                m_numChannels,
                                                m_sampleRate);
  if (fileHandle->error() != 0) {
    throw AuroraError("LibsndfileError", "Libsndfile encountered an error in opening a virtual file", fileHandle->strError());
  }

  int bytesPerSample = m_bitsPerSample / BITS_PER_BYTE;

  // TODO: 'short' assumes bitsPerSample is 16, make more flexible
  int numItems = m_audioData.size() / bytesPerSample;

  fileHandle->write(reinterpret_cast<const short*>(m_audioData.data()), numItems);

  // need to call SndFileHandle destructor to close file
  delete fileHandle;

  return userData.buffer;
}

} // namespace aurora
