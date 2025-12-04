# Stickman Game (Temp name lol)

A 2d platformer.

## Building

### Prerequisites
- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.16+

### Clone and Build
```
# Clone with submodules
git clone --recursive https://github.com/shieldsheafson/stickman-game.git
cd stickman-game

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make  # or cmake --build . on Windows
```

## Usage
```
# From build directory
./stickman  # Linux/Mac
# or
stickman.exe  # Windows
```

Arrow keys to move and z to jump.

## License
MIT License - see LICENSE file for details

## Plan
1.1 Basic Movement
1.2 Basic Attacks and Enemies
1.3 Combos and Attack Based Movement
1.4 Smart Enemies