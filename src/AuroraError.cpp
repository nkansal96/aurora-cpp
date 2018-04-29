#include "aurora/errors/AuroraError.h"
#include <string>

namespace aurora {

AuroraError::AuroraError(const std::string &code, const std::string &message, const std::string &info)
  : m_code(code), m_message(message), m_info(info) {}

std::string AuroraError::getCode() const {
  return m_code;
}

std::string AuroraError::getMessage() const {
  return m_message;
}

std::string AuroraError::getInfo() const {
  return m_info;
}

const char* AuroraError::what() const throw() {
  return "Aurora client error";
}

} //namespace aurora
