cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(curl-download NONE)

include(ExternalProject)

ExternalProject_Add(
  curl
  SOURCE_DIR "@CURL_DOWNLOAD_ROOT@/curl-src"
  BINARY_DIR "@CURL_DOWNLOAD_ROOT@/curl-build"
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG curl-7_59_0
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
  )

