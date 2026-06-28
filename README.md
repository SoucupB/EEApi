# Empire Earth Engine

A modern C++ API and runtime for interacting with **Empire Earth**, designed to provide programmatic access to game state, events, and commands in a way similar to what **BWAPI** provides for StarCraft: Brood War.

The long-term goal of this project is to enable AI research, automation, scripting, tooling, and custom game modifications without modifying gameplay itself.

---

## Features

### Game State Access

Read nearly every aspect of the current game state:

* Units
* Buildings
* Players
* Resources
* Technologies
* Terrain
* Diplomacy
* Fog of war
* Selection
* Camera
* Map information

All information is exposed through a clean C++ interface.

---

## Unit Control

Issue commands to individual units or groups.

Examples include:

* Move
* Attack
* Patrol
* Gather resources
* Construct buildings
* Train units
* Research technologies
* Formations
* Stop
* Hold position

Commands are synchronized with the game simulation to preserve deterministic behavior.

---

## Event System

Receive callbacks whenever something interesting happens.

Examples:

* Match start
* Match end
* Game update
* Unit created
* Unit destroyed
* Unit discovered
* Unit completed
* Building finished
* Technology researched
* Resource exhausted
* Player defeated
* Chat received

Plugins only need to implement the events they care about.

---

## AI Framework

Create bots by implementing a small interface.

```cpp
class MyBot : public AIPlugin
{
public:
    void onStart() override;
    void onFrame() override;
    void onUnitCreate(Unit* unit) override;
    void onUnitDestroy(Unit* unit) override;
};
```

The engine handles initialization, synchronization, and communication with the game.

---

## Replay Support

The engine is intended to support replay analysis, allowing tools to:

* inspect historical game state
* analyze player decisions
* visualize AI behavior
* collect datasets
* benchmark algorithms

---

## Debug Drawing

Draw information directly inside the game.

Examples:

* text
* lines
* circles
* rectangles
* paths
* influence maps
* unit labels

Useful for debugging AI and visualization.

---

## Intended Applications

* RTS AI research
* Autonomous agents
* Reinforcement learning
* Strategy analysis
* Build-order optimization
* Replay analysis
* Tournament bots
* Educational projects
* Custom overlays
* External tools

---

## Design Goals

* Modern C++ interface
* Deterministic behavior
* High performance
* Stable API
* Easy plugin development
* Minimal overhead
* Extensive documentation
* Versioned interfaces
* Cross-version compatibility where possible

---

## Architecture

```
+-------------------------+
|      AI Plugin          |
+-------------------------+
            |
            v
+-------------------------+
|   Empire Earth Engine   |
+-------------------------+
            |
            v
+-------------------------+
|   Game Process Bridge   |
+-------------------------+
            |
            v
+-------------------------+
|     Empire Earth        |
+-------------------------+
```

The engine abstracts away the low-level details of communicating with the game process, exposing a high-level object model to plugins.

---

## Planned Modules

* Core API
* Memory access
* Event dispatcher
* Command queue
* Rendering hooks
* Replay parser
* Map parser
* Pathfinding utilities
* AI helper library
* Debug renderer
* Logging
* Configuration system

---

## Philosophy

This project aims to do for **Empire Earth** what BWAPI did for StarCraft: provide a clean, documented, and extensible interface between the game engine and external programs.

Instead of reverse-engineering game memory for every new project, developers can build against a stable API focused on gameplay concepts rather than implementation details.

Whether you're building a competitive RTS bot, a replay analyzer, or experimenting with machine learning, the engine provides a common foundation for interacting with Empire Earth.

---

## Status

🚧 Early development.

The API is under active design and subject to change as additional game systems are reverse-engineered and documented.

Contributions, testing, and feedback are welcome.
