# CHANGELOG

## [Unreleased]

### 2024-08-14

- Make editor only show when in debug mode

### 2024-08-13

- Add editor using ImGui

### 2024-08-12

- Refactor the Entity component system to use game objects
- Add tag component to entities
- Add id component to entities

### 2024-08-10

- Fixing the input class to handle key held down events.
- Adding layering to the rendering system.
- Add install library script

### 2024-08-10

- Add clickable and draggable components

### 2024-08-09

- Add multiple sprite component

### 2024-08-07

- Add collision detection for player and walls.

### 2024-07-27

#### Added

- Initial setup for the PlayScene and Input handling.
- Player and enemy entities with basic movement.
- Add a simple collision detection system.
- Add entity component system (ECS) for entity management.
- Add animated sprite component for player and enemy entities.
- Add TileMap component for rendering the background.

#### Changed

- Simplified the input handling code.
- Improved the deltaTime calculation in the main loop.

#### Fixed

- Corrected the deltaTime being zero issue.

## 2024-07-22

#### Added

- Basic game loop with input handling.
- PlayScene setup with player and enemy entities.
- Basic player movement using keyboard input.

#### Changed

- Refactored the Scene class to include background color and BGM handling.

#### Fixed

- Initial issues with player movement and scene transitions.

