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

# Now add the main enderman target (sources are in src/)
# Create a simple target if a CMakeLists for enderman is not provided
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.enderman.cmake")
  include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.enderman.cmake)
else()
  file(GLOB_RECURSE ENDERMAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  )

  file(GLOB_RECURSE ENDERMAN_HTTP_ADAPTER 
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/http/*.cpp
  )

  # Filter out nonexistent globs gracefully
  if(ENDERMAN_SOURCES)
    # Build enderman as a static library
    add_library(enderman STATIC ${ENDERMAN_SOURCES} ${ENDERMAN_HTTP_ADAPTER})
    target_include_directories(enderman PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
    target_include_directories(enderman PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    # Link against http library if it created a target named "http"
    if(TARGET http)
      target_link_libraries(enderman PUBLIC http)
    endif()
    if(BUILD_MODULE_JSON)
      if(TARGET json)
        target_link_libraries(enderman PUBLIC json)
      else()
        message(FATAL_ERROR "json module target not found but BUILD_MODULE_JSON=ON")
      endif()
    endif()
    set_target_properties(enderman PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})
  else()
    message(WARNING "No enderman sources found in src/; skipping library creation.")
  endif()
endif()

# Provide a helpful summary
message(STATUS "HTTP library output: ${LIB_OUTPUT_DIR}")
