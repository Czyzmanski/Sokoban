cmake_minimum_required(VERSION 3.0)
project(Sokoban C)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

set_property(GLOBAL PROPERTY RULE_MESSAGES OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 11)
include_directories(src)

set(SOURCE_FILES
        src/board.h
        src/command.h
        src/game.c
        src/game.h
        src/move.h
        src/move_node.h
        src/move_stack.h
        src/position.h
        src/position_node.h
        src/position_queue.h
        src/row.h
        src/sokoban_main.c
        src/squares.h)

add_executable(sokoban ${SOURCE_FILES})
