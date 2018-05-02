#ifndef AURORA_API_H
#define AURORA_API_H

#include <string>
#include <unordered_map>

namespace aurora {

struct InterpretResponse {
  /// Text is the original query
  std::string text;
  /// intent of user, empty string if unclear
  std::string intent;
  /**
   * Entities is a map of the entities in the user's query. The keys
   * are the entity name (like song or location) and the value
   * is the detected value for that entitity
   */
  std::unordered_map<std::string, std::string> entities;
};

class API {
public:
  /**
   * getSTT queries the API with the provided audio file and returns
   * a transcript of the speech.
   * TODO: audio file param
   */
  std::string getSTT();

  // TODO: return audio file
  void getTTS(std::string text);
  /**
   * queries the API with the provided text and returns the interpreted response
   * @param text the string to be interpreted
   */
  InterpretResponse getInterpret(std::string text);
private:

};

} // namespace aurora

#endif // AURORA_API_H
