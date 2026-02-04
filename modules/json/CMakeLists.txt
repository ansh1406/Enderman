cmake_minimum_required(VERSION 3.10)
project(json_module LANGUAGES CXX)

file(GLOB JSON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
set(JSON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(json_objects OBJECT ${JSON_SRC})
target_include_directories(json_objects PUBLIC ${JSON_INCLUDE_DIR})
add_library(json STATIC $<TARGET_OBJECTS:json_objects>)
target_include_directories(json PUBLIC ${JSON_INCLUDE_DIR})

set_target_properties(json PROPERTIES
	OUTPUT_NAME "json"
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)