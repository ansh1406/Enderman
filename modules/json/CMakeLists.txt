cmake_minimum_required(VERSION 3.10)
project(json_module LANGUAGES CXX)

file(GLOB JSON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
set(JSON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(enderman_json STATIC ${JSON_SRC})
target_include_directories(enderman_json PUBLIC ${JSON_INCLUDE_DIR})

set_target_properties(enderman_json PROPERTIES
	OUTPUT_NAME "enderman_json"
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)