#ifndef INTERPRET_H
#define INTERPRET_H

#include <string>
#include <unordered_map>
#include <aurora/API.h>

namespace aurora {

class Interpret {
public:
  Interpret(InterpretResponse &res) : intent(res.intent), entities(res.entities) {}

  /**
   * Intent represents the intent of the user. This can be an empty string if
   * the intent of the user was unclear. Otherwise, it will be one of the
   * pre-determined values listed in the Aurora dashboard.
   */
  const std::string intent;

  /**
   * Entities contain auxiliary information about the user's utterance. This
   * can be an empty map if no such information was detected. Otherwise, it
   * will be a key-value listing according to the entities described on the
   * Aurora dashboard.
   */
  const std::unordered_map<std::string, std::string> entities;
};

} // namespace aurora

#endif // INTERPRET_H
