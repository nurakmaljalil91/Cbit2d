---
name: sdl3-rendering
description: Design and implement SDL3-backed rendering and platform lifecycle code with clean engine boundaries. Use when Codex is working on SDL3 windowing, renderer ownership, input/event flow, render abstractions, batching, or graphics-facing engine systems.
---

# SDL3 Rendering Guidelines

Use SDL3 for platform integration, window creation, input handling, and graphics setup.

## Keep SDL responsibilities clean

- Do not mix gameplay logic with low-level SDL calls.
- Centralize SDL lifecycle management.
- Keep window and context code separate from renderer code.
- Poll input in a dedicated input layer or application loop.
- Make engine systems consume normalized input state instead of raw SDL events unless justified.

## Build a reusable rendering layer

- Build a reusable 2D renderer abstraction over the chosen SDL3 graphics path.
- Support sprite drawing, texture regions, transforms, tinting, layers, and camera transforms.
- Design rendering APIs so gameplay code submits intent instead of low-level draw details.
- Minimize state changes and batch when possible.

## Keep ownership boundaries obvious

- Let `Window` own SDL window lifecycle.
- Let `Renderer` own rendering backend lifecycle.
- Let `Texture` wrap GPU texture and resource lifetime.
- Let `RenderQueue` or an equivalent collect draw requests.
- Let `Camera2D` provide view and projection logic.
