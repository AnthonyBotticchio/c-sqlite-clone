cmake_minimum_required(VERSION 3.25)

# ------ CMake Modules ------

project(c-sqlite-clone)

if(WIN32)
    set(BUILD_SHARED_LIBS OFF) 	# TODO: Builds on Windows don't detect dll's yet
else()
    set(BUILD_SHARED_LIBS ON) 	# Builds all libs as SHARED
endif()

# ------ Libraries ------

add_subdirectory(logger) 	# This is for logging
add_subdirectory(utils) 	# Includes nob (no build) and stb for C hash tables

# ------ Executables ------

add_subdirectory(database)
