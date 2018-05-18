#include "aurora/WAV.h"
#include "sndfile.hh"
#include <cmath>
#include <iostream>
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


Buffer& WAV::audioData() {
  return {m_audioData};
}

Buffer& WAV::wav_header(int dataLen){
  int headerLen = 44;
  uint32_t chunkSize = (dataLen + headerLen - 8);
  //std::cout << "Chunk size is: " << (chunkSize & 0xff) << std::endl;
  // first create the header, then append the rest of the file
  Buffer *header = new Buffer(headerLen);

  // RIFF header
  (*header)[0] = 'R';
  (*header)[1] = 'I';
  (*header)[2] = 'F';
  (*header)[3] = 'F';

  //Chunk size, little-endian in 4 bytes
  // (*header)[4] = chunkSize & 0xff;
  // (*header)[5] = (chunkSize >> 8 ) & 0xff;
  // (*header)[6] = (chunkSize >> 16) & 0xff;
  // (*header)[7] = (chunkSize >> 24) & 0xff; 
  write_uint_to_littleendian(chunkSize, &((*header)[4]));

  // Format (WAVE)
  (*header)[8] = 'W';
  (*header)[9] = 'A';
  (*header)[10] = 'V';
  (*header)[11] = 'E';
  // Metadata subchunk ID ("fmt ")
  (*header)[12] = 'f';
  (*header)[13] = 'm';
  (*header)[14] = 't';
  (*header)[15] = ' ';
  //Metadata subchunk size (16)
  write_uint_to_littleendian(uint32_t(16), &((*header)[16]));
  //Audio format (PCM = 1)
  write_uint_to_littleendian(uint16_t(m_audioFormat), &((*header)[20]));
  // Num Channels (Mono = 1)
  write_uint_to_littleendian(uint16_t(m_numChannels), &((*header)[22]));
  // Sample Rate (16000 Hz)
  write_uint_to_littleendian(uint32_t(m_sampleRate), &((*header)[24]));

  // Byte Rate = SampleRate * NumChannels * BitsPerSample/8 = 32000
  uint32_t byteRate = m_sampleRate * uint32_t(m_numChannels) * uint32_t(m_bitsPerSample) / 8;
  write_uint_to_littleendian(byteRate, &((*header)[28]));

  // Block Align = NumChannels * BitsPerSample/8
  uint16_t blockAlign = m_numChannels * m_bitsPerSample / 8;
  write_uint_to_littleendian(blockAlign,  &((*header)[32]));

  // Bits per sample = 16
  write_uint_to_littleendian(uint16_t(m_bitsPerSample), &((*header)[34]));

  // Data subchunk ID ("data")
  (*header)[36] = 'd';
  (*header)[37] = 'a';
  (*header)[38] = 't';
  (*header)[39] = 'a';

  // Data length
  write_uint_to_littleendian(uint32_t(dataLen), &((*header)[34]));

  return *header;
}

Buffer WAV::data() {
  // find first data index
  int dataLen = m_audioData.size();
  Buffer wav = wav_header(dataLen);
  wav.insert(wav.end(), m_audioData.begin(), m_audioData.end());


  return wav;
}

}
