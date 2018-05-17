#include "aurora/AudioFile.h"
#include "portaudio.h"
#include <sndfile.hh>
#include <portaudio.h>
#include <iostream>

namespace aurora {

// audio file write defaults
const int AUDIO_FILE_FORMAT = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

const PaSampleFormat PORTAUDIO_SAMPLE_FORMAT = paInt16; // 16 bits
using audioSampleType = short; // short = 2 bytes = 16 bits
const int BITS_PER_SAMPLE = sizeof(audioSampleType)*8; // 1 byte = 8 bits
const int BYTES_PER_SAMPLE = sizeof(audioSampleType);

AudioFile::AudioFile(Buffer &b) : m_audioData(b) {}

AudioFile::AudioFile(const std::string &filename) {
  SndfileHandle fileHandle(filename);
  if (fileHandle.error() != 0) {
    throw AuroraError("LibsndfileError", "Libsndfile encountered an error in opening a file", fileHandle.strError());
  }

  // byte buffer to store raw audio data read from file
  Buffer fileBuffer;

  // temporary buffer for reading in chunks of audio file
  const int CHUNK_SIZE = 128;
  audioSampleType tempBuffer[CHUNK_SIZE];

  sf_count_t numItemsRead = 0;

  // read the WAV file in chunks, copying each chunk into the fileBuffer
  while ((numItemsRead = fileHandle.read(tempBuffer, CHUNK_SIZE/fileHandle.channels()))) {
    // append temp buffer to end of fileBuffer (char array)
    char *charTempBuffer = (char*) tempBuffer;
    unsigned tempBufferSize = numItemsRead * BYTES_PER_SAMPLE;
    fileBuffer.insert(fileBuffer.end(), charTempBuffer, charTempBuffer + tempBufferSize);
  }

  if (fileBuffer.size() == 0) {
    throw AuroraError("AudioFileEmptyFile", "AudioFile tried to read empty or nonexistant file", filename);
  }

  // create WAV object from info & raw audio data
  m_audioData = WAV(fileHandle.channels(), fileHandle.samplerate(), fileHandle.format(), BITS_PER_SAMPLE, fileBuffer);
}

AudioFile::~AudioFile() {}

void AudioFile::writeToFile(const std::string &filename) {
  SndfileHandle fileHandle(filename, SFM_WRITE, AUDIO_FILE_FORMAT, m_audioData.getNumChannels(), m_audioData.getSampleRate());
  if (fileHandle.error() != 0) {
    throw AuroraError("LibsndfileError", "Libsndfile encountered an error in opening a file", fileHandle.strError());
  }

  short *shortAudioBuffer = (short*) m_audioData.audioData().data();
  fileHandle.write(shortAudioBuffer, m_audioData.audioData().size() / BYTES_PER_SAMPLE);
}

void AudioFile::pad(float seconds) {}

void AudioFile::padLeft(float seconds) {}

void AudioFile::padRight(float seconds) {}

void AudioFile::trimSilence() {}

void AudioFile::play() {
  // initialize portaudio
  PaError err = Pa_Initialize();
  checkPortAudioError(err);

  int numChannels = m_audioData.getNumChannels();

  // create & configure portaudio audio stream
  PaStream *stream;
  err = Pa_OpenDefaultStream(&stream,
                             0, // no input channels
                             numChannels,
                             PORTAUDIO_SAMPLE_FORMAT,
                             m_audioData.getSampleRate(),
                             paFramesPerBufferUnspecified,
                             nullptr, // use blocking write instead of callback
                             nullptr); // no userData for blocking write
  checkPortAudioError(err);

  // commences audio processing of stream
  err = Pa_StartStream(stream);
  checkPortAudioError(err);

  // tell portaudio to play audio buffer
  err = Pa_WriteStream(stream, m_audioData.audioData().data(), m_audioData.audioData().size()/(BYTES_PER_SAMPLE * numChannels));
  checkPortAudioError(err);

  // terminates audio processing, waiting for all pending audio buffers to complete
  err = Pa_StopStream(stream);
  checkPortAudioError(err);

  // close audio stream, discarding any pending buffers!
  err = Pa_CloseStream(stream);
  checkPortAudioError(err);

  // clean up portaudio resources
  Pa_Terminate();
}

Buffer AudioFile::getWAVData() {
  return m_audioData.data();
}

void AudioFile::checkPortAudioError(PaError &error) {
  if (error != paNoError) {
    // clean up portaudio resources before throwing
    Pa_Terminate();

    throw AuroraError("PortAudioError", "An error occurred while using a PortAudio stream", Pa_GetErrorText(error));
  }
}

} // namespace aurora
