#include <aurora/Aurora.h>
#include <limits.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "exampleutils.h"


using namespace aurora;

int main(int argc, const char * argv[]) {
  setAuroraConfiguration();

  try {
    // open hw.wav file
    AudioFile fileA(assetsPath + "hw.wav");
    // create buffer of formatted WAV file with headers
    Buffer bufferA(fileA.getWAVData());
    AudioFile fileB(bufferA);
    fileB.play();

    // create copy of hw.wav
    fileB.writeToFile(assetsPath + "hw-copy.wav");
  }
  catch (AuroraError &e) {
    std::cout << "AuroraError: " << e.getCode() << ", " << e.getInfo() << ", " << e.getMessage() << std::endl;
  }
}
