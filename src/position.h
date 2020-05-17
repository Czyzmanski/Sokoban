#ifndef POSITION_H
#define POSITION_H

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define NUM_OF_CHESTS 26

struct Position {
    int row;
    int col;
};

typedef struct Position Position;

static inline Position *getNewPosition(int row, int col) {
    Position *pos = malloc(sizeof(Position));
    assert(pos != NULL);
    pos->row = row;
    pos->col = col;
    return pos;
}

static inline bool arePositionsEqual(Position *pos1, Position *pos2) {
    if (pos1 == NULL && pos2 == NULL) {
        return true;
    } else if (pos1 == NULL || pos2 == NULL) {
        return false;
    } else {
        return pos1->row == pos2->row && pos1->col == pos2->col;
    }
}

static inline void initChestsPositions(Position *chestsPos[]) {
    for (int i = 0; i < NUM_OF_CHESTS; i++) {
        chestsPos[i] = NULL;
    }
}

static inline void disposeChestsPositions(Position *chestsPos[]) {
    for (int i = 0; i < NUM_OF_CHESTS; i++) {
        if (chestsPos[i] != NULL) {
            free(chestsPos[i]);
        }
    }
}

#endif // POSITION_H
