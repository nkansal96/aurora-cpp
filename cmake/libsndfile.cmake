macro(fetch_libsndfile _download_module_path _download_root)
  set(LIBSNDFILE_DOWNLOAD_ROOT ${_download_root})

  # copy download cmake file to the libsndfile download root
  configure_file(
    ${_download_module_path}/libsndfile-download.cmake
    ${_download_root}/CMakeLists.txt
    @ONLY
    )
  unset(LIBSNDFILE_DOWNLOAD_ROOT)

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
  set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "don't build shared lib")
  set(BUILD_STATIC_LIBS ON CACHE INTERNAL "build static lib")
  set(DISABLE_EXTERNAL_LIBS ON CACHE INTERNAL "disable support for OGG, VORBIS, FLAC")
  set(BUILD_PROGRAMS OFF CACHE INTERNAL "don't build libsndfile utils")
  set(BUILD_TESTING OFF CACHE INTERNAL "don't build tests")
  set(BUILD_EXAMPLES OFF CACHE INTERNAL "don't build examples")
  set(BUILD_REGTEST OFF CACHE INTERNAL "don't build regtest program")

  # adds the target 'libsndfile'
  add_subdirectory(
    ${_download_root}/libsndfile-src
    ${_download_root}/libsndfile-build
    )

endmacro()
