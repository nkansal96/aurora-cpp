# ----------------------------------------------------------------------------
# API test target
# ----------------------------------------------------------------------------
add_executable(test_api
  test_api.cpp)
# link googletest and aurora to test
target_link_libraries(test_api
  gtest_main
  gmock_main
  aurora)
# add test_api target to tests
add_test(NAME test_api COMMAND test_api)

# ----------------------------------------------------------------------------
# Text test target
# ----------------------------------------------------------------------------
add_executable(test_text
  test_text.cpp)
# link googletest and aurora to test
target_link_libraries(test_text
  gtest_main
  gmock_main
  aurora)
# add test_text target to tests
add_test(NAME test_text COMMAND test_text)



