cmake_minimum_required(VERSION 3.10)

project(enderman-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally build modules
option(BUILD_MODULE_JSON "Build the json module and link it with enderman" ON)

file(GLOB_RECURSE ENDERMAN_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Optionally build json module
if(BUILD_MODULE_JSON)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/modules/json/CMakeLists.txt")
    add_subdirectory(modules/json)
    if(TARGET json)
      list(APPEND ENDERMAN_SOURCES $<TARGET_OBJECTS:json_objects>)
      add_custom_target(copy_json_module_to_plugins ALL)
      add_dependencies(copy_json_module_to_plugins json)
      set(PLUGIN_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/plugins/json/lib")
      set(PLUGIN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/plugins/json/include/enderman/json")
      file(MAKE_DIRECTORY ${PLUGIN_LIB_DIR})
      file(MAKE_DIRECTORY ${PLUGIN_INCLUDE_DIR})
      add_custom_command(TARGET copy_json_module_to_plugins POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying $<TARGET_FILE_NAME:json> to ${PLUGIN_LIB_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:json> ${PLUGIN_LIB_DIR}/$<TARGET_FILE_NAME:json>
        COMMAND ${CMAKE_COMMAND} -E echo "Copying json.hpp to ${PLUGIN_INCLUDE_DIR}/json_core.hpp"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/modules/json/include/json/json.hpp ${PLUGIN_INCLUDE_DIR}/json_core.hpp
      )
      list(APPEND ENDERMAN_SOURCES $<TARGET_OBJECTS:json_objects>)
    else()
      message(FATAL_ERROR "json target not found. Ensure the json module is built correctly.")
    endif()
  else()
    message(FATAL_ERROR "modules/json/CMakeLists.txt not found but BUILD_MODULE_JSON=ON")
  endif()
endif()

# Ensure out-of-source builds by default
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  message(WARNING "In-source build detected. It's recommended to build in a separate directory.")
endif()

set(LIB_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})

# First build the http component (expects http/CMakeLists.txt)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/http/CMakeLists.txt")
  add_subdirectory(http)
else()
  message(FATAL_ERROR "http/CMakeLists.txt not found. Ensure the http folder is present.")
endif()

if(TARGET http)
  list(APPEND ENDERMAN_SOURCES $<TARGET_OBJECTS:http_objects>)
else()
  message(FATAL_ERROR "http target not found. Ensure the http component is built correctly.")
endif()

add_library(enderman_objects OBJECT ${ENDERMAN_SOURCES})
target_include_directories(enderman_objects PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/http/include")
target_include_directories(enderman_objects PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(enderman STATIC $<TARGET_OBJECTS:enderman_objects>)

set_target_properties(enderman PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})

message(STATUS "Enderman library output: ${LIB_OUTPUT_DIR}")