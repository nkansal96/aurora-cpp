#ifndef APIERROR_H
#define APIERROR_H

#include <string>
#include <exception>

namespace aurora {

class APIError : public std::exception {
  public:
  APIError(const std::string &ID,
           const std::string &status,
           const std::string &code,
           const std::string &type,
           const std::string &message,
           const std::string &info);

    /// Id is the request ID for which this error occurred.
    std::string getID() const;

    /// Status is the HTTP Status code for this error
    std::string getStatus() const;

    /// Code is the specific error code (for debugging purposes)
    std::string getCode() const;

    /// Type is the type (BadRequest, NotFound, etc) of error
    std::string getType() const;

    /// Message is a descriptive message of the error, why it occurred, how to resolve, etc.
    std::string getMessage() const;

    /// Info is an optional field describing in detail the error for debugging purposes.
    std::string getInfo() const;

    /// returns a descriptive message of the error, why it occurred, how to resolve, etc.
    virtual const char* what() const noexcept;

  private:
    std::string m_ID;
    std::string m_status;
    std::string m_code;
    std::string m_type;
    std::string m_message;
    std::string m_info;
};

} // namespace aurora

#endif // APIERROR_H
