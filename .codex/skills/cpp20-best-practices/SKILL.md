---
name: cpp20-best-practices
description: Apply modern C++20 engineering guidance for reusable engine and library code. Use when Codex is designing, reviewing, or refactoring C++ engine code and needs guidance on ownership, API boundaries, performance-sensitive patterns, and maintainable language feature usage.
---

# C++20 Engine Best Practices

Use modern C++20 consistently, but do not force features where they reduce clarity.

## Core principles

- Prefer RAII for all resource ownership.
- Prefer `std::unique_ptr` for exclusive ownership.
- Prefer references or raw pointers only for non-owning access.
- Avoid unmanaged global mutable state.
- Keep headers lightweight and reduce compile-time coupling.
- Use `enum class` instead of plain enums.
- Favor value semantics where practical.
- Use `const` aggressively for correctness.

## Use C++20 features deliberately

- Use `concepts` to constrain templates when it improves API clarity.
- Use `std::span` for non-owning array views.
- Use `std::string_view` for read-only string parameters.
- Use `constexpr` and `consteval` where useful.
- Use ranges carefully when readability stays high.
- Use `[[nodiscard]]` for important return values.
- Use `= default` and `= delete` intentionally.
- Use designated initializers only where supported appropriately and readability improves.

## Protect performance and maintainability

- Avoid premature abstraction.
- Avoid virtual dispatch in hot paths unless justified.
- Prefer data-oriented design in ECS and rendering paths.
- Avoid hidden heap allocations in tight loops.
- Keep ownership obvious.
- Document invariants in code comments when needed.

## Keep engine boundaries clean

- Keep engine code reusable and generic.
- Keep game-specific assumptions out of engine modules.
- Keep APIs small and stable.
- Prefer composition over inheritance.
- Fail early with clear logs and assertions in development builds.
