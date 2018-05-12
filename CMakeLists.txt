cmake_minimum_required(VERSION 3.4)
project(aurora LANGUAGES C CXX)

# compile with C++11
set(CMAKE_CXX_STANDARD 11)

# library & binary destinations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# third party code download/installation location
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

# generate compile_commands.json so editors know header search paths
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----------------------------------------------------------------------------
# options
# ----------------------------------------------------------------------------

option(ENABLE_CPPCHECK "Download and build cppcheck from Github" ON)
option(ENABLE_TESTS "Download & build googletest and run Aurora tests" ON)
option(ENABLE_EXAMPLES "Build demo executables" OFF)

# ----------------------------------------------------------------------------
# cpr (curl wrapper) library
# ----------------------------------------------------------------------------
include(cmake/cpr.cmake)
fetch_cpr(
  ${PROJECT_SOURCE_DIR}/cmake
  ${THIRD_PARTY_DIR}/cpr)


# ----------------------------------------------------------------------------
# json library
# ----------------------------------------------------------------------------
include(cmake/json.cmake)
fetch_json(
  ${PROJECT_SOURCE_DIR}/cmake
  ${THIRD_PARTY_DIR}/json)

# ----------------------------------------------------------------------------
# portaudio library
# ----------------------------------------------------------------------------
include(cmake/portaudio.cmake)
fetch_portaudio(
  ${PROJECT_SOURCE_DIR}/cmake
  ${THIRD_PARTY_DIR}/portaudio)

# ----------------------------------------------------------------------------
# Aurora c++ library target
# ----------------------------------------------------------------------------
add_subdirectory(src)

# ----------------------------------------------------------------------------
# cppcheck static code analyzer
# ----------------------------------------------------------------------------
if (ENABLE_CPPCHECK)
  set(CPPCHECK_INSTALL_PREFIX ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cppcheck)

  include(cmake/cppcheck.cmake)
  fetch_cppcheck(
    ${PROJECT_SOURCE_DIR}/cmake
    ${THIRD_PARTY_DIR}/cppcheck
    )

  # run cppcheck on `make all`
  add_custom_target(
    cppcheck ALL
    COMMAND ${CPPCHECK_INSTALL_PREFIX}/bin/cppcheck${CMAKE_EXECUTABLE_SUFFIX}
    --enable=warning,performance,portability,style
    --std=c++11
    --template=vs
    --quiet
    --verbose
    -I"${CMAKE_SOURCE_DIR}/include/"
    include/
    src/ -isrc/CMakeFiles
    )
endif()

# ----------------------------------------------------------------------------
# googletest
# ----------------------------------------------------------------------------
if (ENABLE_TESTS)
  include(cmake/googletest.cmake)
  fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${THIRD_PARTY_DIR}/googletest)
endif()

# ----------------------------------------------------------------------------
# Tests
# ----------------------------------------------------------------------------
if (ENABLE_TESTS)
  # adds 'make test' target that runs all tests
  enable_testing()
  add_subdirectory(tests)
endif()

# ----------------------------------------------------------------------------
# Examples
# ----------------------------------------------------------------------------
if(ENABLE_EXAMPLES)
	add_subdirectory(examples)
endif()

# ----------------------------------------------------------------------------
# Doxygen
# ----------------------------------------------------------------------------
# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
  # set input and output files
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  # request to configure the file
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  # 'make doc' target
  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
