cmake_minimum_required(VERSION 3.10)

project(enderman-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally build modules
option(BUILD_MODULE_JSON "Build the json module and link it with enderman" ON)

# Optionally build json module
if(BUILD_MODULE_JSON)
  if(EXISTS "${CMAKE_SOURCE_DIR}/modules/json/CMakeLists.txt")
    add_subdirectory(modules/json)
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

file(GLOB_RECURSE ENDERMAN_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

if(TARGET http)
  list(APPEND ENDERMAN_SOURCES $<TARGET_OBJECTS:http_objects>)
else()
  message(FATAL_ERROR "http target not found. Ensure the http component is built correctly.")
endif()

if(BUILD_MODULE_JSON)
  if(TARGET json)
    list(APPEND ENDERMAN_SOURCES $<TARGET_OBJECTS:json_objects>)
  else()
    message(FATAL_ERROR "json target not found even though BUILD_MODULE_JSON=ON")
  endif()
endif()

add_library(enderman STATIC ${ENDERMAN_SOURCES})
target_include_directories(enderman PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(enderman PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})

message(STATUS "HTTP library output: ${LIB_OUTPUT_DIR}")

file(GLOB MODULE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/modules/*)

add_custom_target(copy_module_libs ALL)

foreach(MOD_PATH ${MODULE_DIRS})
  get_filename_component(MOD_NAME ${MOD_PATH} NAME)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/modules/${MOD_NAME}/CMakeLists.txt")
    if(TARGET ${MOD_NAME})
      add_dependencies(copy_module_libs ${MOD_NAME})
      set(PLUGIN_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/plugins/${MOD_NAME}/lib")
      file(MAKE_DIRECTORY ${PLUGIN_LIB_DIR})
      add_custom_command(TARGET copy_module_libs POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying $<TARGET_FILE_NAME:${MOD_NAME}> to ${PLUGIN_LIB_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${MOD_NAME}> ${PLUGIN_LIB_DIR}/$<TARGET_FILE_NAME:${MOD_NAME}>
      )
    endif()
  endif()
endforeach()
