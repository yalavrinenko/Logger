cmake_minimum_required(VERSION 2.8)

package(Logger)

set(CMAKE_CXX_STANDARD 14)

add_library(Logger SHARED Logger.hpp Logger.cpp)
