#include <iostream>
#include <string>
#include <aurora/Aurora.h>
#include "exampleutils.h"

using namespace aurora;

int main(int argc, const char * argv[]) {
  setAuroraConfiguration();
  try {
    AudioFile file(assetsPath + "hw.wav");

    file.play();

    Speech speech(file);
    Text text = speech.text();

    std::cout << "text: " << text.getText() << std::endl;
  }
  catch (AuroraError &e) {
    std::cout << "AuroraError: " << e.getCode() << ", " << e.getInfo() << ", " << e.getMessage() << std::endl;
  }
  catch (APIError &e) {
    std::cout << "APIError: " << e.getCode() << ", " << e.getMessage() << std::endl;
  }
}
