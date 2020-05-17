#ifndef COMMAND_H
#define COMMAND_H

#include "move.h"

#define UNDO_COMMAND '0'

struct PushCommand {
    int chestNum;
    char direction;
};

typedef struct PushCommand PushCommand;

static inline int getPushedChestRowNumber(int row, char pushDirection) {
    if (pushDirection == DOWN) {
        return row + 1;
    }
    else if (pushDirection == UP) {
        return row - 1;
    }
    else {
        return row;
    }
}

static inline int getPushedChestColNumber(int col, char pushDirection) {
    if (pushDirection == LEFT) {
        return col - 1;
    }
    else if (pushDirection == RIGHT) {
        return col + 1;
    }
    else {
        return col;
    }
}

#endif // COMMAND_H
