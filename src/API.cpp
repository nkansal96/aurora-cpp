#include "aurora/API.h"
#include "aurora/Backend.h"
#include "aurora/Config.h"
#include <cpr/cpr.h>
#include <string>


namespace aurora {

  InterpretResponse API::getInterpret(std::string text) {
    CallParams params;
    
    params.method = HTTPMethod.GET;
    params.path = INTERPRET_PATH;
    params.credentials.appID = Config.APP_ID;
    params.credentials.appToken = Config.APP_TOKEN;
    params.credentials.deviceID = Config.DEVICE_ID;
    params.query["text"] = text;
    HTTPResponse httpResp = Config.backend.call(params);
    
    InterpretResponse interpretResp;
    interpretResp.text = ;
    interpretResp.intent =;
    interpretResp.entities =;
    return interpretResp;
}



}
