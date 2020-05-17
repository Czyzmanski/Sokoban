#ifndef GAME_H
#define GAME_H

#include "squares.h"
#include "board.h"
#include "position.h"
#include "position_queue.h"
#include "command.h"
#include "move_stack.h"

struct Game {
    Board *board;
    Position *playerPos;
    Position *chestsPos[NUM_OF_CHESTS];
};

typedef struct Game Game;

void findChestsPositions(Game *game);

void findPlayerPosition(Game *game, Position *playerPos);

void markSquareVisited(Game *game, Position *pos);

void unmarkSquareIfVisited(Game *game, Position *pos);

void unmarkVisitedSquares(Game *game);

void initTargetPlayerPosition(Game *game, PushCommand *pushComm,
                              Position *targetPlayerPos);

void initTargetChestPosition(Game *game, PushCommand *pushComm,
                             Position *targetChestPos);

void executeUndoCommand(Game *game, MoveStack *stack);

void executePushCommand(Game *game, PushCommand *pushComm, MoveStack *stack);

bool doesPathExist(Game *game, Position *targetPlayerPos);

static inline char getSquare(Game *game, Position *pos) {
    return game->board->rows[pos->row]->squares[pos->col];
}

static inline void setSquare(Game *game, Position *pos, char newSquare) {
    game->board->rows[pos->row]->squares[pos->col] = newSquare;
}

static inline Position *getChestPosition(Game *game, int chestNum) {
    return game->chestsPos[chestNum];
}

static inline void setCurrentChestSquareToBlankSquare(Game *game,
                                                      Position *currChestPos) {
    if (isFinalChestSquare(getSquare(game, currChestPos))) {
        setSquare(game, currChestPos, FINAL_BLANK_SQUARE);
    }
    else {
        /* getSquare(game, currChestPos) is a non-final chest square. */
        setSquare(game, currChestPos, BLANK_SQUARE);
    }
}

static inline void setCurrentPlayerSquareToChestSquare(Game *game,
                                                       Position *currPlayerPos,
                                                       int chestNum) {
    if (getSquare(game, currPlayerPos) == PLAYER_SQUARE) {
        setSquare(game, currPlayerPos, getChestName(chestNum, PLAYER_SQUARE));
    }
    else {
        /* getSquare(game, currPlayerPos) is equal to FINAL_PLAYER_SQUARE. */
        setSquare(game, currPlayerPos, getChestName(chestNum, FINAL_PLAYER_SQUARE));
    }
}

static inline void setCurrentBlankSquareToPlayerSquare(Game *game,
                                                       Position *pastPlayerPos) {
    if (getSquare(game, pastPlayerPos) == BLANK_SQUARE) {
        setSquare(game, pastPlayerPos, PLAYER_SQUARE);
    }
    else {
        /* getSquare(game, pastPlayerPos) is equal to FINAL_BLANK_SQUARE. */
        setSquare(game, pastPlayerPos, FINAL_PLAYER_SQUARE);
    }
}

static inline void setCurrentPlayerSquareToBlankSquare(Game *game,
                                                       Position *currPlayerPos) {
    if (getSquare(game, currPlayerPos) == PLAYER_SQUARE) {
        setSquare(game, currPlayerPos, BLANK_SQUARE);
    }
    else {
        /* getSquare(game, currPlayerPos) is equal to FINAL_PLAYER_SQUARE. */
        setSquare(game, currPlayerPos, FINAL_BLANK_SQUARE);
    }
}

static inline void setCurrentChestSquareToPlayerSquare(Game *game,
                                                       Position *currChestPos) {
    if (isFinalChestSquare(getSquare(game, currChestPos))) {
        setSquare(game, currChestPos, FINAL_PLAYER_SQUARE);
    }
    else {
        /* getSquare(game, currChestPos) is a non-final chest square. */
        setSquare(game, currChestPos, PLAYER_SQUARE);
    }
}

static inline void setCurrentBlankSquareToChestSquare(Game *game,
                                                      Position *targetChestPos,
                                                      int chestNum) {
    if (getSquare(game, targetChestPos) == BLANK_SQUARE) {
        setSquare(game, targetChestPos, getChestName(chestNum, BLANK_SQUARE));
    }
    else {
        /* getSquare(game, targetChestPos) is equal to FINAL_BLANK_SQUARE. */
        setSquare(game, targetChestPos, getChestName(chestNum, FINAL_BLANK_SQUARE));
    }
}

/* Adds given position to queue if it is not visited yet and can
 * form a valid path. */
static inline void addPositionIfLegal(Game *game, Position *pos,
                                      PositionQueue *queue) {
    if (isPositionInRange(game->board, pos) && isLegalSquare(getSquare(game, pos))) {
        markSquareVisited(game, pos);
        pushBack(queue, pos);
    }
    else {
        free(pos);
    }
}

/* Adds all neighbor positions to queue which are not visited yet
 * and can form a valid path. */
static inline void addNeighborsIfLegal(Game *game, Position *pos,
                                       PositionQueue *queue) {
    Position *upperNeighbor = getNewPosition(pos->row - 1, pos->col);
    addPositionIfLegal(game, upperNeighbor, queue);

    Position *rightNeighbor = getNewPosition(pos->row, pos->col + 1);
    addPositionIfLegal(game, rightNeighbor, queue);

    Position *downNeighbor = getNewPosition(pos->row + 1, pos->col);
    addPositionIfLegal(game, downNeighbor, queue);

    Position *leftNeighbor = getNewPosition(pos->row, pos->col - 1);
    addPositionIfLegal(game, leftNeighbor, queue);
}

/* Checks if specified chest can be pushed in given direction. */
static inline bool isChestPushPossible(Game *game, PushCommand *pushComm) {
    Position targetChestPos;
    initTargetChestPosition(game, pushComm, &targetChestPos);
    return isPositionInRange(game->board, &targetChestPos) &&
           isLegalSquare(getSquare(game, &targetChestPos));
}

/* Checks if player can go up to chest in order to push it. */
static inline bool isApproachPossible(Game *game, PushCommand *pushComm) {
    Position targetPlayerPos;
    initTargetPlayerPosition(game, pushComm, &targetPlayerPos);
    return isPositionInRange(game->board, &targetPlayerPos) &&
           isLegalSquare(getSquare(game, &targetPlayerPos)) &&
           doesPathExist(game, &targetPlayerPos);
}

static inline bool isPushCommandPossible(Game *game, PushCommand *pushComm) {
    return isChestPushPossible(game, pushComm) && isApproachPossible(game, pushComm);
}

static inline void disposeGame(Game *game) {
    disposeBoard(game->board);
    disposeChestsPositions(game->chestsPos);
}

#endif // GAME_H
