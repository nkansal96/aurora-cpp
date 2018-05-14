#ifndef AURORA_TEXT_H
#define  AURORA_TEXT_H

#include <string>

namespace aurora {

// forward declarations
class Speech;
class Interpret;

/**
 * Text encapsulates some text, whether it is obtained from STT, a user input,
 * or generated programmatically, and allows high-level operations to be
 * conducted and chained on it (like converting to speech, or calling Interpret).
 */
class Text {
public:
  explicit Text(const std::string &text) : m_text(text) {}
  virtual ~Text() {}

  /**
   * Speech calls the Aurora TTS service on the text encapsulated in this object
   * and converts it to a `Speech` object. Further operations can then be done
   * on it, such as saving to file or speaking the resulting audio.
   */
  virtual Speech speech() const;

  /**
   * Interpret calls the Aurora Interpret service on the text encapsulated in this
   * object and converts it to an `Interpret` object, which contains the results
   * from the API call.
   */
  virtual Interpret interpret() const;

  /// returns the encapsulated text string
  virtual std::string getText() const;

  bool operator==(const Text &other) const;

private:
  std::string m_text;
};

/// enable comparison of Text objects with strings
inline bool operator==(const Text &lhs, const std::string &rhs) { return lhs.getText() == rhs; }
inline bool operator==(const std::string &lhs, const Text &rhs) { return lhs == rhs.getText(); }

} // namespace aurora

#endif // AURORA_TEXT_H
