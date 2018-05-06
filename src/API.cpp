#include "aurora/API.h"
#include "aurora/Backend.h"
#include "aurora/Config.h"
#include "aurora/errors/AuroraError.h"
#include "aurora/errors/APIError.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>

#include <iostream>

/// for convenience
using json = nlohmann::json;

// HTTP status codes
const int STATUS_CODE_OK = 200;

namespace aurora {

InterpretResponse API::getInterpret(const std::string &text) {
  // set up API call params
  CallParams params;
  params.method = GET;
  params.path = INTERPRET_PATH;
  params.credentials = config.getCredentials();
  params.query["text"] = text;

  // request interpretation from API server
  HTTPResponse httpRes = config.backend->call(params);

  // check if there were api errors
  API::checkStatus(httpRes);

  // convert response body to json object
  json j = json::parse(httpRes.response);

  InterpretResponse interpretRes;
  interpretRes.text = text;

  // extract interpret value
  if (j.find("intent") != j.end()) {
    interpretRes.intent = j["intent"];
  }
  else {
    throw AuroraError("MissingIntentField", "No 'intent' field found in interpret response object", "API::getInterpret");
  }


  // extract entities field
  if (j.find("entities") != j.end()) {
    const auto entities = j["entities"];
    for (auto it = entities.begin(); it != entities.end(); it++) {
      interpretRes.entities[it.key()] = it.value();
    }
  }
  else {
    throw AuroraError("MissingEntitiesField", "No 'entities' field found in interpret response object", "API::getInterpret");
  }

  return interpretRes;
}

void API::checkStatus(HTTPResponse &res) {
  if (res.statusCode != STATUS_CODE_OK) {
    // convert response body to json object
    json j = json::parse(res.response);

    throw APIError(j["id"], std::to_string(res.statusCode), j["code"], j["type"], j["message"], "API::checkStatus");
  }
}

}
