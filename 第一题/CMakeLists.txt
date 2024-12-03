cmake_minimum_required(VERSION 3.10)
project(RGB2NV12)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(rgb2nv12 main.cpp)
target_link_libraries(rgb2nv12 ${OpenCV_LIBS})