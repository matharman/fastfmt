cmake_minimum_required(VERSION 3.16)
project(fastfmt C CXX)

list(APPEND CMAKE_EXE_LINKER_FLAGS "-Wl,-T,${CMAKE_CURRENT_LIST_DIR}/fastfmt.ld")

add_subdirectory(../rzcobs-c rzcobs_bin)

add_library(fastfmt STATIC fastfmt.c)
target_link_libraries(fastfmt PRIVATE rzcobs)
target_include_directories(fastfmt PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_executable(fastfmt_sink_ex main.c)
target_compile_options(fastfmt_sink_ex PRIVATE -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=FASTFMT -std=gnu11)
target_link_libraries(fastfmt_sink_ex PRIVATE fastfmt)

add_custom_command(TARGET fastfmt_sink_ex POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS --dump-section .fastfmt=${CMAKE_CURRENT_BINARY_DIR}/fastfmt_strings ${CMAKE_CURRENT_BINARY_DIR}/fastfmt_sink_ex)
