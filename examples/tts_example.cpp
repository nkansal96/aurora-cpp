#include <iostream>
#include <string>
#include <aurora/Aurora.h>
#include "exampleutils.h"

using namespace aurora;

int main(int argc, const char * argv[]) {
  setAuroraConfiguration();
  for (;;) {
    std::string input;
    std::cout << "tts > ";
    std::getline(std::cin, input);
    try {
      Text text(input);
      text.speech().getAudio().play();
    }
    catch (AuroraError &e) {
      std::cout << "AuroraError: " << e.getCode() << ", " << e.getInfo() << ", " << e.getMessage() << std::endl;
    }
    catch (APIError &e) {
      std::cout << "APIError: " << e.getCode() << ", " << e.getMessage() << std::endl;
    }
  }
}
