cmake_minimum_required(VERSION 4.2)
project(Cbit2dSDL3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    string(REPLACE "/Zc:__cplusplus" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(simdjson CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    get_target_property(SPDLOG_HEADER_ONLY_OPTIONS spdlog::spdlog_header_only INTERFACE_COMPILE_OPTIONS)
    if(SPDLOG_HEADER_ONLY_OPTIONS)
        list(REMOVE_ITEM SPDLOG_HEADER_ONLY_OPTIONS "/Zc:__cplusplus")
        set_target_properties(
            spdlog::spdlog_header_only
            PROPERTIES
            INTERFACE_COMPILE_OPTIONS "${SPDLOG_HEADER_ONLY_OPTIONS}"
        )
    endif()
endif()

add_executable(
    Cbit2dSDL3
    apps/sandbox/main.cpp
    apps/sandbox/scenes/menu_scene.cpp
    apps/sandbox/scenes/play_scene.cpp
    src/assets/tile_map.cpp
    src/core/application.cpp
    src/ecs/drag_system.cpp
    src/ecs/entity_component_system.cpp
    src/ecs/game_object.cpp
    src/ecs/sprite_animation_system.cpp
    src/ecs/sprite_render_system.cpp
    src/ecs/ui_system.cpp
    src/core/input.cpp
    src/core/logger.cpp
    src/core/scene_manager.cpp
)

target_include_directories(Cbit2dSDL3 PRIVATE include)

target_link_libraries(
    Cbit2dSDL3
    PRIVATE
    SDL3::SDL3
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
    $<IF:$<TARGET_EXISTS:SDL3_ttf::SDL3_ttf>,SDL3_ttf::SDL3_ttf,SDL3_ttf::SDL3_ttf-static>
    EnTT::EnTT
    fmt::fmt-header-only
    glm::glm
    spdlog::spdlog_header_only
)

if(MINGW)
    target_compile_options(Cbit2dSDL3 PRIVATE -USPDLOG_FWRITE_UNLOCKED)
endif()
