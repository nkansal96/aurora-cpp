macro(fetch_curl _download_module_path _download_root)
  set(CURL_DOWNLOAD_ROOT ${_download_root})

  # copy download cmake file to the curl download root
  configure_file(
    ${_download_module_path}/curl-download.cmake
    ${_download_root}/CMakeLists.txt
    @ONLY
    )
  unset(CURL_DOWNLOAD_ROOT)

  execute_process(
    COMMAND
    "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY
    ${_download_root}
    )
  execute_process(
    COMMAND
    "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY
    ${_download_root}
    )

  # set options
  set(CURL_STATICLIB ON CACHE INTERNAL "build curl static lib")
  set(CURL_SHARED_LIBS OFF CACHE INTERNAL "build curl shared library")
  set(CURL_ZLIB OFF CACHE INTERNAL "build curl with zlib")
  set(HTTP_ONLY ON CACHE INTERNAL "disable all protocols except HTTP")
  set(BUILD_TESTING OFF CACHE INTERNAL "No curl tests")
  set(BUILD_CURL_EXE OFF CACHE INTERNAL "build curl exe")

  if(APPLE)
    set(CMAKE_USE_DARWINSSL ON CACHE INTERNAL "build curl with macOS ssl")
    set(CMAKE_USE_OPENSSL OFF CACHE INTERNAL "build curl with openSSL")
    set(CURL_CA_PATH_SET OFF CACHE INTERNAL "set curl CA_PATH")
  endif(APPLE)

  # adds the target 'libcurl'
  add_subdirectory(
    ${_download_root}/curl-src
    ${_download_root}/curl-build
    )
endmacro()
