cmake_minimum_required(VERSION 3.16)
project(defmt C CXX)

list(APPEND CMAKE_EXE_LINKER_FLAGS "-Wl,-T,${CMAKE_CURRENT_LIST_DIR}/defmt.ld")

#add_subdirectory(../rzcobs-c rzcobs_bin)

add_executable(defmt_exp main.c vlog.c)
target_compile_options(defmt_exp PRIVATE -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=ROOT)
#target_link_libraries(defmt_exp PRIVATE rzcobs)
