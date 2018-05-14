#include "aurora/Speech.h"
#include "aurora/Text.h"
#include "aurora/API.h"

namespace aurora {

Speech::Speech(const AudioFile &file) : m_file(file) {}

Text Speech::text() {
  return Text(API::getSTT(m_file));
}

AudioFile& Speech::getAudio() {
  return m_file;
}

} // namespace aurora
