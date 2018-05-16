#include "aurora/WAV.h"
#include "sndfile.hh"
#include <cmath>
#include "aurora/utils.h"

//implement these: https://github.com/auroraapi/aurora-go/blob/master/audio/wav.go

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

/*
WAV::WAV(char* filename){
  SndfileHandle file = SndfileHandle(filename);
  m_sampleRate = file.sampleRate();
  m_numChannels = file.channels();
  m_audioFormat = file.format();
  //determine bits per sample using audio format info 
  switch(SF_FORMAT_SUBMASK &m_audioFormat):
    case 0x0001:
      m_bitsPerSample = 8;
      break;
    case 0x0002:
      m_bitsPerSample = 16;
      break;
    case 0x0003: 
      m_bitsPerSample = 24;
      break;
    case 0x0004:
      m_bitsPerSample = 32;
      break;

    default: m_bitsPerSample = 0; // until I can figure out what to do with this
  
  int file_len = file.frames() * file.channels() * (m_bitsPerSample/8); 
  short* buffer = new short[file_len];
  memset(buffer, 0, sizeof(buffer));
  file.write(buffer, file_len);

}
*/

WAV::WAV(int numChannels, int sampleRate, int audioFormat, int bitsPerSample, Buffer &audioData)
  : m_numChannels(numChannels),
    m_sampleRate(sampleRate),
    m_audioFormat(audioFormat),
    m_bitsPerSample(bitsPerSample),
    m_audioData(audioData) {}

WAV::~WAV() {}

int WAV::getSampleRate() {
  return m_sampleRate;
}

int WAV::getNumChannels() {
  return m_numChannels;
}

void WAV::trimSilent(double threshold, double padding){
  int sampleSize = m_numChannels * (m_bitsPerSample/8); 
  int step = 1024;
  double max_amplitude = std::pow(2.0, double(m_bitsPerSample)) / 2.0;
  double silence_threshold = threshold * max_amplitude; 

  int n1=0; 
  //identify beginning range below threshold
  while(n1 < m_audioData.size()){
    std::vector<char>::const_iterator first = m_audioData.begin() + n1;
    std::vector<char>::const_iterator last = m_audioData.begin() + n1 + (sampleSize * step);
    std::vector<char> subVec(first, last);
    double sampleRMS = rms(sampleSize, subVec);
    if (sampleRMS > silence_threshold) break;
    n1 += sampleSize * step;
  }
  //identify end range below threshold
  int n2 = m_audioData.size();
  while(n2 >= 0){
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
  std::vector<char>::const_iterator last = m_audioData.end() - n2 + paddingSize;
  m_audioData.erase(m_audioData.begin(), first);
  m_audioData.erase(last, m_audioData.end());
}


Buffer WAV::data() {
  return {m_audioData};
}

}
