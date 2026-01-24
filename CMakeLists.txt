cmake_minimum_required(VERSION 3.25)

# ------ CMake Modules ------

project(c-sqlite-clone)

if(WIN32 OR APPLE)
    set(BUILD_SHARED_LIBS OFF) 
else()
    set(BUILD_SHARED_LIBS ON)
endif()

# ------ Libraries ------

add_subdirectory(logger) 	# Logging
# add_subdirectory(utils) 	# Includes nob (no build) and stb for C hash tables

# ------ Executables ------

add_subdirectory(database)
