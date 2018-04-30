# ----------------------------------------------------------------------------
# backend test target
# ----------------------------------------------------------------------------
add_executable(test_backend
  test_backend.cpp)
# link googletest and aurora to test
target_link_libraries(test_backend
  gtest_main
  gmock_main
  aurora)
# add test_backend target to tests
add_test(NAME test_backend COMMAND test_backend)
