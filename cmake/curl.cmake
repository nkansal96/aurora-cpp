cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(ExternalProject)

# the libcurl CMake file is poorly maintained
# so we build and add the target ourself

set(CURL_DOWNLOAD_ROOT ${THIRD_PARTY_DIR}/curl)
set(CURL_PREFIX ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/curl)
set(CURL_OPTIONS
  -DCMAKE_INSTALL_PREFIX=${CURL_PREFIX}
  -DCURL_STATICLIB=ON
  -DBUILD_CURL_EXE=OFF
  -DHTTP_ONLY=ON
  -DBUILD_CURL_TESTS=OFF
  -DBUILD_SHARED_LIBS=OFF
  -DCURL_ZLIB=OFF
  )

if(APPLE)
  set(CURL_OPTIONS ${CURL_OPTIONS} -DCMAKE_USE_DARWINSSL=ON -DCMAKE_USE_OPENSSL=OFF  -DCURL_CA_PATH_SET=OFF)
endif(APPLE)

ExternalProject_Add(
  curl
  PREFIX ${CURL_DOWNLOAD_ROOT}
  SOURCE_DIR ${CURL_DOWNLOAD_ROOT}/curl-src
  BINARY_DIR ${CURL_DOWNLOAD_ROOT}/curl-build
  GIT_REPOSITORY https://github.com/curl/curl.git
  GIT_TAG curl-7_59_0
  CMAKE_ARGS 
  )

# add libcurl target
if(APPLE)
  find_library(COREFOUNDATION_FRAMEWORK "CoreFoundation")
  if(NOT COREFOUNDATION_FRAMEWORK)
      message(FATAL_ERROR "CoreFoundation framework not found")
  endif()

  find_library(SECURITY_FRAMEWORK "Security")
  if(NOT SECURITY_FRAMEWORK)
     message(FATAL_ERROR "Security framework not found")
  endif()

  set(CURL_LIBS "${COREFOUNDATION_FRAMEWORK}" "${SECURITY_FRAMEWORK}")
endif()



 set(LIBCURL_FILENAME ${CMAKE_STATIC_LIBRARY_PREFIX}curl${CMAKE_STATIC_LIBRARY_SUFFIX})
 set(CURL_LIBRARY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/curl/lib/${LIBCURL_FILENAME})
 add_library(libcurl STATIC IMPORTED)
 set_property(TARGET libcurl PROPERTY IMPORTED_LOCATION ${CURL_LIBRARY})
 add_dependencies(libcurl curl)
 target_link_libraries(libcurl INTERFACE ${CURL_LIBS})
