cmake_minimum_required(VERSION 3.10)

project(enderman-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(http 4.2.3 EXACT REQUIRED)

option(ENDERMAN_PLUGIN_STANDARD_BODIES "Enable standard bodies plugin" ON)

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

target_link_libraries(enderman PUBLIC http::http)


if(ENDERMAN_PLUGIN_STANDARD_BODIES)
  add_subdirectory(plugins/standard_bodies)
  target_link_libraries(enderman PUBLIC enderman_standard_bodies)
endif()



include(GNUInstallDirs)

install(TARGETS enderman
  EXPORT endermanTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${ENDERMAN_INCLUDE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT endermanTargets
  FILE endermanTargets.cmake
  NAMESPACE enderman::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/enderman
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