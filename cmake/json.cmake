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

  # include all nlohmann_json interface directories
  include_directories(${_download_root}/json-src/single_include/)
endmacro()
