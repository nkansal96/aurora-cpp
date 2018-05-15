cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(libsndfile-download NONE)

include(ExternalProject)

ExternalProject_Add(
  libsndfile
  SOURCE_DIR "@LIBSNDFILE_DOWNLOAD_ROOT@/libsndfile-src"
  BINARY_DIR "@LIBSNDFILE_DOWNLOAD_ROOT@/libsndfile-build"
  GIT_REPOSITORY https://github.com/erikd/libsndfile.git
  GIT_TAG 1.0.28
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
)
