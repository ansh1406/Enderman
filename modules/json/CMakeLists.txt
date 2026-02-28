cmake_minimum_required(VERSION 3.10)
project(json_module LANGUAGES CXX)

file(GLOB JSON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
set(JSON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(json STATIC ${JSON_SRC})
target_include_directories(json PUBLIC ${JSON_INCLUDE_DIR})

set_target_properties(json PROPERTIES
	OUTPUT_NAME "json"
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)