cmake_minimum_required(VERSION 3.14)
project(mla_gfx)

set(CMAKE_CXX_STANDARD 99)

include_directories(framework)
include_directories(tests/emulation)
include_directories(tests/emulation/baselines)

add_compile_definitions(NO_WEAK)
add_definitions(-D__int64=int64_t)
add_compile_options(-fms-extensions)

aux_source_directory(tests/emulation/baselines/ baselinec)

add_library(
        fonts
        ${baselinec}
)

add_executable(
        emulation
        framework/gfx/src/gfx_primitive.c
        tests/emulation/main.c
        tests/emulation/primitive_tests.c
        tests/emulation/test_font_1.c
        tests/emulation/virtual_screen.c
        tests/emulation/baselines/tests.c
        tests/emulation/test_function.h)

target_link_libraries(emulation fonts)

