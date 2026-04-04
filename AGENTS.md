# Cbit2dSDL3 Agent Guide

## Purpose

`Cbit2dSDL3` is intended to become a reusable 2D game engine/library, not a standalone game.

The long-term role of this repository is:

- Provide the core Cbit2D engine/library implementation.
- Build as a library that can be consumed by game projects.
- Serve as one submodule inside a larger `Cbit2D` super-repository.
- Support a separate game/application repository or submodule that depends on this engine.

Any executable produced directly by this repository is primarily for:

- engine smoke tests
- sandbox experiments
- rendering or tooling validation
- editor/runtime diagnostics

It should not be treated as the final shipped game application by default.

## Technology Direction

Current or planned core dependencies:

- `SDL3` for windowing, rendering, platform abstraction, input, and low-level graphics-related services
- `spdlog` for logging
- `entt` for ECS and related runtime patterns
- `glm` for math
- `simdjson` for JSON parsing
- `ImGui` for engine tooling/editor UI

Additional libraries may be introduced later when justified by engine needs.

## Architectural Intent

When making changes, optimize for an engine/library codebase rather than a one-off app.

Preferred direction:

- Keep engine code modular and reusable.
- Separate engine runtime code from test/demo code.
- Separate editor/tooling code from runtime/game-facing code when possible.
- Favor clean public interfaces over tight coupling to temporary sample logic.
- Avoid hardwiring repository assumptions that would make submodule integration difficult.

Likely future structure, even if the repo has not fully reached it yet:

- engine/library source
- test or sample executable
- editor/tooling integration
- asset/data loading systems
- game-facing API surface

## Build Context

Primary local workflow uses CLion on Windows with its bundled CMake/Ninja tools and the CLion-managed `vcpkg` installation.

Expected CLion configure command:

```powershell
C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=C:/Users/User/AppData/Local/Programs/CLion/bin/ninja/win/x64/ninja.exe -DCMAKE_TOOLCHAIN_FILE=C:\Users\User\.vcpkg-clion\vcpkg\scripts\buildsystems\vcpkg.cmake -G Ninja -S C:\Users\User\Developments\Cbit2dSDL3 -B C:\Users\User\Developments\Cbit2dSDL3\cmake-build-debug
```

Common local build directory:

- `C:\Users\User\Developments\Cbit2dSDL3\cmake-build-debug`
- `C:\Users\User\.vcpkg-clion\vcpkg\scripts\buildsystems\vcpkg.cmake`

Common local run behavior:

- CLion builds through the above CMake/Ninja toolchain with the `vcpkg` toolchain file enabled.
- Produced executables are typically run from `cmake-build-debug`.
- If an executable exists in this repo, assume it is for engine validation/testing unless explicitly documented otherwise.

## Implementation Guidance

When editing this repository:

- Prefer introducing a real library target as the project matures.
- Keep demo/test entry points thin and dependent on the library, not the other way around.
- Avoid embedding game-specific rules, data, or content into core engine modules.
- Treat SDL3 as the primary platform/rendering foundation unless a change explicitly expands that abstraction.
- Design systems so they can be consumed cleanly from an external game repository.
- Keep dependency wiring and initialization explicit and maintainable.

## Code Organization Expectations

Preferred direction for future changes:

- place core engine code in dedicated engine/library directories
- place temporary app/test bootstrap code separately
- keep editor-only concerns isolated from runtime-only concerns
- keep third-party code vendored or integrated in clearly bounded locations
- avoid mixing public API headers with unrelated internals without a reason

## Notes For Agents

- Do not assume the current executable is the primary product.
- Do assume this repo is expected to evolve into a reusable engine/library.
- Do prefer changes that help future submodule consumption.
- Do preserve compatibility with the CLion + CMake + Ninja workflow unless there is a clear reason to change it.
- If build/run behavior changes, update this file so the local workflow remains accurate.

## Coding Rules

- Use `cbit2d` as the engine namespace.
- Do not introduce engine code under other root namespaces such as `cbit`.
- Prefix private member variables with `_`.
- Write comments using Doxygen-style conventions so the codebase is ready for Doxygen later.
- Add a Doxygen-style comment block at the top of each method definition.
- For new or edited `.hpp` and `.cpp` files, add a file header comment block at the top of the file.

Required file header format:

```cpp
/**
 * @file    Logger.h
 * @brief   Header file for the Logger class.
 * @details This file contains the definition of the Logger class which is responsible for logging messages to the console and a log file.
 *          The Logger class uses the spdlog library to log messages.
 * @author  Nur Akmal bin Jalil
 * @date    2024-07-27
 */
```

Guidance for that header:

- Keep the same Doxygen-style structure for both header and source files.
- Replace the filename, brief, and details so they match the actual file being edited.
- Preserve the author line as `Nur Akmal bin Jalil` unless you explicitly decide to change the project convention later.
- Update the date to the appropriate file date or project convention when creating or editing files.
- Prefer Doxygen tags such as `@brief`, `@param`, `@return`, and `@details` where they add useful structure.
