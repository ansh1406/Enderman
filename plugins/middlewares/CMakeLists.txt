add_library(enderman_middleware INTERFACE)
add_library(enderman::middleware ALIAS enderman_middleware)
target_include_directories(enderman_middleware
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS enderman_middleware
    EXPORT endermanTargets
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
