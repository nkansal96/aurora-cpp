add_library(aurora STATIC
  Backend.cpp)
# look in ./include/ for aurora headers
include_directories(${CMAKE_SOURCE_DIR}/include)
# add dependencies to interface
target_link_libraries(aurora INTERFACE libcurl nlohmann_json)
