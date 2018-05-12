#ifndef AURORAERROR_H
#define AURORAERROR_H

#include <string>
#include <exception>

namespace aurora {

/// used for errors that occur in the Aurora client library
class AuroraError : public std::exception {
public:
  AuroraError(const std::string &code, const std::string &message, const std::string &info);

  /// Code is the specific error code (for debugging purposes)
  std::string getCode() const;

  /// Message is a descriptive message of the error, why it occurred, how to resolve, etc.
  std::string getMessage() const;

  /// Info is an optional field describing in detail the error for debugging purposes.
  std::string getInfo() const;

  /// returns a descriptive message of the error, why it occurred, how to resolve, etc.
  virtual const char* what() const noexcept;

private:
  const std::string m_code;

  const std::string m_message;

  const std::string m_info;
};

} // namespace aurora

#endif // AURORAERROR_H
