macro(fetch_portaudio _download_module_path _download_root)
  set(PORTAUDIO_DOWNLOAD_ROOT ${_download_root})

  # copy download cmake file to the portaudio download root
  configure_file(
    ${_download_module_path}/portaudio-download.cmake
    ${_download_root}/CMakeLists.txt
    @ONLY
    )
  unset(PORTAUDIO_DOWNLOAD_ROOT)

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

  # add the targets 'portaudio' and 'portaudio_static'
  add_subdirectory(
    ${_download_root}/portaudio-src
    ${_download_root}/portaudio-build
    )
endmacro()
