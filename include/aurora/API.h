#ifndef AURORA_API_H
#define AURORA_API_H

#include <string>
#include <unordered_map>

namespace aurora {

// forward declarations
struct HTTPResponse;
class AudioFile;

const std::string INTERPRET_PATH = "/interpret/";
const std::string TTS_PATH = "/tts/";
const std::string STT_PATH = "/stt/";

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
   */
  static std::string getSTT(AudioFile &file);

  /**
   * getTTS calls the TTS API given some text and returns an AudioFile
   * with the audio from converting the text to speech.
   */
  static AudioFile getTTS(const std::string &text);

  /**
   * queries the API with the provided text and returns the interpreted response
   * @param text the string to be interpreted
   */
  static InterpretResponse getInterpret(const std::string &text);

  /// check status code for API errors and throw exception if so
  static void checkStatus(HTTPResponse &res);
};

} // namespace aurora

#endif // AURORA_API_H
