#ifndef SPEECH_H
#define SPEECH_H

#include <string>
#include <functional>
#include "aurora/AudioFile.h"

namespace aurora {

// forward declarations
class Text;
class Speech;

/// ListenDefaultLength is the default length of time (in seconds) to listen for.
const float listenDefaultLength = 0.0;
/**
 * ListenDefaultSilenceLen is the default amount of silence (in seconds)
 * that the recording framework will allow before stoping.
 */
const float listenDefaultSilenceLen = 0.5;

/**
 * ListenParams configures how the recording framework should listen for
 * speech.
 */
struct ListenParams {
  /// construct with defaults
  ListenParams() : length(listenDefaultLength), silenceLen(listenDefaultSilenceLen) {}
  /**
   * Length specifies how long to listen for in seconds. A value of 0
   * means that the recording framework will continue to listen until
   * the specified amount of silence. A value greater than 0 will
   * override any value set to `SilenceLen`
   */
  float length;
  /**
   * SilenceLen is how long of silence (in seconds) will be allowed before
   * automatically stopping. This value is only taken into consideration if
   * `Length` is 0
   */
  float silenceLen;
};

/**
 * SpeechHandleFunc is the type of function that is passed to `ContinuouslyListen`.
 * It is called every time a speech utterance is decoded and passed to the
 * function. The function must  return a boolean that indicates whether or not to
 * continue listening (true to continue listening, false to stop listening).
 */
using SpeechHandleFunc = std::function<bool(Speech&)>;

/**
 * TextHandleFunc is the type of function that is passed to
 * `ContinuouslyListenAndTranscribe`. It is called every time a speech utterance
 * is decoded and converted to text. The resulting text object is passed to the
 * function. The function must return a boolean that indicates whether or not to
 * continue listening (true to continue listening, false to stop listening).
 */
using TextHandleFunc = std::function<bool(Text&)>;

/**
 * Speech represents a user's utterance. It has high-level methods that let
 * you operate on the speech (like convert it to text) and also allows you
 * to access the underlying audio data to manipulate it, save it, play it, etc.
 */
class Speech {
public:

  /// construct a new Speech object from the given audio file
  explicit Speech(const AudioFile &file);

  /**
   * Text calls the Aurora STT API and converts a user's utterance into
   * a text transcription. This is populated into a `Text` object, allowing you
   * to chain and combine high-level abstractions.
   */
  virtual Text text();

  /// returns a reference to the underlying audio file object
  virtual AudioFile& getAudio();
private:
  /**
   * m_file is the underlying audio that this struct wraps. You can create
   * a speech object and set this directly if you want to operate on some
   * pre-recorded audio
   */
  AudioFile m_file;
};

// listening functions

/**
 * `Listen` takes in `ListenParams` and generates a speech object based on those
 * parameters by recording from the default input device.
 * Currently, this function uses the default audio input interface (an option
 * to change this will be available at a later time).
 */
Speech listen(const ListenParams &params);

/**
 * ContinuouslyListen calls `Listen` continuously.
 *
 * Note that the `ListenParams` is expected to contain values for every field,
 * including defaults for fields that you did not want to change. To avoid having
 * to do this, you should call `NewListenParams` to obtain an instance of
 * `ListenParams` with all of the default filled out, and then override them with
 * the ones you want to change. Alternatively, you can pass `nil` to simply use the
 * default parameters.
 *
 * This function accepts another function as an argument that is called each time
 * a speech utterance is decoded. See the documentation for `SpeechHandleFunc`
 * for more information.
 */
void continuouslyListen(const ListenParams &params, SpeechHandleFunc callback);

/**
 * ListenAndTranscribe starts listening with the given parameters, except instead
 * of waiting for the audio to finish capturing and returning a Speech object,
 * it directly streams it to the API, transcribing it in real-time. When the
 * transcription completes, this function directly returns a Text object. This
 * reduces latency by a significant amount if you already know you want to
 * transcribe the audio.
 */
Text listenAndTranscribe(const ListenParams &params);

/**
 * ContinuouslyListenAndTranscribe is a combination of `ContinuouslyListen`
 * and `ListenAndTranscribe`. See the documentation for those two functions to
 * understand how it works. The difference is that this handler function receives
 * objects of type *Text instead of *Speech. See the documentation for `TextHandleFunc`
 * for more information on that.
 */
void continuouslyListenAndTranscribe(const ListenParams &params, TextHandleFunc callback);

} // namespace aurora

#endif // SPEECH_H
