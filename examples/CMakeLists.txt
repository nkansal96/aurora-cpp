cmake_minimum_required(VERSION 3.11)
#include_directories(../include)
#include_directories(../third_part/cpr/cpr-src)
#----------------------------------------
# Interpret Demo via Text Class
#---------------------------------------
project(interpret_demo)
#set(SOURCES interpret_demo.cpp ../src/*)
#file(GLOB SOURCES interpret_demo.cpp "../src/*.cpp")


add_executable(interpret_demo interpret_demo.cpp)
target_link_libraries(interpret_demo aurora)


