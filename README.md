# Cbit2d

Cbit2d is a custom 2D game engine built as a shared library, utilizing SDL2 for graphics and input, and spdlog for logging. This library is designed to be lightweight and easy to integrate into your 2D game projects.

## Features

- 2D game engine functionality
- Easy integration with SDL2 for graphics and input handling
- Logging with spdlog for detailed runtime information
- Modular design for extensibility

## Requirements

- CMake 3.28 or higher
- MinGW for compiler (if using Windows)
- A C++17 compatible compiler or higher
- SDL2
- spdlog
- EnTT (for ECS functionality)

## Installation

### 1. Clone the Repository

```sh
git clone https://github.com/nurakmaljalil91/cbit2d.git
cd cbit2d
```

### 2. Build and Install the Library

```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target install
```

or

```sh
cmake -B_builds -DCMAKE_INSTALL_PREFIX="C:\Users\User\Developments\SharkCardGame\vendors\cbit2d"  -G "MinGW Makefiles" -DCMAKE_CXX_STANDARD=20
cmake --build _builds --target install
```

This will build and install the Cbit2d library to the default installation directory.

## Usage

### 1. Include Cbit2d in Your Project

Ensure that your project is set up to find and link against the installed Cbit2d library.

#### `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.28)

project(MyGameProject)

set(CMAKE_CXX_STANDARD 17)

# Set Cbit2d path
set(CBIT2D_PATH /path/to/cbit2d/installation)
include_directories(${CBIT2D_PATH}/include)
link_directories(${CBIT2D_PATH}/lib)

# Set entt path
include_directories(vendors/entt)

# Set spdlog path
set(spdlog_PATH vendors/spdlog)
include_directories(${spdlog_PATH}/include)
link_directories(${spdlog_PATH}/lib)

# Set SDL2 path
set(SDL2_PATH vendors/SDL2)
include_directories(${SDL2_PATH}/include)
link_directories(${SDL2_PATH}/lib)
file(COPY ${SDL2_PATH}/bin/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})

# Find SDL2 package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(MyGame application/src/main.cpp)

target_link_libraries(MyGame PRIVATE Cbit2d SDL2::SDL2main SDL2::SDL2 spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> pthread)

# Set linker flags for console application
set_target_properties(MyGame PROPERTIES
        LINK_FLAGS "-mconsole"
)
```

### 2. Initialize and Use the Library

Ensure that the logger is initialized and used in your application.

#### `main.cpp`

```cpp
#include "Application.h"

int main() {
    Application app;
    app.run();
    return 0;
}
```
## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Author

Nur Akmal bin Jalil - [GitHub](https://github.com/nurakmaljalil91)

### Notes:

- Make sure to replace `/path/to/cbit2d/installation` with the actual path where the Cbit2d library is installed.
- Adjust the paths and configurations according to your actual project structure and requirements.
- Provide additional instructions or details if necessary.

This `README.md` should help users understand how to set up, build, install, and use the Cbit2d shared library in their projects.