cmake_minimum_required(VERSION 3.10)
project(json_module LANGUAGES CXX)

file(GLOB JSON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
set(JSON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(enderman_json STATIC ${JSON_SRC})
target_include_directories(enderman_json
    PUBLIC
    $<BUILD_INTERFACE:${JSON_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(enderman_json PROPERTIES
    OUTPUT_NAME "enderman_json"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)


include(GNUInstallDirs)

install(TARGETS enderman_json
    EXPORT endermanJsonTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT endermanJsonTargets
	FILE endermanJsonConfig.cmake
	NAMESPACE enderman_json::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/enderman_json
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/endermanJsonConfigVersion.cmake"
	VERSION 1.0.0
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/endermanJsonConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/endermanJsonConfig.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/enderman_json	
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/endermanJsonConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/endermanJsonConfigVersion.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/enderman_json
)