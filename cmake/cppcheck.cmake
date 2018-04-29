macro(fetch_cppcheck _download_module_path _download_root)
  set(CPPCHECK_DOWNLOAD_ROOT ${_download_root})

  # copy download cmake file to the cppcheck download root
  configure_file(
    ${_download_module_path}/cppcheck-download.cmake
    ${_download_root}/CMakeLists.txt
    @ONLY
    )
  unset(CPPCHECK_DOWNLOAD_ROOT)

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

endmacro()
