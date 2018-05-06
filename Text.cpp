#include "aurora/Text.h"
#include "aurora/Interpret.h"
#include "aurora/API.h"

namespace aurora {

/*
Speech Text::speech() const {
  // TODO: call speech api
}
*/

Interpret Text::interpret() const {
  InterpretResponse res = API::getInterpret(m_text);
  return Interpret(res);
}

std::string Text::getText() const {
  return m_text;
}

} // namespace aurora