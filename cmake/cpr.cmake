macro(fetch_cpr _download_module_path _download_root)
  set(CPR_DOWNLOAD_ROOT ${_download_root})

  # copy download cmake file to the cpr download root
  configure_file(
    ${_download_module_path}/cpr-download.cmake
    ${_download_root}/CMakeLists.txt
    @ONLY
    )
  unset(CPR_DOWNLOAD_ROOT)

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
  set(BUILD_CPR_TESTS OFF CACHE INTERNAL "build cpr tests")

  if(APPLE)
    set(USE_SYSTEM_CURL ON CACHE INTERNAL "do use system curl")
    set(CMAKE_USE_DARWINSSL ON CACHE INTERNAL "use darwinssl")
  endif()

  if(WIN32)
    set(CMAKE_USE_OPENSSL OFF CACHE INTERNAL "don't use openssl")
    set(USE_WINDOWS_SSPI ON CACHE INTERNAL "do use winssl")
  endif()

  # adds the target 'cpr'
  add_subdirectory(
    ${_download_root}/cpr-src
    ${_download_root}/cpr-build
    )
endmacro()
