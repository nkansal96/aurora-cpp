cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(ExternalProject)

# the libcurl CMake file is poorly maintained
# so we build and add the target ourself

set(CURL_DOWNLOAD_ROOT ${THIRD_PARTY_DIR}/curl)
set(CURL_PREFIX ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/curl)

# configure command
# TODO: enable SSL & zlib, windows support
set(CURL_CONFIGURE ${CURL_DOWNLOAD_ROOT}/curl-src/configure
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
  PREFIX ${CURL_DOWNLOAD_ROOT}
  SOURCE_DIR ${CURL_DOWNLOAD_ROOT}/curl-src
  BINARY_DIR ${CURL_DOWNLOAD_ROOT}/curl-build
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG curl-7_59_0
  UPDATE_COMMAND ""
  PATCH_COMMAND ${CURL_DOWNLOAD_ROOT}/curl-src/buildconf
  CONFIGURE_COMMAND ${CURL_CONFIGURE}
  BUILD_COMMAND make
  INSTALL_COMMAND make install
  TEST_COMMAND ""
  )

# add libcurl target
set(LIBCURL_FILENAME ${CMAKE_STATIC_LIBRARY_PREFIX}curl${CMAKE_STATIC_LIBRARY_SUFFIX})
set(CURL_LIBRARY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/curl/lib/${LIBCURL_FILENAME})
add_library(libcurl STATIC IMPORTED)
set_property(TARGET libcurl PROPERTY IMPORTED_LOCATION ${CURL_LIBRARY})
add_dependencies(libcurl curl)
