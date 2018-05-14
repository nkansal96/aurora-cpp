macro(test_macro test_name)
  add_executable(test_${test_name}
    test_${test_name}.cpp)
  # link googletest and aurora to test
  target_link_libraries(test_${test_name}
    gtest_main
    gmock_main
    aurora)
  # add test target to tests
  add_test(NAME test_${test_name} COMMAND test_${test_name})
endmacro(test_macro)

# ----------------------------------------------------------------------------
# API test target
# ----------------------------------------------------------------------------
test_macro(api)

# ----------------------------------------------------------------------------
# Text test target
# ----------------------------------------------------------------------------
test_macro(text)

# ----------------------------------------------------------------------------
# Backend test target
# ----------------------------------------------------------------------------
test_macro(backend)

# ----------------------------------------------------------------------------
# Speech test target
# ----------------------------------------------------------------------------
test_macro(speech)



