cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(portaudio-download NONE)

include(ExternalProject)

ExternalProject_Add(
  portaudio
  SOURCE_DIR "@PORTAUDIO_DOWNLOAD_ROOT@/portaudio-src"
  BINARY_DIR "@PORTAUDIO_DOWNLOAD_ROOT@/portaudio-build"
  GIT_REPOSITORY https://git.assembla.com/portaudio.git
  GIT_TAG c5e8f1dca7f2d1b9c3d5c92a3f6f174870c303d8
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
)
