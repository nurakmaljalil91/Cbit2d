cmake_minimum_required(VERSION 3.29)

project(Cbit2d VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

# set entt path
include_directories(vendors/entt)

# set spdlog path
set(spdlog_PATH vendors/spdlog)
include_directories(${spdlog_PATH}/include)
link_directories(${spdlog_PATH}/lib)

# Set CMAKE_PREFIX_PATH to include spdlog_DIR
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${spdlog_PATH})

find_package(spdlog REQUIRED)

# Set SDL2 path
set(SDL2_PATH vendors/SDL2)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SDL2_PATH})
include_directories(${SDL2_PATH}/include)
link_directories(${SDL2_PATH}/lib)
file(COPY ${SDL2_PATH}/bin/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})

# Find SDL2 package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Set SDL2_image path
set(SDL2_IMAGE_PATH vendors/SDL_image)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SDL2_IMAGE_PATH})
include_directories(${SDL2_IMAGE_PATH}/include)
link_directories(${SDL2_IMAGE_PATH}/lib)
file(COPY ${SDL2_IMAGE_PATH}/bin/SDL2_image.dll DESTINATION ${CMAKE_BINARY_DIR})

# set SDL_ttf path
set(SDL2_TTF_PATH vendors/SDL_ttf)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SDL2_TTF_PATH})
include_directories(${SDL2_TTF_PATH}/include)
link_directories(${SDL2_TTF_PATH}/lib)
file(COPY ${SDL2_TTF_PATH}/bin/SDL2_ttf.dll DESTINATION ${CMAKE_BINARY_DIR})

# set SDL2_mixer path
set(SDL2_MIXER_PATH vendors/SDL_mixer)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SDL2_MIXER_PATH})
include_directories(${SDL2_MIXER_PATH}/include)
link_directories(${SDL2_MIXER_PATH}/lib)
file(COPY ${SDL2_MIXER_PATH}/bin/SDL2_mixer.dll DESTINATION ${CMAKE_BINARY_DIR})

# Copy all files in the resources folder
file(GLOB RESOURCES_FILES ${CMAKE_SOURCE_DIR}/application/resources/*)
file(COPY ${RESOURCES_FILES} DESTINATION ${CMAKE_BINARY_DIR}/resources)

# Create a shared library
add_library(Cbit2d SHARED
        src/core/Application.cpp
        src/core/Application.h
        src/utilities/Logger.cpp
        src/utilities/Logger.h
        src/core/Scene.cpp
        src/core/Scene.h
        src/core/SceneManager.cpp
        src/core/SceneManager.h
        src/core/Components.h
        src/utilities/LocalMachine.cpp
        src/utilities/LocalMachine.h
)

# Create an executable
add_executable(Cbit2dApp application/src/main.cpp
        src/core/Application.cpp
        src/core/Application.h
        src/utilities/Logger.cpp
        src/utilities/Logger.h
        src/utilities/LocalMachine.cpp
        src/utilities/LocalMachine.h
        src/core/Scene.cpp
        src/core/Scene.h
        src/core/SceneManager.cpp
        src/core/SceneManager.h
        src/core/Components.h
        application/src/scenes/MenuScene.cpp
        application/src/scenes/MenuScene.h
        application/src/scenes/PlayScene.cpp
        application/src/scenes/PlayScene.h
)

# Link SDL2 library for the library
target_link_libraries(Cbit2d PRIVATE mingw32 SDL2::SDL2main SDL2::SDL2 SDL2_image SDL2_ttf SDL2_mixer spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> pthread)

# Link SDL2 library for the executable
target_link_libraries(Cbit2dApp PRIVATE mingw32 SDL2::SDL2main SDL2::SDL2 SDL2_image SDL2_ttf SDL2_mixer spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> pthread)

# Set linker flags for console application
set_target_properties(Cbit2dApp PROPERTIES
        LINK_FLAGS "-mconsole"
)

# Specify the include directories
target_include_directories(Cbit2d PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include/cbit2d>
)

# Set the C++ standard
set_target_properties(Cbit2d PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES)

# Export the targets to a script
install(TARGETS Cbit2d
        EXPORT Cbit2dTargets
        RUNTIME DESTINATION bin           # for DLLs
        LIBRARY DESTINATION lib           # for shared libraries
        ARCHIVE DESTINATION lib)          # for static libraries and import libraries

# Install core header and source files to include/cbit2d/core
install(FILES
        src/core/Application.h
        src/core/Application.cpp
        src/core/Scene.h
        src/core/Scene.cpp
        src/core/SceneManager.h
        src/core/SceneManager.cpp
        src/core/Components.h
        DESTINATION include/cbit2d/core)

# Install core header and source files to include/cbit2d/core
file(GLOB CORE_FILES
        "src/core/*.h"
        "src/core/*.cpp")
install(FILES ${CORE_FILES} DESTINATION include/cbit2d/core)

# Install utilities header and source files to include/cbit2d/utilities
file(GLOB UTILITIES_FILES
        "src/utilities/*.h"
        "src/utilities/*.cpp")
install(FILES ${UTILITIES_FILES} DESTINATION include/cbit2d/utilities)

# Create and install a package configuration file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/Cbit2dConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_LIST_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Cbit2dConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/Cbit2d"
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/Cbit2dConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/Cbit2dConfigVersion.cmake"
        DESTINATION lib/cmake/Cbit2d)
