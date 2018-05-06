#include "aurora/API.h"
#include "aurora/Backend.h"
#include "aurora/Config.h"
#include "aurora/errors/AuroraError.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>

/// for convenience
using json = nlohmann::json;

namespace aurora {

InterpretResponse API::getInterpret(const std::string &text) {
  // set up API call params
  CallParams params;
  params.method = GET;
  params.path = INTERPRET_PATH;
  params.credentials = {config.appID,
                        config.appToken,
                        config.deviceID};
  params.query["text"] = text;

  // request interpretation from API server
  HTTPResponse httpRes = config.backend->call(params);

  // TODO: check for API errors

  // convert response body to json object
  json j = json::parse(httpRes.response);

  InterpretResponse interpretRes;
  interpretRes.text = text;

  // extract interpret value
  if (j.find("intent") != j.end()) {
    interpretRes.intent = j["intent"];
  }
  else {
    throw AuroraError("0", "No 'intent' field found in interpret response object", "API::getInterpret");
  }


  // extract entities field
  if (j.find("entities") != j.end()) {
    const auto entities = j["entities"];
    for (auto it = entities.begin(); it != entities.end(); it++) {
      interpretRes.entities[it.key()] = it.value();
    }
  }
  else {
    throw AuroraError("0", "No 'entities' field found in interpret response object", "API::getInterpret");
  }

  return interpretRes;
}



}
