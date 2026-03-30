# Cbit2d

Cbit2d is a C++20 2D game engine and sample application built on SDL2, EnTT, spdlog, GLM, simdjson, and ImGui.

This repository uses vendored dependencies under `vendors/` for the current supported build workflow.

## Project Layout

- `src/`: engine code
- `src/core/`: application, scene, ECS, input, tile map, asset handling
- `src/editor/`: ImGui debug/editor code behind `ENABLE_EDITOR`
- `src/utilities/`: logging and machine helpers
- `application/src/`: sample app entry point and scenes
- `application/resources/`: images, fonts, audio, maps copied into the build output
- `vendors/`: current vendored third-party libraries

## Prerequisites

- CMake 3.29 or newer
- A MinGW GCC toolchain
- Ninja or MinGW Makefiles
- The `vendors/` directory present with SDL2, SDL_image, SDL_ttf, SDL_mixer, EnTT, spdlog, GLM, simdjson, and ImGui

## Build

This is the supported path for building and running the sample game.

### Configure

Using CLion's bundled CMake and Ninja:

```powershell
C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=C:/Users/User/AppData/Local/Programs/CLion/bin/ninja/win/x64/ninja.exe -G Ninja -S C:\Users\User\Developments\Cbit2d -B C:\Users\User\Developments\Cbit2d\cmake-build-debug
```

Or from a shell where `cmake` and `ninja` are already on `PATH`:

```powershell
cmake -S . -B cmake-build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

### Build

```powershell
cmake --build cmake-build-debug
```

### Run

```powershell
C:\Users\User\Developments\Cbit2d\cmake-build-debug\Cbit2dApp.exe
```

### Notes

- The build copies SDL DLLs and MinGW runtime DLLs into `cmake-build-debug`
- Runtime logs go to `cmake-build-debug\logs\logfile.log`
- Runtime assets are copied to `cmake-build-debug\resources`
- The app should be launched from the build output so it can find `resources/` and `logs/`

## Common Problems

### App exits immediately on startup

Check:

- `cmake-build-debug\logs\logfile.log`
- `cmake-build-debug\resources\`
- that the executable is being run from the build output

### `Cbit2dApp.exe` says a DLL is missing

Rebuild first. The project now copies:

- `SDL2.dll`
- `SDL2_image.dll`
- `SDL2_ttf.dll`
- `SDL2_mixer.dll`
- `libstdc++-6.dll`
- `libgcc_s_seh-1.dll`
- `libwinpthread-1.dll`

## Current Recommendation

If your goal is to work on the engine or run the sample app now:

1. Use the vendored workflow in this repository

## License

This project is licensed under the MIT License.
