cmake_minimum_required(VERSION 3.16)
project(fastfmt C CXX)

add_subdirectory(lib/rzcobs)

add_library(fastfmt STATIC lib/fastfmt.c)
target_link_libraries(fastfmt PRIVATE rzcobs)
target_include_directories(fastfmt PUBLIC lib)
target_compile_options(fastfmt PRIVATE -Wall -Wextra -Werror -std=gnu11)

add_subdirectory(example)

if(BUILD_TESTING)
    enable_testing()
endif()
