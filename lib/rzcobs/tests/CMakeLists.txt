include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.11.0
	)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

macro(add_gtest_bin)
	set(options)
	set(one-val EXE)
	set(multi-val SOURCES)
	cmake_parse_arguments(GTEST_BIN "${options}" "${one-val}" "${multi-val}" ${ARGN})

	add_executable(${GTEST_BIN_EXE} ${GTEST_BIN_SOURCES})
	target_link_libraries(${GTEST_BIN_EXE} PRIVATE
		rzcobs
		gtest_main
		gmock_main
		asan
		ubsan
		)

	target_compile_features(${GTEST_BIN_EXE} PRIVATE cxx_std_17)
	target_compile_options(${GTEST_BIN_EXE} PRIVATE -fsanitize=undefined -fsanitize=address)
	gtest_discover_tests(${GTEST_BIN_EXE})
endmacro()

add_gtest_bin(
	EXE rzcobs_encode_test
	SOURCES rzcobs_encode_test.cc
	)
