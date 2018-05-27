#include "exampleutils.h"
#include <aurora/Aurora.h>
#include <stdlib.h>
#include <iostream>

void setAuroraConfiguration() {
  if(!getenv("APP_TOKEN")){
    std::cout << "ERROR: No \"APP_TOKEN\" enviornment variable set" << std::endl;
    exit(1);
  } else {
    aurora::config.appToken = getenv("APP_TOKEN");
  }
  if(!getenv("APP_ID")) {
    std::cout << "ERROR: No \"APP_ID\" enviornment variable set"<< std::endl;
    exit(1);
  } else {
    aurora::config.appID = getenv("APP_ID");
  }

  //device ID is optional, if not set, set as empty string
  if(!getenv("DEVICE_ID")) {
    aurora::config.deviceID = "";
  } else {
    aurora::config.deviceID = getenv("DEVICE_ID");
  }
}
