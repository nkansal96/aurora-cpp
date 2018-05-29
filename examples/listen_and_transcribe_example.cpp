#include <stdlib.h>
#include <iostream>
#include <string>
#include <aurora/Aurora.h>
#include "exampleutils.h"

using namespace aurora;

int main(int argc, const char * argv[]) {
  setAuroraConfiguration();
  ListenParams params = ListenParams();
  params.length = 4.0;
  params.silenceLen = 0.0;
  
  try {
    std::cout << "[RECORDING]" << std::endl;
    Text text = listenAndTranscribe(params);
    //AudioFile file(4.0, 0.0);
    std::cout << "[END RECORDING]" << std::endl;

    //std::cout << "Sending to STT API..." << std::endl;
    //Text text = speech.text();

    std::cout << "[TRANSCRIPT]" << std::endl << text.getText() << std::endl;
  }
  catch (AuroraError &e) {
    std::cout << "AuroraError: " << e.getCode() << ", " << e.getInfo() << ", " << e.getMessage() << std::endl;
  }
  catch (APIError &e) {
    std::cout << "APIError: " << e.getCode() << ", " << e.getMessage() << std::endl;
  }
}
