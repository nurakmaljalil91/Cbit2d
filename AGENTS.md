# AGENTS.md

## Purpose

This repository contains `Cbit2d`, a C++20 SDL2-based 2D engine plus a sample application. Use this file as the working agreement for code changes in this repo.

## Project Layout

- `src/core/`: engine runtime code such as `Application`, scenes, input, ECS, tile maps, and game objects.
- `src/editor/`: debug/editor functionality gated by `ENABLE_EDITOR`.
- `src/utilities/`: shared utilities such as logging and local machine helpers.
- `application/src/`: sample app entry point and scenes (`MenuScene`, `PlayScene`).
- `application/resources/`: runtime assets copied into the build output.
- `docs/`: changelog and project notes.
- `vendors/`: third-party dependencies expected by CMake.

## Build Context

- Build system: CMake.
- Language level: C++20 in `CMakeLists.txt`.
- Main library target: `Cbit2d`.
- Sample executable target: `Cbit2dApp`.
- Dependency path: vendored dependencies under `vendors/`.
- Editor code is enabled by `-DENABLE_EDITOR=ON` or debug builds that define `ENABLE_EDITOR`.
- The active local workflow uses CLion's bundled CMake, Ninja, and MinGW toolchain with the build directory `cmake-build-debug`.

Configure the stable vendored debug build:

```powershell
C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=C:/Users/User/AppData/Local/Programs/CLion/bin/ninja/win/x64/ninja.exe -G Ninja -S C:\Users\User\Developments\Cbit2d -B C:\Users\User\Developments\Cbit2d\cmake-build-debug
```

Build the vendored debug build:

```powershell
cmake --build cmake-build-debug
```

Run the game executable directly from the debug build directory:

```powershell
C:\Users\User\Developments\Cbit2d\cmake-build-debug\Cbit2dApp.exe
```

Optional install:

```powershell
cmake --build cmake-build-debug --target install
```

## Run Notes

- Run from `cmake-build-debug` so the executable can find `resources/`, `logs/`, the SDL DLLs, and the copied MinGW runtime DLLs.
- `CMakeLists.txt` now copies `libstdc++-6.dll`, `libgcc_s_seh-1.dll`, and `libwinpthread-1.dll` into the build directory for MinGW builds.
- The sample app writes runtime logs to `cmake-build-debug/logs/logfile.log`.
- If the game exits on startup, check the log file first and then verify the `resources/` folder exists under the build directory.

## Coding Expectations

- Preserve the existing style unless a local cleanup is required for correctness.
- Keep changes scoped. Avoid broad refactors unless they are necessary for the task.
- Prefer fixing the engine and sample app in a way that keeps both `Cbit2d` and `Cbit2dApp` building.
- Treat `ENABLE_EDITOR` as a real compile boundary. Editor-only code should stay behind the existing preprocessor guards.
- Do not introduce new external dependencies unless explicitly requested.
- Add comments only where the intent would otherwise be hard to infer.

## Editing Notes

- The worktree is already dirty in `src/editor/DebugMode.cpp` and `src/editor/DebugMode.h`. Treat those edits as user-owned unless the task explicitly targets them.
- Prefer minimal, reviewable patches.
- When touching build logic, verify both target lists and install rules still make sense.
- When touching application startup, remember the sample app currently creates an `Application`, enables debug mode and FPS display, initializes scenes, and runs from `application/src/main.cpp`.

## Validation

When practical, validate with:

- CMake configure succeeds.
- The project builds successfully with `cmake --build cmake-build-debug`.
- `cmake-build-debug/Cbit2dApp.exe` launches successfully.
- Editor-gated changes are tested with `ENABLE_EDITOR=ON`.
- Runtime/resource changes still align with `application/resources/`.

## Change Awareness

Recent repo history in `docs/CHANGELOG.md` shows active work around the editor, debug mode visibility, scene hierarchy, ECS, and input behavior. Be alert for regressions in those areas when changing engine code.
