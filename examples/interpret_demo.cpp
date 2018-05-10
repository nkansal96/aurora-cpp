#include "aurora/Config.h"
#include "aurora/Text.h"
#include "aurora/Interpret.h"
#include <cstdlib>
#include <string>
#include <iostream>

void setAuroraConfiguration();  
void printInterpretationIntent(const aurora::Interpret& interpretation);
void printInterpretationEntities(const aurora::Interpret& interpretation);

int main() {
  
  setAuroraConfiguration();
  
  for(;;) {
    std::cout << "Enter a phrase to be interpretted as> ";
    std::string phrase;
    std::getline(std::cin, phrase);
    
    //create a Text object
    aurora::Text auroraText(phrase);
      
    
    try {
      //interpret Text
      aurora::Interpret interpretation = auroraText.interpret();
      
      printInterpretationIntent(interpretation);
      printInterpretationEntities(interpretation);
    }
    catch (std::exception &e) {
      std::cout << "Exception occurred in interpretation: " << e.what() << std::endl;
    }
    
  }
  
}

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

void printInterpretationIntent(const aurora::Interpret& interpretation) {
  if (interpretation.intent != "") { //check to see if intent was understood                                          
    std::cout << "intent: " << interpretation.intent << std::endl; //print intent                
  } else {
    std::cout << "Error, intention was unclear" << std::endl;
  }
}

void printInterpretationEntities(const aurora::Interpret& interpretation) {
  if (!interpretation.entities.empty()){ //check to see if no information was detected                                
    
    //print entities                                                                                                  
    for(std::unordered_map<std::string, std::string>::const_iterator it = interpretation.entities.begin(); it != interpretation.entities.end(); it++){
      std::cout << it->first << " : " << it->second << std::endl;
    }
  } else {
    std::cout << "Error, no information pertaining to intent" << std::endl;
  }
}

