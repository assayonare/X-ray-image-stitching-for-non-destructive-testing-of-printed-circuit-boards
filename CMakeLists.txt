cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(image_stitching)

set(OpenCV_DIR "C:/opencv/build")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(image_stitching src/main.cpp)
target_link_libraries(image_stitching ${OpenCV_LIBS})

