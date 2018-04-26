#include <string>

namespace aurora {

/// APIError is the error that is returned from API calls.
struct APIError {
  /// Id is the request ID for which this error occurred.
  std::string ID;

  /// Status is the HTTP Status code for this error
  std::string status;

  /// Type is the type (BadRequest, NotFound, etc) of error
  std::string type;

  /// Message is a descriptive message of the error, why it occurred, how to resolve, etc.
  std::string message;

  /// Info is an optional field describing in detail the error for debugging purposes.
  std::string info;
};

} // namespace aurora
