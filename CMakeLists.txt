cmake_minimum_required(VERSION 3.8)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pedantic -Wall -Wextra -O3 -march=native -mtune=native")


#SET(CMAKE_C_COMPILER /usr/local/bin/gcc-7)
#SET(CMAKE_CXX_COMPILER /usr/local/bin/g++-7)

FIND_PACKAGE(OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(SOURCES
        src/camera.cpp
        src/scene.cpp
        src/triangle.cpp
        src/vector.h
        src/trianglemesh.cpp src/sphere.cpp src/sceneobject.cpp)
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
        src/sphere.h src/sceneobject.h src/material.h src/light.h src/point_light.h src/area_light.h src/bsdf.h src/intersectioninfo.h src/rng.h)

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
