#ifndef AURORAERROR_H
#define AURORAERROR_H

#include <string>
#include <exception>

namespace aurora {

/// used for errors that occur in the Aurora client library
class AuroraError : public std::exception {
public:
AuroraError(std::string code, std::string message, std::string info);

std::string getCode() const;
std::string getMessage() const;
std::string getInfo() const;

const char* what () const throw();

private:
/// Code is the specific error code (for debugging purposes)
std::string m_code;

/// Message is a descriptive message of the error, why it occurred, how to resolve, etc.
std::string m_message;

/// Info is an optional field describing in detail the error for debugging purposes.
std::string m_info;
};

}

#endif // AURORAERROR_H
