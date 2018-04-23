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

  # download & install curl
  execute_process(
    COMMAND
    "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY
    ${_download_root}
    )


  set(CURL_LIBRARY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/curl/lib/libcurl.a)
  add_library(libcurl STATIC IMPORTED)
  #set_target_properties(curl PROPERTIES IMPORTED_LOCATION  ${CURL_LIBRARY})
  set_property(TARGET libcurl PROPERTY IMPORTED_LOCATION ${CURL_LIBRARY})

endmacro()
