cmake_minimum_required(VERSION 3.10)

project(ECS-Frame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ACTOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/actor_version/*.cpp"
)

add_library(ecsfrmat STATIC ${ACTOR_SOURCES})

set_target_properties(ecsfrmat PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)
