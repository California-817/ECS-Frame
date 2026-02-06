cmake_minimum_required(VERSION 3.10)

project(ECS-Frame)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -Og -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated  -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
# set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -Og -fPIC -ggdb -std=c11 -Wall -Wno-deprecated  -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/actor_version/core)

# file(GLOB_RECURSE ACTOR_SOURCES
#     ${CMAKE_CURRENT_SOURCE_DIR}/actor_version/core/*.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/actor_version/core/protobuf/*.cc
# )

# add_library(ecsfrmat STATIC ${ACTOR_SOURCES})

# set_target_properties(ecsfrmat PROPERTIES
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
#     LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
# )

# add_executable(test ${CMAKE_CURRENT_SOURCE_DIR}/actor_version/app/test/test.cpp)

# target_link_libraries(test ecsfrmat
#                            spdlog
#                            dl
#                            yaml-cpp
#                            protobuf
# )

# set_target_properties(test PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
# )
