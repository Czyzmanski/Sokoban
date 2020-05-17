#include "game.h"
#include "move_stack.h"

void findChestsPositions(Game *game) {
    for (int i = 0; i < game->board->size; i++) {
        for (int j = 0; j < game->board->rows[i]->size; j++) {
            Position *pos = getNewPosition(i, j);
            char square = getSquare(game, pos);
            if (isChestSquare(square)) {
                int chestNum = getChestNum(square);
                game->chestsPos[chestNum] = pos;
            }
            else {
                free(pos);
            }
        }
    }
}

void findPlayerPosition(Game *game, Position *playerPos) {
    bool isFound = false;
    for (int i = 0; i < game->board->size && !isFound; i++) {
        for (int j = 0; j < game->board->rows[i]->size && !isFound; j++) {
            Position pos;
            pos.row = i;
            pos.col = j;
            if (isPlayerSquare(getSquare(game, &pos))) {
                isFound = true;
                playerPos->row = i;
                playerPos->col = j;
            }
        }
    }
}

void markSquareVisited(Game *game, Position *pos) {
    if (getSquare(game, pos) == BLANK_SQUARE) {
        setSquare(game, pos, VISITED_BLANK_SQUARE);
    }
    else if (getSquare(game, pos) == FINAL_BLANK_SQUARE) {
        setSquare(game, pos, VISITED_FINAL_BLANK_SQUARE);
    }
    else if (getSquare(game, pos) == PLAYER_SQUARE) {
        setSquare(game, pos, VISITED_PLAYER_SQUARE);
    }
    else if (getSquare(game, pos) == FINAL_PLAYER_SQUARE) {
        setSquare(game, pos, VISITED_FINAL_PLAYER_SQUARE);
    }
}

void unmarkSquareIfVisited(Game *game, Position *pos) {
    if (getSquare(game, pos) == VISITED_BLANK_SQUARE) {
        setSquare(game, pos, BLANK_SQUARE);
    }
    else if (getSquare(game, pos) == VISITED_FINAL_BLANK_SQUARE) {
        setSquare(game, pos, FINAL_BLANK_SQUARE);
    }
    else if (getSquare(game, pos) == VISITED_PLAYER_SQUARE) {
        setSquare(game, pos, PLAYER_SQUARE);
    }
    else if (getSquare(game, pos) == VISITED_FINAL_PLAYER_SQUARE) {
        setSquare(game, pos, FINAL_PLAYER_SQUARE);
    }
}

void unmarkVisitedSquares(Game *game) {
    for (int i = 0; i < game->board->size; i++) {
        for (int j = 0; j < game->board->rows[i]->size; j++) {
            Position pos;
            pos.row = i;
            pos.col = j;
            unmarkSquareIfVisited(game, &pos);
        }
    }
}

/* TargetPlayerPosition is the position where player have to go
 * in order to execute push command. */
void initTargetPlayerPosition(Game *game, PushCommand *pushComm,
                              Position *targetPlayerPos) {
    Position *chestToPushPos = getChestPosition(game, pushComm->chestNum);

    int targetRow = chestToPushPos->row;
    if (pushComm->direction == DOWN) {
        targetRow--;
    }
    else if (pushComm->direction == UP) {
        targetRow++;
    }

    int targetCol = chestToPushPos->col;
    if (pushComm->direction == LEFT) {
        targetCol++;
    }
    else if (pushComm->direction == RIGHT) {
        targetCol--;
    }

    targetPlayerPos->row = targetRow;
    targetPlayerPos->col = targetCol;
}

/* Computes and initializes coordinates of position where chest
 * will be pushed if move is possible. */
void initTargetChestPosition(Game *game, PushCommand *pushComm,
                             Position *targetChestPos) {
    Position *chestToPushPos = getChestPosition(game, pushComm->chestNum);

    int targetRow = chestToPushPos->row;
    if (pushComm->direction == DOWN) {
        targetRow++;
    }
    else if (pushComm->direction == UP) {
        targetRow--;
    }

    int targetCol = chestToPushPos->col;
    if (pushComm->direction == LEFT) {
        targetCol--;
    }
    else if (pushComm->direction == RIGHT) {
        targetCol++;
    }

    targetChestPos->row = targetRow;
    targetChestPos->col = targetCol;
}

void executeUndoCommand(Game *game, MoveStack *stack) {
    Move *pastMove = pop(stack);
    Position *currChestPos = getChestPosition(game, pastMove->chestNum);
    Position *currPlayerPos = game->playerPos;
    Position *pastPlayerPos = pastMove->prevPlayerPos;

    setCurrentChestSquareToBlankSquare(game, currChestPos);
    setCurrentPlayerSquareToChestSquare(game, currPlayerPos, pastMove->chestNum);
    setCurrentBlankSquareToPlayerSquare(game, pastPlayerPos);

    currChestPos->row = currPlayerPos->row;
    currChestPos->col = currPlayerPos->col;

    currPlayerPos->row = pastPlayerPos->row;
    currPlayerPos->col = pastPlayerPos->col;

    disposeMove(pastMove);
}

void executePushCommand(Game *game, PushCommand *pushComm, MoveStack *stack) {
    Position *currPlayerPos = game->playerPos;
    Position *currChestPos = getChestPosition(game, pushComm->chestNum);

    setCurrentPlayerSquareToBlankSquare(game, currPlayerPos);
    setCurrentChestSquareToPlayerSquare(game, currChestPos);

    push(stack, getNewMove(pushComm->chestNum,
                           getNewPosition(currPlayerPos->row, currPlayerPos->col)));

    currPlayerPos->row = currChestPos->row;
    currPlayerPos->col = currChestPos->col;

    currChestPos->row = getPushedChestRowNumber(currChestPos->row,
                                                pushComm->direction);
    currChestPos->col = getPushedChestColNumber(currChestPos->col,
                                                pushComm->direction);

    setCurrentBlankSquareToChestSquare(game, currChestPos, pushComm->chestNum);
}

bool doesPathExist(Game *game, Position *targetPlayerPos) {
    PositionQueue queue;
    initPositionQueue(&queue);

    Position *playerPos = game->playerPos;
    Position *copiedPlayerPos = getNewPosition(playerPos->row, playerPos->col);

    bool isPathFound = false;
    addPositionIfLegal(game, copiedPlayerPos, &queue);

    while (!isPositionQueueEmpty(&queue) && !isPathFound) {
        Position *currPos = popFront(&queue);
        if (arePositionsEqual(currPos, targetPlayerPos)) {
            isPathFound = true;
        } else {
            addNeighborsIfLegal(game, currPos, &queue);
        }
        free(currPos);
    }

    clearPositionQueue(&queue);
    unmarkVisitedSquares(game);

    return isPathFound;
}
