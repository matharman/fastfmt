add_executable(fastfmt_sink_ex main.c)
target_compile_options(fastfmt_sink_ex PRIVATE -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=FASTFMT_SINK)
target_link_libraries(fastfmt_sink_ex PRIVATE fastfmt)
target_link_options(fastfmt_sink_ex PRIVATE -Wl,-T,${CMAKE_CURRENT_LIST_DIR}/fastfmt_sink_ex.ld)

add_custom_command(TARGET fastfmt_sink_ex POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS --dump-section .fastfmt=${CMAKE_CURRENT_BINARY_DIR}/fastfmt_strings ${CMAKE_CURRENT_BINARY_DIR}/fastfmt_sink_ex)
