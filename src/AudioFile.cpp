#include "aurora/AudioFile.h"
#include "aurora/errors/AuroraError.h"
#include "portaudio.h"
#include <sndfile.hh>
#include <portaudio.h>
#include <algorithm>

namespace aurora {

// audio file write defaults
const int AUDIO_FILE_FORMAT = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

const PaSampleFormat PORTAUDIO_SAMPLE_FORMAT = paInt16; // 16 bits
using AudioSampleType = short; // short = 2 bytes = 16 bits
const int BITS_PER_SAMPLE = sizeof(AudioSampleType)*8; // 1 byte = 8 bits
const int BYTES_PER_SAMPLE = sizeof(AudioSampleType);



// TODO: move to Utils file
/**
 * Record records audio from the default input device
 * @param length the duration of audio to record, in seconds
 * @param silenceLen if length is 0, the amount of silence in seconds to allow before
 * ending recording
 * @returns a Buffer containing the recorded raw audio data
 */
Buffer record(float length, float silenceLen) {
  // TODO: check errors

  int totalSamplesToRecord = length * (float)defaultSampleRate;
  const int CHUNK_SIZE = 512; // max number of samples to read at once

  // buffer for storing recorded audio, big enough to hold all samples
  Buffer outBuffer(totalSamplesToRecord * BYTES_PER_SAMPLE);

  // initialize portaudio
  PaError err = Pa_Initialize();

  // create & configure portaudio audio stream
  PaStream *stream;
  err = Pa_OpenDefaultStream(&stream,
                             1, // mono input channel
                             0, // no output channels
                             PORTAUDIO_SAMPLE_FORMAT,
                             defaultSampleRate,
                             paFramesPerBufferUnspecified,
                             nullptr, // use blocking read instead of callback
                             nullptr); // no userData for blocking read
  if (err != paNoError) {
    // clean up portaudio resources before throwing
    Pa_Terminate();

    throw AuroraError("PortAudioError", "An error occurred while using a PortAudio stream", Pa_GetErrorText(err));
  }

  // commences audio processing of stream
  err = Pa_StartStream(stream);

  // record audio in chunks
  int progress = 0; // samples recorded so far
  while (progress < totalSamplesToRecord) {
    AudioSampleType *buffPtr = reinterpret_cast<AudioSampleType*>(outBuffer.data()) + progress;
    int samplesRead = std::min(CHUNK_SIZE, totalSamplesToRecord - progress);
    err = Pa_ReadStream(stream, buffPtr, samplesRead);

    // TODO: check for silence

    progress += samplesRead;
  }

  // terminates audio processing, waiting for all pending audio buffers to complete
  err = Pa_StopStream(stream);

  // close audio stream, discarding any pending buffers!
  err = Pa_CloseStream(stream);

  // clean up portaudio resources
  Pa_Terminate();

  // TODO: remove silence at beginning

  // remove pop at beginning
  // std::fill(outBuffer.begin(), outBuffer.begin() + 2000, 0);

  return outBuffer;
}

AudioFile::AudioFile(Buffer &b) : m_audioData(b) {}

AudioFile::AudioFile(const std::string &filename) {
  SndfileHandle fileHandle(filename);
  if (fileHandle.error() != 0) {
    throw AuroraError("LibsndfileError", "Libsndfile encountered an error in opening a file", fileHandle.strError());
  }

  // byte buffer to store raw audio data read from file
  Buffer fileBuffer;

  // temporary buffer for reading in chunks of audio file
  const int CHUNK_SIZE = 512;
  AudioSampleType tempBuffer[CHUNK_SIZE];

  sf_count_t numItemsRead = 0;

  // read the WAV file in chunks, copying each chunk into the fileBuffer
  while ((numItemsRead = fileHandle.read(tempBuffer, CHUNK_SIZE/fileHandle.channels()))) {
    // append temp buffer to end of fileBuffer (char array)
    char *charTempBuffer = reinterpret_cast<char*>(tempBuffer);
    unsigned tempBufferSize = numItemsRead * BYTES_PER_SAMPLE;
    fileBuffer.insert(fileBuffer.end(), charTempBuffer, charTempBuffer + tempBufferSize);
  }

  if (fileBuffer.size() == 0) {
    throw AuroraError("AudioFileEmptyFile", "AudioFile tried to read empty or nonexistant file", filename);
  }

  // create WAV object from info & raw audio data
  m_audioData = WAV(fileBuffer, fileHandle.channels(), fileHandle.samplerate(), fileHandle.format(), BITS_PER_SAMPLE);
}

AudioFile::AudioFile(float length, float silenceLen) {
  Buffer recordBuffer = record(length, silenceLen);
  m_audioData = WAV(recordBuffer, defaultNumChannels, defaultSampleRate, defaultAudioFormat, BITS_PER_SAMPLE);
}

AudioFile::~AudioFile() {}

void AudioFile::writeToFile(const std::string &filename) {
  SndfileHandle fileHandle(filename, SFM_WRITE, AUDIO_FILE_FORMAT, m_audioData.getNumChannels(), m_audioData.getSampleRate());
  if (fileHandle.error() != 0) {
    throw AuroraError("LibsndfileError", "Libsndfile encountered an error in opening a file", fileHandle.strError());
  }

  fileHandle.write(reinterpret_cast<AudioSampleType*>(m_audioData.audioData().data()),
                   m_audioData.audioData().size() / BYTES_PER_SAMPLE);
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
