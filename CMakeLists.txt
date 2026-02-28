cmake_minimum_required(VERSION 3.10)

project(enderman-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally build modules
option(BUILD_MODULE_JSON "Build the json module and link it with enderman" ON)

find_package(http REQUIRED)

set(ENDERMAN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB_RECURSE ENDERMAN_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(enderman STATIC ${ENDERMAN_SOURCES})

target_include_directories(enderman
  PUBLIC
  $<BUILD_INTERFACE:${ENDERMAN_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(enderman PRIVATE http)


# Optionally build json module
if(BUILD_MODULE_JSON)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/modules/json/CMakeLists.txt")
    add_subdirectory(modules/json)
    if(TARGET json)
      add_custom_target(copy_json_module_to_plugins ALL)
      add_dependencies(copy_json_module_to_plugins json)
      set(PLUGIN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/plugins/json/include/enderman/json")
      file(MAKE_DIRECTORY ${PLUGIN_INCLUDE_DIR})
      add_custom_command(TARGET copy_json_module_to_plugins POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying json.hpp to ${PLUGIN_INCLUDE_DIR}/json_core.hpp"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/modules/json/include/json/json.hpp ${PLUGIN_INCLUDE_DIR}/json_core.hpp
      )
      
      target_link_libraries(enderman PRIVATE json)

    else()
      message(FATAL_ERROR "json target not found. Ensure the json module is built correctly.")
    endif()
  else()
    message(FATAL_ERROR "modules/json/CMakeLists.txt not found but BUILD_MODULE_JSON=ON")
  endif()
endif()


include(GNUInstallDirs)

install(TARGETS enderman
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${ENDERMAN_INCLUDE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/endermanConfigVersion.cmake"
  VERSION 1.0.0
  COMPATIBILITY SameMinorVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/endermanConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/endermanConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/enderman
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/endermanConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/endermanConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/enderman
)