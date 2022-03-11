cmake_minimum_required(VERSION 3.16)
project(defmt C CXX)

list(APPEND CMAKE_EXE_LINKER_FLAGS "-Wl,-T,${CMAKE_CURRENT_LIST_DIR}/defmt.ld")

add_subdirectory(../rzcobs-c rzcobs_bin)

add_executable(defmt_exp main.c vlog.c log_output.c)
target_compile_options(defmt_exp PRIVATE -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=ROOT -std=gnu11)
target_link_libraries(defmt_exp PRIVATE rzcobs)

#objcopy --dump-section .defmt=build/defmt_strings build/defmt_exp
add_custom_command(TARGET defmt_exp POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS --dump-section .defmt=${CMAKE_CURRENT_BINARY_DIR}/defmt_strings ${CMAKE_CURRENT_BINARY_DIR}/defmt_exp)
