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
set(SDL2_IMAGE_PATH vendors/SDL_Image)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SDL2_IMAGE_PATH})
include_directories(${SDL2_IMAGE_PATH}/include)
link_directories(${SDL2_IMAGE_PATH}/lib)
file(COPY ${SDL2_IMAGE_PATH}/bin/SDL2_image.dll DESTINATION ${CMAKE_BINARY_DIR})

# Create a shared library
add_library(Cbit2d SHARED
        src/core/Application.cpp
        src/core/Application.h
        src/utilities/Logger.cpp
        src/utilities/Logger.h
        src/core/Scene.cpp
        src/core/Scene.h
        src/core/SceneManager.cpp
        src/core/SceneManager.h)

# Create an executable
add_executable(Cbit2dApp application/src/main.cpp
        src/core/Application.cpp
        src/core/Application.h
        src/utilities/Logger.cpp
        src/utilities/Logger.h
        src/core/Scene.cpp
        src/core/Scene.h
        src/core/SceneManager.cpp
        src/core/SceneManager.h
        application/src/scenes/CustomScene.cpp
        application/src/scenes/CustomScene.h)

# Link SDL2 library for the library
target_link_libraries(Cbit2d PRIVATE mingw32 SDL2::SDL2main SDL2::SDL2 SDL2_image spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> pthread)

# Link SDL2 library for the executable
target_link_libraries(Cbit2dApp PRIVATE mingw32 SDL2::SDL2main SDL2::SDL2 SDL2_image spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> pthread)

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

# Install header and source files
install(FILES src/core/Application.h src/core/Application.cpp DESTINATION include/cbit2d/core)
install(FILES src/core/Scene.h src/core/Scene.cpp DESTINATION include/cbit2d/core)
install(FILES src/core/SceneManager.h src/core/SceneManager.cpp DESTINATION include/cbit2d/core)
install(FILES src/utilities/Logger.h src/utilities/Logger.cpp DESTINATION include/cbit2d/utilities)

# Export the targets to a script
install(EXPORT Cbit2dTargets
        FILE Cbit2dTargets.cmake
        NAMESPACE Cbit2d::
        DESTINATION lib/cmake/Cbit2d)

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
