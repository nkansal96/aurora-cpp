cmake_minimum_required(VERSION 3.4)
project(aurora)

# library & binary destinations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# third party code download/installation location
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

# googletest
include(cmake/googletest.cmake)
fetch_googletest(
  ${PROJECT_SOURCE_DIR}/cmake
  ${THIRD_PARTY_DIR}/googletest)

# libcurl
include(cmake/curl.cmake)
fetch_curl(
  ${PROJECT_SOURCE_DIR}/cmake
  ${THIRD_PARTY_DIR}/curl)

# Aurora c++ library
add_library(aurora STATIC
  src/Backend.cpp)
target_include_directories(aurora PRIVATE
  src/include/aurora)
target_link_libraries(aurora libcurl)

# test executable
add_executable(mytest
  tests/mytest.cpp)
target_include_directories(mytest PRIVATE
   src/include)
target_link_libraries(mytest gtest_main aurora)
# add mytest target to Makefile
add_test(NAME mytest COMMAND mytest)
