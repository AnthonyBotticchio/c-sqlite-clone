cmake_minimum_required(VERSION 3.25)

# ------ Application Setup ------

add_executable(c-sqlite-clone)

target_link_libraries(c-sqlite-clone PRIVATE
    logger
    utils
)

# ------ Sources ------

target_include_directories(c-sqlite-clone PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE C_SOURCES             "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE C_PRIVATE_HEADERS     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

target_sources(c-sqlite-clone PRIVATE
    ${C_SOURCES} 
    ${C_PRIVATE_HEADERS}
)