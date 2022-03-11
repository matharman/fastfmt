add_library(rzcobs STATIC rzcobs.c)
target_include_directories(rzcobs PUBLIC ${CMAKE_CURRENT_LIST_DIR})

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
