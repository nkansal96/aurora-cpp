add_library(aurora STATIC
  Backend.cpp
  AuroraError.cpp)
# look in ./include/ for aurora headers & add to interface
target_include_directories(aurora PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  )
# add dependencies to interface
target_link_libraries(aurora INTERFACE libcurl nlohmann_json)
