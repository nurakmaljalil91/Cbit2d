---
name: engine-architecture
description: Design reusable 2D engine and library architecture with clean separation between engine and game code. Use when Codex is planning modules, defining subsystem boundaries, splitting runtime and app code, or restructuring a codebase toward maintainable engine architecture.
---

# 2D Engine Architecture

Design the engine as a reusable library with a clean split between engine and game code.

## Structure the engine into clear modules

### Core

- Application
- EngineConfig
- Logger
- Time
- Window
- Input
- Service registry or lightweight subsystem access pattern

### ECS and scene

- Scene or World
- Entity wrapper if useful
- Component registration and helpers
- System execution pipeline
- Scene manager

### Rendering

- Renderer2D
- Texture
- Sprite
- Camera2D
- Render target or framebuffer abstraction if needed
- Tilemap renderer
- Debug renderer

### Resources

- AssetManager
- Texture cache
- Font, shader, and audio resource loaders
- Path helpers and manifest support if needed

### Utilities

- Assertions
- File I/O helpers
- Serialization helpers
- Math helpers

## Keep boundaries explicit

- Keep the engine unaware of specific game scenes, quests, enemies, or rules.
- Keep the game layer dependent on engine APIs, ECS, and rendering services.
- Keep SDL3-specific code near platform and runtime boundaries.
- Keep EnTT usage consistent across engine and game.
