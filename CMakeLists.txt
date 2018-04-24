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
  include)
target_link_libraries(aurora libcurl)

# test executable
add_executable(test_backend
  tests/test_backend.cpp)
target_include_directories(test_backend PRIVATE
   include)
target_link_libraries(test_backend gtest_main aurora)
# add mytest target to Makefile
add_test(NAME test_backend COMMAND test_backend)
