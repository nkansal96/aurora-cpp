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

  # add all search paths to interface
  # add_library(libsndfile INTERFACE)
  include_directories( ${_download_root}/libsndfile-src/src)


endmacro()