cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(libsndfile-download NONE)

include(ExternalProject)

ExternalProject_Add(
  libsndfile
  SOURCE_DIR "@LIBSNDFILE_DOWNLOAD_ROOT@/libsndfile-src"
  BINARY_DIR "@LIBSNDFILE_DOWNLOAD_ROOT@/libsndfile-build"
  GIT_REPOSITORY https://github.com/erikd/libsndfile.git
  GIT_TAG 7ea3f9d8746000cc82c016d0b5d48452bb80e9fe
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
)
