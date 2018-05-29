#include "aurora/API.h"
#include "aurora/Backend.h"
#include "aurora/Config.h"
#include "aurora/errors/AuroraError.h"
#include "aurora/errors/APIError.h"
#include "aurora/AudioFile.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>

#include <iostream>

/// for convenience
using json = nlohmann::json;

// HTTP status codes
const int STATUS_CODE_OK = 200;

namespace aurora {

  std::string API::getSTT(const AudioFile &file) {
    return getSTTFromBuffer(file.getWAVData());
  }

  std::string API::getSTTFromBuffer(const Buffer &audio) {
    // set up API call params                                                                                 
    CallParams params;
    params.method = POST;
    params.path = STT_PATH;
    params.credentials = config.getCredentials();
    params.body = audio;

    // request text from API server                                                                           
    HTTPResponse httpRes = config.backend->call(params);

    // check if there were api errors                                                                         
    API::checkStatus(httpRes);

    // convert response body to json object                                                                   
    json j = json::parse(httpRes.response);

    // extract transcript field value                                                                         
    if (j.find("transcript") == j.end()) {
      throw AuroraError("MissingTranscriptField", "No 'transcript' field found in stt response object", "API:\
:getSTT");
    }

    return j["transcript"];
  }

AudioFile API::getTTS(const std::string &text) {
  // set up API call params
  CallParams params;
  params.method = GET;
  params.path = TTS_PATH;
  params.credentials = config.getCredentials();
  params.query["text"] = text;

  // request speech from API server
  HTTPResponse httpRes = config.backend->call(params);

  // check if there were api errors
  API::checkStatus(httpRes);

  // copy response into a buffer and create an audio file with it
  Buffer audioBuffer(httpRes.response.begin(), httpRes.response.end());
  return AudioFile(audioBuffer);
}

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

  // ensure intent field exists
  if (j.find("intent") == j.end()) {
    throw AuroraError("MissingIntentField", "No 'intent' field found in interpret response object", "API::getInterpret");
  }

  interpretRes.intent = j["intent"];

  // ensure entities field exists
  if (j.find("entities") == j.end()) {
    throw AuroraError("MissingEntitiesField", "No 'entities' field found in interpret response object", "API::getInterpret"); 
  }

  // copy entities into map
  const auto entities = j["entities"];
  for (auto it = entities.begin(); it != entities.end(); it++) {
    interpretRes.entities[it.key()] = it.value();
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
