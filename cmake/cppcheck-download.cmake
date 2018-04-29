cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(cppcheck-download NONE)

include(ExternalProject)

ExternalProject_Add(
  curl
  SOURCE_DIR "@CPPCHECK_DOWNLOAD_ROOT@/cppcheck-src"
  BINARY_DIR "@CPPCHECK_DOWNLOAD_ROOT@/cppcheck-build"
  GIT_REPOSITORY https://github.com/danmar/cppcheck
  GIT_TAG 1.83
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=@CPPCHECK_INSTALL_PREFIX@
  )
