cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(curl-download NONE)

include(ExternalProject)

# configure command
set(CURL_PREFIX @CMAKE_LIBRARY_OUTPUT_DIRECTORY@/curl)
# TODO: enable SSL & zlib, windows support
set(CURL_CONFIGURE @CURL_DOWNLOAD_ROOT@/curl-src/configure
  --enable-static
  --disable-shared
  --without-ssl
  --without-zlib
  --disable-manual
  --disable-ldap
  --without-libidn2
  --disable-cookies
  --disable-ftp
  --disable-imap
  --prefix=${CURL_PREFIX})

ExternalProject_Add(
  curl
  SOURCE_DIR "@CURL_DOWNLOAD_ROOT@/curl-src"
  BINARY_DIR "@CURL_DOWNLOAD_ROOT@/curl-build"
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG curl-7_59_0
  PATCH_COMMAND @CURL_DOWNLOAD_ROOT@/curl-src/buildconf
  CONFIGURE_COMMAND ${CURL_CONFIGURE}
  BUILD_COMMAND make
  INSTALL_COMMAND make install
  TEST_COMMAND ""
  )
