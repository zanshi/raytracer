cmake_minimum_required(VERSION 3.8)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        src/camera.cpp
        src/scene.cpp
        src/triangle.cpp
        src/trianglemesh.cpp)
set(HEADERS
        src/common.h
        src/camera.h
        src/scene.h
        src/ray.h
        src/pixel.h
        src/vertex.h
        src/triangle.h
        src/direction.h
        src/colordbl.h
        src/trianglemesh.h
        src/shape.h
        src/sphere.h)

include_directories(src)
include_directories(vendor)

add_library(raytracer
        ${SOURCES}
        ${HEADERS})


set(ALL_LIBS
        raytracer
        )

add_executable(raytracer_exe main.cpp)

SET_TARGET_PROPERTIES ( raytracer_exe PROPERTIES OUTPUT_NAME raytracer )
TARGET_LINK_LIBRARIES ( raytracer_exe ${ALL_LIBS} )

INSTALL ( TARGETS
        raytracer_exe
        DESTINATION
        bin
        )