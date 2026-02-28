add_library(enderman_json_plugin INTERFACE)
add_library(enderman::json ALIAS enderman_json_plugin)

target_include_directories(enderman_json_plugin
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(enderman_json_plugin INTERFACE enderman_json::enderman_json)

install(TARGETS enderman_json_plugin
    EXPORT endermanTargets
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
