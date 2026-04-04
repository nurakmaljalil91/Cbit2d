---
name: entt-ecs-patterns
description: Apply disciplined EnTT ECS design for gameplay and engine runtime systems. Use when Codex is creating components, structuring systems, managing entity lifetime, or reviewing ECS code for ownership, update order, and boundary issues.
---

# ECS Rules with EnTT

Use EnTT as the gameplay and runtime data model, but keep usage disciplined.

## Keep components simple

- Keep components mostly plain data.
- Avoid storing ownership-heavy resources directly in components unless carefully justified.
- Keep components focused and small.
- Prefer tags for marker state.
- Separate transient runtime state from serialized gameplay state where useful.

## Keep systems focused

- Put behavior in systems.
- Make systems operate on clear component sets.
- Avoid giant god systems.
- Keep update order explicit.
- Be careful with cross-system side effects.

## Manage entity lifetime carefully

- Avoid long-lived raw references to components.
- Be cautious when storing entity handles.
- Centralize entity creation patterns when scenes become complex.
- Define clear ownership for prefabs, spawned entities, and scene teardown.

## Use EnTT pragmatically

- Use views and groups when they improve clarity or performance.
- Avoid overusing events everywhere.
- Use context and singletons sparingly for shared services or frame state.
- Keep ECS-friendly boundaries with rendering and input systems.
