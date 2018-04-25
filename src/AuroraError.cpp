#include "aurora/errors/AuroraError.h"

#include <string>

aurora::AuroraError::AuroraError(std::string code, std::string message, std::string info) {
  m_code = code;
  m_message = message;
  m_info = info;
}

std::string aurora::AuroraError::getCode() const {
  return m_code;
}

std::string aurora::AuroraError::getMessage() const {
  return m_message;
}

std::string aurora::AuroraError::getInfo() const {
  return m_info;
}

const char* aurora::AuroraError::what() const throw() {
  return m_message.c_str();
}
