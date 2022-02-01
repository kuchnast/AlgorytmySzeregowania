cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AlgorytmySzeregowania)

include_directories(inc/)
set(SOURCES src/main.cpp)

#add_library(OTHER lib.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(lf linefeeder_v2/linefeeder.cpp)
add_executable(rep linefeeder_v2/repeater.cpp)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wpedantic -Wextra -Wno-unused-parameter -g)
target_link_libraries(${PROJECT_NAME} PUBLIC)
