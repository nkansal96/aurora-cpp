macro(fetch_json _download_module_path _download_root)
  set(JSON_DOWNLOAD_ROOT ${_download_root})

  # copy download cmake file to the json download root
  configure_file(
    ${_download_module_path}/json-download.cmake
    ${_download_root}/CMakeLists.txt
    @ONLY
    )
  unset(JSON_DOWNLOAD_ROOT)

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

  # add all nlohmann_json header search paths to interface
  add_library(json INTERFACE)
  target_include_directories(json INTERFACE ${_download_root}/json-src/single_include)

endmacro()
