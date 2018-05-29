#include <unistd.h>
#include "aurora/AudioFile.h"
#include "aurora/errors/AuroraError.h"
#include "portaudio.h"
#include <sndfile.hh>
#include <portaudio.h>
#include <algorithm>

// disabled until portaudio bug fixed, as explained in
// https://github.com/EddieRingle/portaudio/blob/9eb5f0b3d820a81d385504d9c54534abbeea1099/examples/paex_read_write_wire.c#L59
#define CHECK_OVERFLOW  (0)
#define CHECK_UNDERFLOW (0)

// TODO: calibrate constants
const double SILENCE_THRESHOLD = 0.03;
const double SILENCE_PADDING = 0.25;

namespace aurora {

AudioFile::AudioFile(Buffer &b) : m_audioData(b), m_playing(false), m_shouldStop(false) {}

AudioFile::AudioFile(const std::string &filename) : m_playing(false), m_shouldStop(false) {
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

AudioFile::AudioFile(float length, float silenceLen) : m_playing(false), m_shouldStop(false) {
  Buffer recordBuffer = record(length, silenceLen);
  m_audioData = WAV(recordBuffer, defaultNumChannels, defaultSampleRate, defaultAudioFormat, BITS_PER_SAMPLE);
}

AudioFile::AudioFile(const WAV &wav) : m_audioData(wav), m_playing(false), m_shouldStop(false) {}

AudioFile::~AudioFile() {}

void AudioFile::writeToFile(const std::string &filename) {
  SndfileHandle fileHandle(filename, SFM_WRITE, AUDIO_FILE_FORMAT, m_audioData.getNumChannels(), m_audioData.getSampleRate());
  if (fileHandle.error() != 0) {
    throw AuroraError("LibsndfileError", "Libsndfile encountered an error in opening a file", fileHandle.strError());
  }

  fileHandle.write(reinterpret_cast<AudioSampleType*>(m_audioData.audioData().data()),
                   m_audioData.audioData().size() / BYTES_PER_SAMPLE);
}

void AudioFile::pad(float seconds) {
  // left and right
  pad(seconds, true, true);
}

void AudioFile::padLeft(float seconds) {
  pad(seconds, true, false);
}

void AudioFile::padRight(float seconds) {
  pad(seconds, false, true);
}

void AudioFile::trimSilence() {
  m_audioData.trimSilent(SILENCE_THRESHOLD, SILENCE_PADDING);
}

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


  Buffer &audioBuffer = m_audioData.audioData();

  int bytesPerFrame = m_audioData.bytesPerFrame();
  int framesPerChunk = 1024;
  int step = framesPerChunk * bytesPerFrame;
  m_playing = true;
  for (int offset = 0; offset < audioBuffer.size(); offset += step) {
    if (m_shouldStop) {
      m_shouldStop = false;
      break;
    }

    // don't write bytes from past the end of the buffer if chunk is too big
    int numFrames = std::min(((int)audioBuffer.size() - offset)/bytesPerFrame, framesPerChunk);
    err = Pa_WriteStream(stream, audioBuffer.data() + offset, numFrames);
  }

  // terminates audio processing, waiting for all pending audio buffers to complete
  err = Pa_StopStream(stream);
  checkPortAudioError(err);

  // close audio stream, discarding any pending buffers!
  err = Pa_CloseStream(stream);
  checkPortAudioError(err);

  // clean up portaudio resources
  Pa_Terminate();

  m_playing = false;
}

void AudioFile::stop() {
  if (m_playing) {
    m_shouldStop = true;
  }
}

Buffer AudioFile::getWAVData() {
  return m_audioData.data();
}

void AudioFile::checkPortAudioError(PaError &error) {
  if (error != paNoError) {
    // clean up portaudio resources before throwing
    Pa_Terminate();

    m_playing = false;

    throw AuroraError("PortAudioError", "An error occurred while using a PortAudio stream", Pa_GetErrorText(error));
  }
}

void AudioFile::pad(float seconds, bool padLeft, bool padRight) {
  int numBytes = m_audioData.bytesPerFrame() * (int)((float)m_audioData.getSampleRate() * seconds);
  Buffer padding(numBytes, 0);

  if (padLeft)
    m_audioData.prependAudioData(padding);
  if (padRight)
    m_audioData.appendAudioData(padding);
}

} // namespace aurora
