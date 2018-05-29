#include "aurora/utils.h"
#include "aurora/Speech.h"
#include "aurora/Text.h"
#include "aurora/API.h"
#include "aurora/AudioFile.h"

namespace aurora {

Speech::Speech(const AudioFile &file) : m_file(file) {}

Text Speech::text() {
  return Text(API::getSTT(m_file));
}

AudioFile& Speech::getAudio() {
  return m_file;
}

Speech listen(const ListenParams &params) {
  return Speech(AudioFile(params.length, params.silenceLen));
}

void continuouslyListen(const ListenParams &params, SpeechHandleFunc callback) {
  for (;;) {
    Speech sp = listen(params);
    if (!callback(sp)) {
      break;
    }
  }
}

Text listenAndTranscribe(const ListenParams &params) {
  Buffer recordBuffer = record(params.length, params.silenceLen);
  return Text(API::getSTTFromBuffer(recordBuffer));
}

void continuouslyListenAndTranscribe(const ListenParams &params, TextHandleFunc callback) {
  for (;;) {
    Text tx = listenAndTranscribe(params);
    if (!callback(tx)) {
      break;
    }
  }
}

} // namespace aurora
