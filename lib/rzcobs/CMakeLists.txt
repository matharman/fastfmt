add_library(rzcobs STATIC rzcobs.c)
target_include_directories(rzcobs PUBLIC ${CMAKE_CURRENT_LIST_DIR})

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
