add_library(enderman_standard_bodies INTERFACE)
add_library(enderman::standard_bodies ALIAS enderman_standard_bodies)
target_include_directories(enderman_standard_bodies
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS enderman_standard_bodies
    EXPORT endermanTargets
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})