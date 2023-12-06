cmake_minimum_required(VERSION 3.21)

project(gui)

add_executable(gui src/main.c)

add_library(raylib  STATIC IMPORTED)

set_property(TARGET raylib PROPERTY IMPORTED_LOCATION ../lib/libraylib.a)

target_link_libraries(gui raylib)
target_link_libraries(gui m)
target_link_libraries(gui opengl32)
target_link_libraries(gui gdi32)
target_link_libraries(gui winmm)

include_directories(./include)

