cmake_minimum_required(VERSION 3.11)

#----------------------------------------
# Interpret Demo via Text Class
#---------------------------------------
add_executable(interpret_demo
  interpret_demo.cpp)
target_link_libraries(interpret_demo
  aurora)


