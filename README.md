# 🦊🐰 Renards vs Lapins - Predator-Prey Simulation

A C++ predator-prey simulation featuring foxes and rabbits, rendered via [SFML](https://www.sfml-dev.org/) and built with a modular architecture and extensive unit tests.

![Demo Animation](frames/simulationVideo.gif)

## 📦 Features

- Turn-based simulation of animal behaviors (movement, hunger, reproduction).
- Two species: **Lapins (Rabbits)** and **Renards (Foxes)**.
- SFML-based GUI for real-time interaction.
- Fraises (strawberries) as food for rabbits.
- Unit-tested with [Doctest](https://github.com/doctest/doctest).
- Designed for both Linux and Windows (via WSL/cross-compilation).

## 🗂️ Project Structure
├── main.cpp # CLI Simulation (optional)

├── test.cpp # Unit tests using Doctest

├── sfml/ # SFML-based UI and interaction

│ ├── sfml_main.cpp

│ ├── Menu.cpp

│ └── JeuSFML.cpp

├── coord.cpp # Coordinate and position logic

├── animal.cpp # Animal base logic

├── population.cpp # Population tracking and ID system

├── grille.cpp # Grid management for placing animals

├── jeu.cpp # Game logic and turn engine

├── export.cpp # Frame export and image generation

└── Makefile # Build instructions


## 🚀 Building & Running

### Linux (WSL or native)
- make ./main

- make sfml ./sfml_sim

- make test ./test

- make clean 

## ✅ Dependencies
- C++17

- SFML 2.5+ (Graphics, Window, System)

- Doctest (included via test.cpp)

- For Windows builds: x86_64-w64-mingw32-g++, windres

## 🧠 Design Highlights
- Ensemble is a custom set structure for managing IDs and neighbors.

- Coord encodes 2D positions and neighbor relations.

- Animal tracks species, hunger, and position.

- Population handles unique ID assignment and animal updates.

- Grille maps animals to positions on the 2D grid.

- Jeu coordinates turns, movements, reproduction, and interactions.

## 🖼️ GUI (SFML)
Use the menu to launch a simulation with preconfigured parameters or tweak values in code (soon will be available in menu). 
Animals are drawn, strawberries appear, and the world evolves over time.

## 🧪 Unit Testing
### Over 70 test cases cover:
- Coordinate validity

- Ensemble logic

- Animal behavior

- Population rules

- Grille consistency

- Game engine correctness
