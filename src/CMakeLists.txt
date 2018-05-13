add_library(aurora STATIC
  Backend.cpp
  AuroraError.cpp
  APIError.cpp
  API.cpp
  Config.cpp
  Text.cpp
  Speech.cpp
  WAV.cpp
  AudioFile.cpp)
# look in ../include/ for aurora headers & add to interface
target_include_directories(aurora PUBLIC
  ${CMAKE_SOURCE_DIR}/include)
# add dependencies to interface
target_link_libraries(aurora PUBLIC
  cpr
  json
  portaudio_static)
