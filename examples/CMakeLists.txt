cmake_minimum_required(VERSION 3.11)

# preprocessor variable for getting asset path in examples
add_definitions("-DEXAMPLES_ASSETS_PATH=\"${CMAKE_SOURCE_DIR}/examples/assets/\"")

# look for headers in example directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#----------------------------------------
# Interpret Demo via Text Class
#---------------------------------------
add_executable(interpret_example
  interpret_example.cpp)
target_link_libraries(interpret_example
  aurora)

#----------------------------------------
# STT example
#---------------------------------------
add_executable(stt_example
  stt_example.cpp
  exampleutils.cpp)
target_link_libraries(stt_example
  aurora)

#----------------------------------------
# Record STT example
#---------------------------------------
add_executable(record_stt_example
  record_stt_example.cpp
  exampleutils.cpp)
target_link_libraries(record_stt_example
  aurora)

#----------------------------------------
# TTS example
#---------------------------------------
add_executable(tts_example
  tts_example.cpp
  exampleutils.cpp)
target_link_libraries(tts_example
  aurora)

