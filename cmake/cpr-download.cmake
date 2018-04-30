cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(cpr-download NONE)

include(ExternalProject)

ExternalProject_Add(
  cpr
  SOURCE_DIR "@CPR_DOWNLOAD_ROOT@/cpr-src"
  BINARY_DIR "@CPR_DOWNLOAD_ROOT@/cpr-build"
  GIT_REPOSITORY https://github.com/whoshuu/cpr.git
  GIT_TAG 3d14e61ed247a90a07db26fb107beb3086a035d6
  GIT_SUBMODULES "opt/curl"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
)
