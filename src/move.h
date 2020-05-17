#ifndef MOVE_H
#define MOVE_H

#include "position.h"

/* Definitions of possible push directions. */
#define DOWN '2'
#define UP '8'
#define LEFT '4'
#define RIGHT '6'

struct Move {
    int chestNum;
    Position *prevPlayerPos;
};

typedef struct Move Move;

static inline Move *getNewMove(int chestNum, Position *prevPlayerPos) {
    Move *move = malloc(sizeof(Move));
    assert(move != NULL);
    move->chestNum = chestNum;
    move->prevPlayerPos = prevPlayerPos;
    return move;
}

static inline void disposeMove(Move *move) {
    free(move->prevPlayerPos);
    free(move);
}

#endif // MOVE_H
