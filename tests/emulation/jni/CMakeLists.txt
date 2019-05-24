cmake_minimum_required(VERSION 3.14)
project(mla_gfx)

include_directories(../../../framework)
include_directories(..)
include_directories(../baselines)
include_directories("$ENV{JAVA_HOME}/include")
include_directories("$ENV{JAVA_HOME}/include/win32")

add_compile_definitions(NO_WEAK)
add_definitions(-D__int64=int64_t)
add_compile_options(-fms-extensions)

add_library(
        emulator
        SHARED
        ../../../framework/gfx/src/gfx_primitive.c
        ../virtual_screen.c
        emulator.c
        )
set_property(TARGET emulator PROPERTY C_STANDARD 99)



