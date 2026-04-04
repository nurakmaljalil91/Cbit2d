---
name: resource-and-asset-management
description: Structure asset loading, caching, and resource ownership for a reusable game engine. Use when Codex is designing or reviewing asset managers, file loading workflows, cache lifetimes, resource handles, or engine boundaries around textures, audio, fonts, and data files.
---

# Resource and Asset Management

Keep resource loading centralized and predictable.

## Follow these principles

- Centralize asset loading.
- Avoid duplicate loads of the same resource.
- Make ownership and lifetime obvious.
- Provide graceful failure paths for missing assets.
- Support hot reload later when practical, especially for shaders and textures during development.

## Assign clear responsibilities

- Let `AssetManager` coordinate loading and caching.
- Let resource loaders handle file-format specifics.
- Use handles or IDs if pointer ownership becomes messy.
- Keep asset APIs simple for gameplay programmers.

## Cover common resource categories

- Textures
- Sprite sheets
- Fonts
- Audio
- Data files such as JSON, YAML, or custom config
- Shader files when a shader pipeline is used

## Avoid these anti-patterns

- Loading files directly inside gameplay systems
- Recreating textures repeatedly
- Hiding expensive loads in unexpected places
- Mixing resource ownership into unrelated systems
