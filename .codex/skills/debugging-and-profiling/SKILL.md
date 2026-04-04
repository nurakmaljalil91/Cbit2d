---
name: debugging-and-profiling
description: Improve observability, debugging, and runtime performance diagnostics for engine code. Use when Codex is adding logs, assertions, debug overlays, frame timing, subsystem metrics, or profiling-driven optimization guidance.
---

# Debugging and Profiling

## Use logging deliberately

- Use `spdlog` consistently.
- Add clear categories such as engine, renderer, ECS, assets, and gameplay.
- Prefer structured, useful logs over noisy logs.
- Log startup configuration and major subsystem initialization.

## Make failures easy to diagnose

- Add assertions for invalid state in development builds.
- Validate resource loading and file paths.
- Surface SDL errors clearly.
- Make rendering failures easy to trace.
- Add debug overlays when useful for FPS, entity counts, and draw calls.

## Measure before optimizing

- Measure frame time.
- Measure system update time.
- Track draw calls, batches, and loaded resource counts.
- Optimize based on profiling instead of guessing.
- Keep hot paths allocation-light.
