cmake_minimum_required(VERSION 3.1)

project(Arrancar0)

option(USE_CLANG "build application with clang" ON) # ON is the default

if (USE_CLANG)
	SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
	SET (CMAKE_AR      "/usr/bin/llvm-ar")
	SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
	SET (CMAKE_NM      "/usr/bin/llvm-nm")
	SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
	SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
endif (USE_CLANG)


if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING
		"Choose the type of build, options are: Debug Release
		RelWithDebInfo MinSizeRel."
		FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Specify output directories.
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

# Include custom macros to find SC2Api.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wthread-safety -fno-omit-frame-pointer -fsanitize=all")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} -Wthread-safety -fno-omit-frame-pointer -fsanitize=all")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_package(SC2Api REQUIRED)

find_package(RapidJSON REQUIRED)

find_package(GLog REQUIRED)

find_package(GTest REQUIRED)

# Build with c++14 support, required by sc2api.
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("src")

option(ENABLE_TESTS "Build tests" ON)
if(ENABLE_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

# Futile test
# add_custom_command(
		# OUTPUT compile_commands.json
		# COMMAND ${CMAKE_COMMAND} -E copy
			# ${CMAKE_SOURCE_DIR}/build/compile_commands.json
			# ${CMAKE_SOURCE_DIR}
		# DEPENDS compile_commands.json
	# )
