cmake_minimum_required(VERSION 3.17)
project(RayTracing CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
include_directories(.)

set(CMAKE_CXX_STANDARD 20)
SET(GCC_COVERAGE_LINK_FLAGS "-lsfml-graphics  -lsfml-window -lsfml-system -O3")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(RayTracingInteractive interactive.cpp vec3.hpp color.hpp ray.hpp hittable/hittable.hpp hittable/sphere.hpp hittable/hittable_list.hpp rtweekend.hpp camera.hpp modifiers/material.hpp timer.hpp raytracer.hpp hittable/2dhittables.hpp hittable/moving_sphere.hpp hittable/aabb.hpp hittable/bvh.hpp modifiers/texture.hpp modifiers/perlin.hpp rtw_stb_image.hpp hittable/box.hpp modifiers/rotate.hpp modifiers/constant_medium.hpp hittable/cylinder.hpp hittable/cone.hpp scenes.hpp onb.hpp denoise.hpp)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(RayTracingInteractive sfml-graphics)
target_link_libraries(RayTracingInteractive "/usr/local/Cellar/open-image-denoise/1.3.0/lib/libOpenImageDenoise.1.3.0.dylib")

