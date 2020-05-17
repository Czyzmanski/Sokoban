cmake_minimum_required(VERSION 3.16)
project(Sokoban C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(Sokoban
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
