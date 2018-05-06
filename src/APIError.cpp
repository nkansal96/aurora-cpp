#include "aurora/errors/APIError.h"
#include <string>

namespace aurora {

APIError::APIError(const std::string &ID,
                   const std::string &status,
                   const std::string &code,
                   const std::string &type,
                   const std::string &message,
                   const std::string &info)
  : m_ID(ID), m_status(status), m_code(code), m_type(type), m_message(message), m_info(info) {}

std::string APIError::getID() const {
  return m_ID;
}

std::string APIError::getStatus() const {
  return m_status;
}

std::string APIError::getCode() const {
  return m_code;
}

std::string APIError::getType() const {
  return m_type;
}

std::string APIError::getMessage() const {
  return m_message;
}

std::string APIError::getInfo() const {
  return m_info;
}

const char* APIError:: what() const noexcept {
  return "Aurora API Error";
}

} // namespace aurora
