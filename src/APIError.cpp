#include "aurora/errors/APIError.h"
#include <string>

namespace aurora {

APIError::APIError(std::string ID, std::string status, std::string code, std::string type, std::string message, std::string info) {
  m_ID = ID;
  m_status = status;
  m_code = code;
  m_type = type;
  m_message = message;
  m_info = info;
}

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

} // namespace aurora
