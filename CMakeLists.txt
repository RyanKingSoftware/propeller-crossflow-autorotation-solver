cmake_minimum_required(VERSION 3.30)
project(solver VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Release)

add_executable(${PROJECT_NAME} main.cpp solver.h system_parameters.h vec3.h util.h)
