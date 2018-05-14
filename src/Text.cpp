#include "aurora/Text.h"
#include "aurora/Interpret.h"
#include "aurora/API.h"
#include "aurora/Speech.h"

namespace aurora {


Speech Text::speech() const {
  return Speech(API::getTTS(m_text));
}

Interpret Text::interpret() const {
  InterpretResponse res = API::getInterpret(m_text);
  return Interpret(res);
}

std::string Text::getText() const {
  return m_text;
}

bool Text::operator==(const Text &other) const {
  return m_text == other.m_text;
}

} // namespace aurora
