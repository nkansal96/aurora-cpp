# Aurora C++ SDK


## Overview

Aurora is the enterprise end-to-end speech solution. This C++ SDK will allow you to quickly and easily use the Aurora service to integrate voice capabilities into your application.

The SDK is currently in development. Bugs and limited functionality should be expected.

## Installation

**The required C++ version is C++11**

This repository creates an `aurora` CMake library target that can be linked to your executables. This will download dependencies and add aurora's header search paths to your target.

```cmake
add_subdirectory(lib/aurora)
add_executable(example_program
  example_program.cpp)
target_link_libraries(example_program
  aurora)
```

Alternatively, you can build the aurora static library with `cmake .` and `make` and the static library will be generated in `lib/libaurora.a`. You can link this library to your project or move it in your system's library directory.

## Basic Usage

First, make sure you have an account with [Aurora](http://dashboard.auroraapi.com) and have created an Application.

### Text to Speech (TTS)

```cpp
#include <aurora/Aurora.h>

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  // Create a Text object and query the TTS service
  aurora::Speech speech = aurora::Text("Hello World").speech();
  
  // Play the resulting audio...
  speech.getAudio().play();
  
  // ... or save it into a file
  speech.getAudio().writeToFile("test.wav");
}
```

### Speech to Text (STT)

#### Convert a WAV file to Speech

```cpp
#include <aurora/Aurora.h>
#include <iostream>

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  // Load a WAV file
  aurora::AudioFile audio("test.wav");
  
  aurora::Speech speech(audio);
  Text text = speech.text();
  
  std::cout << "Transcribed: " << text.getText() << std::endl;
}
```

#### Convert a previous Text API call to Speech
```cpp
#include <aurora/Aurora.h>
#include <iostream>

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  // Create a Text object and query the TTS service
  aurora::Speech speech = aurora::Text("Hello World").speech();
  
  // Convert the generated speech back to text
  aurora::Text text = speech.text();
  
  std::cout << text.getText() << std::endl; // "hello world"
}
```

#### Listen for a specified amount of time
```cpp
#include <aurora/Aurora.h>
#include <iostream>

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  // create default listen parameters
  aurora::ListenParams params;
  params.length = 3.0;
  
  // Listen for 3 seconds
  aurora::Speech speech = aurora::listen(params);
  
  // Convert the recorded speech to text
  aurora::Text text = speech.text();
  
  std::cout << text.getText() << std::endl;
}
```

#### Listen for an unspecified amount of time

Calling this API will start listening and will automatically stop listening after a certain amount of silence (default is 0.5 seconds).
```cpp
#include <aurora/Aurora.h>
#include <iostream>

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  // create default listen parameters
  aurora::ListenParams params;
  params.silenceLen = 1.0;

  // Listen until 1 second of silence
  aurora::Speech speech = aurora::listen(params);

  // Convert the recorded speech to text
  aurora::Text text = speech.text();
  
  std::cout << text.getText() << std::endl;
}
```

#### Continuously listen

Continuously listen and retrieve speech segments. Note: you can do anything with these speech segments, but here we'll convert them to text. Just like the previous example, these segments are demarcated by silence (0.5 seconds by default) and can be changed by setting the `SilenceLen` parameter. Additionally, you can make these segments fixed length (as in the example before the previous) by setting the `Length` parameter.

```cpp
#include <aurora/Aurora.h>
#include <iostream>

bool listenCallback(aurora::Speech &speech) {
  // convert detected speech to text
  aurora::Text text = speech.text();

  std::cout << text.getText() << std::endl;

  // continue listening
  return true;
}

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  // create default listen parameters
  aurora::ListenParams params;
  params.silenceLen = 1.0;

  // Listen until 1 second of silence
  aurora::continuouslyListen(params, listenCallback);

  // Reduce the amount of silence in between speech segments
  params.silenceLen = 0.5;
  aurora::continuouslyListen(params, listenCallback);

  // Fixed-length speech segments of 3 seconds (overrides silenceLen parameter)
  params.length = 3.0;
  aurora::continuouslyListen(params, listenCallback);

  // lambda expression callback
  aurora::continuouslyListen(params, [](aurora::Speech &speech) {
    // convert detected speech to text
    aurora::Text text = speech.text();

    std::cout << text.getText() << std::endl;

    // continue listening
    return true;
  });
}
```

#### Listen and Transcribe

If you already know that you wanted the recorded speech to be converted to text, you can do it in one step, reducing the amount of code you need to write and also reducing latency. Using the `ListenAndTranscribe` method, the audio that is recorded automatically starts uploading as soon as you call the method and transcription begins. When the audio recording ends, you get back the final transcription.

```cpp
#include <aurora/Aurora.h>
#include <iostream>

bool listenCallback(aurora::Text &text) {
  std::cout << text.getText() << std::endl;

  // continue listening
  return true;
}

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  // create default listen parameters
  aurora::ListenParams params;
  
  // Listen and transcribe once
  aurora::Text text = aurora::listenAndTranscribe(params);
  listenCallback(text);

  // Continuously listen. while recording, this method also streams the data
  // to the backend. Once recording is finished, a transcript is almost
  // instantly available. The callback here receives an aurora::Text (as opposed
  // to the aurora::Speech object in regular continuouslyListen).
  aurora::continuouslyListenAndTranscribe(params, listenCallback);

  // lambda expression callback
  aurora::continuouslyListenAndTranscribe(params, [](aurora::Text &text) {
    std::cout << text.getText() << std::endl;

    // continue listening
    return true;
  });
}
```

#### Listen and echo example

```cpp
#include <aurora/Aurora.h>

bool listenCallback(aurora::Text &text) {
  // Perform TTS on the transcribed text
  aurora::Speech speech = text.speech();

  // Speak
  speech.getAudio().play();

  // continue listening
  return true;
}

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  aurora::ListenParams params;
  aurora::continuouslyListenAndTranscribe(params, listenCallback);
}
```

### Interpret (Language Understanding)

The interpret service allows you to take any Aurora `Text` object and understand the user's intent and extract additional query information. Interpret can only be called on `Text` objects and return `Interpret` objects after completion. To convert a user's speech into and `Interpret` object, it must be converted to text first.

#### Basic example

```cpp
#include <aurora/Aurora.h>
#include <iostream>

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  // Create a Text object
  aurora::Text text("What is the time in Los Angeles?");

  // Call the interpret service
  aurora::Interpret i = text.interpret();


  std::cout << "Intent: " << i.intent << std::endl;

  // iterate through map, printing entity pairs
  std::cout << "Entities: " << std::endl;
  for (auto &entity : i.entities) {
    std::cout << entity.first << ": " << entity.second << std::endl;
  }

  // This should print:
  // Intent: time
  // Entities:
  // location: los angeles
}
```

#### User query example

```cpp
#include <aurora/Aurora.h>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  // Read line-by-line from stdin
  for (;;) {
    std::string t;
    std::cin >> t;

    // Interpret what the user types
    aurora::Interpret i = aurora::Text(t).interpret();

    // print out intent and entities
    std::cout << "Intent: " << i.intent << std::endl;
    std::cout << "Entities: " << std::endl;
    for (auto &entity : i.entities) {
      std::cout << entity.first << ": " << entity.second << std::endl;
    }
  }
}
```

#### Smart Lamp

This example shows how easy it is to voice-enable a smart lamp. It responds to queries in the form of "turn on the lights" or "turn off the lamp". You define what `object` you're listening for (so that you can ignore queries like "turn on the music").

```cpp
#include <aurora/Aurora.h>
#include <iostream>
#include <string>
#include <vector>

bool listenCallback(aurora::Text &text) {
  aurora::Interpret i = text.interpret();

  std::string intent = i.intent;
  std::string object = i.entities["object"];
  std::vector<std::string> validWords = {"light", "lights", "lamp"};

  for (std::string &word : validWords) {
    if (object == word) {
      if (intent == "turn_on") {
        // turn on the lamp
      }
      else if (intent == "turn_off") {
        // turn off the lamp
      }

      break;
    }
  }

  // continue listening
  return true;
}

int main(int argc, char **argv) {
  aurora::config.appID = "YOUR_APP_ID";
  aurora::config.appToken = "YOUR_APP_TOKEN";
  aurora::config.deviceID = "YOUR_DEVICE_ID";
  
  aurora::ListenParams params;
  aurora::continuouslyListenAndTranscribe(params, listenCallback);
}
```

## Example Trivia Game Project
In addition to small example programs found in the [examples/](https://github.com/nkansal96/aurora-cpp/tree/master/examples) directory, we developed a [proof of concept voice-controlled trivia game](https://github.com/nkansal96/aurora-trivia-game) that shows how to use this library.

## Development
Aurora uses cmake to build the Aurora library, build and run tests, and manage dependencies. To configure the project and download dependencies, run `cmake .` in the root directory of the project.

### Generating Documentation
Aurora uses Doxygen to generate documentation. To generate documentation, you must have Doxygen installed. Then run `make doc`. Documentation will be generated in the `/docs` directory.

### Testing
Aurora uses googletest to create and run tests. After building all tests with `make`, you can run all tests with `make test`.

### Static Analysis Checking
To prevent errors and improve performance, Aurora uses cppcheck to analyze all code for deficiencies. CPPCheck is automatically invoked on every build with `make`, and it can be explicityl invoked with `make cppcheck`. CMake handles downloading and building cppcheck during the configuration stage.
