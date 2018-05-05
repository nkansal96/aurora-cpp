#IFNDEF CONFIG_H
#DEF CONFIG_H
#include <string.h>
#include "aurora/backend.h"
namespace aurora {

struct Config {

  static std::string DEVICE_ID;
  static std::string APP_TOKEN;
  static std::string APP_ID;
  static Backend backend;
  

}

}
#ENDDEF //CONFIG_H
