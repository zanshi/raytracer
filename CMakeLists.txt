cmake_minimum_required(VERSION 3.8)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp vertex.h direction.h colordbl.h ray.h triangle.h pixel.h scene.cpp scene.h scene.cpp scene.h camera.cpp camera.h)
add_executable(raytracer ${SOURCE_FILES})