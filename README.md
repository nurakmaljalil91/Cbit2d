# Cbit2dSDL3

`Cbit2dSDL3` is the early foundation of a custom 2D game engine/library for the broader `Cbit2D` ecosystem.

This repository is intended to become a reusable engine module that can be consumed by separate game projects, not the final game application itself. In the larger setup, this repo is expected to live as a submodule inside a parent `Cbit2D` repository alongside one or more game repositories that depend on it.

## Goals

- Build a reusable 2D engine/library
- Keep engine code separate from game-specific code
- Use SDL3 as the main low-level graphics/platform layer
- Provide a clean foundation for runtime systems, tooling, and editor features
- Support a lightweight local executable for engine validation and testing

## Planned Core Dependencies

- `SDL3` for windowing, rendering, input, and platform abstraction
- `spdlog` for logging
- `entt` for ECS
- `glm` for math
- `simdjson` for JSON parsing
- `ImGui` for editor and tooling UI

More libraries can be added later if they are justified by engine needs.

## Current State

Right now the project is still minimal:

- CMake-based build
- Dependencies discovered through `find_package(...)` and resolved locally through CLion's `vcpkg` toolchain
- A simple executable target used for testing/bootstrap purposes

That executable should be treated as a temporary validation app, not the long-term primary output of the repository.

## Build Workflow

Primary local development is done with CLion on Windows using its bundled CMake and Ninja tools plus the CLion-managed `vcpkg` toolchain.

Typical CLion configure command:

```powershell
C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=C:/Users/User/AppData/Local/Programs/CLion/bin/ninja/win/x64/ninja.exe -DCMAKE_TOOLCHAIN_FILE=C:\Users\User\.vcpkg-clion\vcpkg\scripts\buildsystems\vcpkg.cmake -G Ninja -S C:\Users\User\Developments\Cbit2dSDL3 -B C:\Users\User\Developments\Cbit2dSDL3\cmake-build-debug
```

Typical local build directory:

```text
C:\Users\User\Developments\Cbit2dSDL3\cmake-build-debug
```

Toolchain file used locally:

```text
C:\Users\User\.vcpkg-clion\vcpkg\scripts\buildsystems\vcpkg.cmake
```

After build, any produced executable in the build folder is mainly for:

- smoke testing
- runtime verification
- rendering checks
- editor/tooling experiments

## Recommended Direction

As the project grows, the engine should move toward:

- a real library target for the engine core
- a small sandbox/test application that links against the library
- separated runtime, editor, and test code
- explicit public API boundaries
- clean integration with an external game repository

## Proposed Repository Layout

This is a practical target layout for the next phase of the project:

```text
Cbit2dSDL3/
  CMakeLists.txt
  README.md
  AGENTS.md
  cmake/
  vendors/
  include/
    cbit/
      core/
      math/
      ecs/
      graphics/
      assets/
      scene/
      io/
      editor/
  src/
    core/
    math/
    ecs/
    graphics/
    assets/
    scene/
    io/
    editor/
  apps/
    sandbox/
  tests/
  docs/
```

Suggested responsibilities:

- `include/cbit/` holds public headers intended for consumers of the engine library
- `src/` holds engine implementation
- `apps/sandbox/` holds a lightweight test or demo executable
- `tests/` holds automated tests as the project matures
- `docs/` holds architecture notes, conventions, and subsystem documentation
- `cmake/` holds custom CMake modules and helper scripts

## Suggested Module Breakdown

Reasonable early engine modules:

- `core` for application lifecycle, configuration, service bootstrapping, time, and platform abstractions
- `graphics` for SDL-backed rendering, textures, camera basics, and render pipeline helpers
- `ecs` for `entt` integration and world/entity orchestration
- `math` for `glm` wrappers, transforms, and common math utilities
- `assets` for resource loading and asset management
- `io` for JSON and file loading
- `scene` for scene composition and runtime coordination
- `editor` for ImGui tooling and debug/editor windows

## Integration Model

Long-term, the larger setup can look like this:

```text
Cbit2D/
  engine/        -> this repository as a submodule
  game/          -> a separate game repository/submodule
```

The `game` side should depend on the engine's public API, while this repository stays focused on engine concerns rather than game-specific content.

## Near-Term Priorities

Good next steps for the codebase:

1. Split the current executable bootstrap from future engine library code.
2. Introduce an actual engine library target in CMake.
3. Create `include/` and `src/` directories with a small `core` module.
4. Add dependency wiring for logging, math, ECS, JSON, and editor tooling incrementally.
5. Keep the sandbox executable thin and use it only to exercise the engine.

## Notes

- Prefer designing this repository as a reusable library first.
- Avoid putting game-specific rules into the engine core.
- Keep CLion compatibility and the existing CMake/Ninja workflow intact unless there is a clear reason to change them.
