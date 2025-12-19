cmake_minimum_required(VERSION 3.16)
project(stickman VERSION 1.2)

if(APPLE)
    execute_process(
        COMMAND xcrun --show-sdk-path
        OUTPUT_VARIABLE CMAKE_OSX_SYSROOT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

configure_file(src/stickmanconfig.h.in src/stickmanconfig.h)

# Force SDL to build as static library
set(SDL_SHARED OFF CACHE BOOL "Build a SDL shared library (if available)")
set(SDL_STATIC ON CACHE BOOL "Build a SDL static library (if available)")

# Add SDL subdirectory
add_subdirectory(SDL EXCLUDE_FROM_ALL)

add_executable(stickman)

# Add all source files
target_sources(stickman PRIVATE
    src/box.cpp
    src/collision.cpp 
    src/driver.cpp
    src/float2.cpp
    src/game.cpp
    src/level.cpp
    src/player.cpp
    src/utils.cpp

    src/playerstates/ducking.cpp
    src/playerstates/falling.cpp
    src/playerstates/groundsliding.cpp
    src/playerstates/jumping.cpp
    src/playerstates/standing.cpp
    src/playerstates/wallsliding.cpp

    src/attack.h
    src/box.h
    src/collision.h
    src/direction.h
    src/float2.h
    src/inputmanager.h
    src/level.h
    src/player.h
    src/utils.h
    src/weapon.h

    # Generated header
    ${CMAKE_BINARY_DIR}/src/stickmanconfig.h
)

# Set up include directories
target_include_directories(stickman PRIVATE 
    src
    ${CMAKE_BINARY_DIR}/src
)

# Link SDL3 statically
target_link_libraries(stickman PRIVATE SDL3::SDL3-static)
