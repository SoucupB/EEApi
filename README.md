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
* Terrain
* Diplomacy

All information is exposed through a relatively clean C++ interface.

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
* Stop

Commands are synchronized with the game simulation to preserve deterministic behavior.

---

## Versions

The engine is compatible with version v1.00.2xxx (Probably most of the GOG versions) of Empire Earth Art of the conquest. Since its main state source code
did not receive any significant updates over the years, it is unlikely that the AI engine will break the game.

---

## Event System

Receive callbacks whenever something interesting happens.

Examples:

* Unit created
* Unit destroyed
* Building finished

Plugins only need to implement the events they care about.

---

## AI Framework

Create bots by implementing a small interface.

```cpp
/*
 * A method that will execute on each frame. The overall game state will not change until this method
 * finish its execution.
 */
void bt_OnFrame() {
  
}

/*
 * A method that will execute on the first game state. When this method executes, all the initial units
 * are available on things like "vector<Unit> unit_Filter(uint8_t (*method)(Unit));"
 */
void bt_OnInit() {

}

/*
 * A method that will execute on the when a unit is destroyed. The responsability of having a consistent state
 * regarding the implementation of the Bot needs to be implemented by the enginer user. In other words, if you issue
 * commands to a unit that does no longer exist anymore, it will result in "undefined" behaviour.
 * This method is not called only when a unit dies, it might happen with other events.
 */
void bt_OnUnitDestroy(Unit unit) {

}

/*
 * A method that will execute when a unit is created.
 */
void bt_OnUnitCreate(Unit unit) {
  
}

/*
 * This method executes when the game loads. This method should used to free and initialize different datastructures
 * that the developer will use to code behaviour. Please note that at this state of initialization there are not units
 * initialized
 */
void bt_OnGamePrepare() {
  
}
```

The engine handles initialization, synchronization, and communication with the game.

---

## Intended Applications

* RTS AI research
* Autonomous agents
* Reinforcement learning
* Strategy analysis
* Build-order optimization
* Educational projects
* External tools

---

## Design Goals

* Deterministic behavior
* High performance
* Stable API
* Easy plugin development
* Minimal overhead
* Extensive documentation
* Versioned interfaces

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

## Future Plans

* Command queues
* Rendering hooks
* Replay parser
* AI helper library
* Debug renderer
* Logging
* Unit/Integration tests

---

## Philosophy

This engine is still in its infancy and will probably be that way for awhile and the design might not be very consistent everywhere.

The main language is C style C++. I used C++ for it for the only reason that I needed the STL containers in order to implement and test the ideas fast.

This project aims to do for **Empire Earth** what BWAPI did for StarCraft that will interface between the game engine and external programs.

Instead of reverse-engineering game memory for every new project, developers can build against an API focused on gameplay concepts rather than implementation details.

Whether you're building a competitive RTS bot or an experimenting with machine learning, the engine provides a common foundation for interacting with Empire Earth.

---

## Status

🚧 Early development.

The API is under active design and subject to change as additional game systems are reverse-engineered and documented.

Contributions, testing, and feedback are welcome.
