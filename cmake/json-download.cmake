cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(json-download NONE)

include(ExternalProject)

ExternalProject_Add(
  json
  PREFIX @JSON_DOWNLOAD_ROOT@
  SOURCE_DIR "@JSON_DOWNLOAD_ROOT@/json-src"
  BINARY_DIR "@JSON_DOWNLOAD_ROOT@/json-build"
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.1.2
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
)
