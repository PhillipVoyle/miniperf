
cmake_minimum_required(VERSION 3.16)

project(measure)

add_executable(measure measure.cc)
target_include_directories(measure PRIVATE "../include")


