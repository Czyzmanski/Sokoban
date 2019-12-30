
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define END_OF_DATA '.'

#define GROWTH_FACTOR 2
#define INITIAL_CAPACITY 1

#define NUM_OF_CHESTS 26

#define BLANK_SQUARE '-'
#define VISITED_BLANK_SQUARE '$'
#define TARGET_BLANK_SQUARE '+'
#define VISITED_TARGET_BLANK_SQUARE '&'
#define WALL_SQUARE '#'
#define PLAYER_SQUARE '@'
#define VISITED_PLAYER_SQUARE '!'
#define TARGET_PLAYER_SQUARE '*'
#define VISITED_TARGET_PLAYER_SQUARE '^'

#define UNDO_COMMAND '0'

// Definitions of possible push directions.
#define DOWN '2'
#define UP '8'
#define LEFT '4'
#define RIGHT '6'

bool isPlayerSquare(char square) {
    return square == PLAYER_SQUARE || square == TARGET_PLAYER_SQUARE;
}

bool isTargetChestSquare(char square) {
    return 'A' <= square && square <= 'Z';
}

bool isChestSquare(char square) {
    return ('a' <= square && square <= 'z') || ('A' <= square && square <= 'Z');
}

bool isBlankSquare(char square) {
    return square == BLANK_SQUARE || square == TARGET_BLANK_SQUARE;
}

// Checks if square can form a path to position where player can push chest and
// if chest can be pushed onto square.
bool isLegalSquare(char square) {
    return isBlankSquare(square) || isPlayerSquare(square);
}

int getChestNum(char chestName) {
    if (isTargetChestSquare(chestName)) {
        return chestName - 'A';
    } else {
        return chestName - 'a';
    }
}

char getChestName(int chestNum, char square) {
    if (square == BLANK_SQUARE || square == PLAYER_SQUARE) {
        return 'a' + chestNum;
    } else {
        return 'A' + chestNum;
    }
}

struct Row {
    char *squares;
    int size;
    int capacity;
};

typedef struct Row Row;

void initRow(Row *row) {
    row->size = 0;
    row->capacity = INITIAL_CAPACITY;
    row->squares = malloc(row->capacity * sizeof(char));
    assert(row->squares != NULL);
}

Row *getNewRow() {
    Row *row = malloc(sizeof(Row));
    assert(row != NULL);
    initRow(row);
    return row;
}

void reallocRow(Row *row) {
    row->capacity *= GROWTH_FACTOR;
    row->squares = realloc(row->squares, row->capacity * sizeof(char));
    assert(row->squares != NULL);
}

void addToRow(Row *row, char square) {
    if (row->size == row->capacity) {
        reallocRow(row);
    }
    row->squares[row->size] = square;
    row->size++;
}

void loadLineToRow(Row *row, int c) {
    do {
        addToRow(row, c);
        c = getchar();
    } while (c != '\n');
}

void printRow(Row *row) {
    for (int i = 0; i < row->size; i++) {
        printf("%c", row->squares[i]);
    }
}

void disposeRow(Row *row) {
    free(row->squares);
}

struct Board {
    Row **rows;
    int size;
    int capacity;
};

typedef struct Board Board;

void initBoard(Board *board) {
    board->size = 0;
    board->capacity = INITIAL_CAPACITY;
    board->rows = malloc(board->capacity * sizeof(Row *));
    assert(board->rows != NULL);
}

void reallocBoard(Board *board) {
    board->capacity *= GROWTH_FACTOR;
    board->rows = realloc(board->rows, board->capacity * sizeof(Row *));
    assert(board->rows != NULL);
}

void addToBoard(Board *board, Row *row) {
    if (board->size == board->capacity) {
        reallocBoard(board);
    }
    board->rows[board->size] = row;
    board->size++;
}

void printBoard(Board *board) {
    for (int i = 0; i < board->size; i++) {
        printRow(board->rows[i]);
        printf("\n");
    }
}

void readInitialBoardState(Board *board) {
    int c = getchar();
    while (c != '\n') {
        Row *row = getNewRow();
        loadLineToRow(row, c);
        addToBoard(board, row);
        c = getchar();
    }
}

void disposeBoard(Board *board) {
    for (int i = 0; i < board->size; i++) {
        disposeRow(board->rows[i]);
        free(board->rows[i]);
    }
    free(board->rows);
}

struct Position {
    int row;
    int col;
};

typedef struct Position Position;

Position *getNewPosition(int row, int col) {
    Position *pos = malloc(sizeof(Position));
    assert(pos != NULL);
    pos->row = row;
    pos->col = col;
    return pos;
}

bool arePositionsEqual(Position *pos1, Position *pos2) {
    if (pos1 == NULL && pos2 == NULL) {
        return true;
    } else if (pos1 == NULL || pos2 == NULL) {
        return false;
    } else {
        return pos1->row == pos2->row && pos1->col == pos2->col;
    }
}

void initChestsPositions(Position *chestsPos[]) {
    for (int i = 0; i < NUM_OF_CHESTS; i++) {
        chestsPos[i] = NULL;
    }
}

void disposeChestsPositions(Position *chestsPos[]) {
    for (int i = 0; i < NUM_OF_CHESTS; i++) {
        if (chestsPos[i] != NULL) {
            free(chestsPos[i]);
        }
    }
}

struct PushCommand {
    int chestNum;
    char direction;
};

typedef struct PushCommand PushCommand;

struct Game {
    Board *board;
    Position *playerPos;
    Position *chestsPos[NUM_OF_CHESTS];
};

typedef struct Game Game;

char getSquare(Game *game, Position *pos) {
    return game->board->rows[pos->row]->squares[pos->col];
}

void setSquare(Game *game, Position *pos, char newSquare) {
    game->board->rows[pos->row]->squares[pos->col] = newSquare;
}

void disposeGame(Game *game) {
    disposeBoard(game->board);
    disposeChestsPositions(game->chestsPos);
}

Position *getChestPosition(Game *game, int chestNum) {
    return game->chestsPos[chestNum];
}

struct Move {
    int chestNum;
    Position *prevPlayerPos;
};

typedef struct Move Move;

Move *getNewMove(int chestNum, Position *prevPlayerPos) {
    Move *move = malloc(sizeof(Move));
    assert(move != NULL);
    move->chestNum = chestNum;
    move->prevPlayerPos = prevPlayerPos;
    return move;
}

void disposeMove(Move *move) {
    free(move->prevPlayerPos);
    free(move);
}

struct MoveNode {
    Move *move;
    struct MoveNode *next;
};

typedef struct MoveNode MoveNode;

MoveNode *getNewMoveNode(Move *move, MoveNode *next) {
    MoveNode *node = malloc(sizeof(MoveNode));
    assert(node != NULL);
    node->move = move;
    node->next = next;
    return node;
}

void disposeMoveNode(MoveNode *node) {
    disposeMove(node->move);
    free(node);
}

struct MoveStack {
    MoveNode *top;
};

typedef struct MoveStack MoveStack;

void initMoveStack(MoveStack *stack) {
    stack->top = NULL;
}

bool isMoveStackEmpty(MoveStack *stack) {
    return stack->top == NULL;
}

Move *top(MoveStack *stack) {
    return stack->top->move;
}

Move *pop(MoveStack *stack) {
    Move *topMove = top(stack);
    MoveNode *topNode = stack->top;
    stack->top = topNode->next;
    free(topNode);
    return topMove;
}

void push(MoveStack *stack, Move *move) {
    MoveNode *node = getNewMoveNode(move, stack->top);
    stack->top = node;
}

void clearMoveStack(MoveStack *stack) {
    while (!isMoveStackEmpty(stack)) {
        Move *move = pop(stack);
        disposeMove(move);
    }
}

struct PositionNode {
    Position *pos;
    struct PositionNode *next;
};

typedef struct PositionNode PositionNode;

PositionNode *getNewPositionNode(Position *pos, PositionNode *next) {
    PositionNode *newNode = malloc(sizeof(PositionNode));
    assert(newNode != NULL);
    newNode->pos = pos;
    newNode->next = next;
    return newNode;
}

void disposePositionNode(PositionNode *node) {
    free(node->pos);
    free(node);
}

struct PositionQueue {
    PositionNode *front;
    PositionNode *back;
};

typedef struct PositionQueue PositionQueue;

void initPositionQueue(PositionQueue *queue) {
    queue->front = NULL;
    queue->back = NULL;
}

bool isPositionQueueEmpty(PositionQueue *queue) {
    return queue->front == NULL;
}

Position *front(PositionQueue *queue) {
    return queue->front->pos;
}

Position *popFront(PositionQueue *queue) {
    if (queue->front == queue->back) {
        queue->back = NULL;
    }
    Position *frontPos = front(queue);
    PositionNode *frontNode = queue->front;
    queue->front = frontNode->next;
    free(frontNode);
    return frontPos;
}

void pushBack(PositionQueue *queue, Position *pos) {
    PositionNode *node = getNewPositionNode(pos, NULL);
    if (isPositionQueueEmpty(queue)) {
        queue->front = node;
    } else {
        queue->back->next = node;
    }
    queue->back = node;
}

void clearPositionQueue(PositionQueue *queue) {
    while (!isPositionQueueEmpty(queue)) {
        Position *pos = popFront(queue);
        free(pos);
    }
}

void executeUndoCommand(Game *game, MoveStack *stack) {
    Move *pastMove = pop(stack);

    Position *currChestPos = getChestPosition(game, pastMove->chestNum);
    if (isTargetChestSquare(getSquare(game, currChestPos))) {
        setSquare(game, currChestPos, TARGET_BLANK_SQUARE);
    } else {
        // getSquare(game, currChestPos) is non-target chest square.
        setSquare(game, currChestPos, BLANK_SQUARE);
    }

    Position *currPlayerPos = game->playerPos;
    if (getSquare(game, currPlayerPos) == PLAYER_SQUARE) {
        setSquare(game, currPlayerPos,
                  getChestName(pastMove->chestNum, PLAYER_SQUARE));
    } else {
        // getSquare(game, currPlayerPos) is equal to TARGET_PLAYER_SQUARE.
        setSquare(game, currPlayerPos,
                  getChestName(pastMove->chestNum, TARGET_PLAYER_SQUARE));
    }

    Position *pastPlayerPos = pastMove->prevPlayerPos;
    if (getSquare(game, pastPlayerPos) == BLANK_SQUARE) {
        setSquare(game, pastPlayerPos, PLAYER_SQUARE);
    } else {
        // getSquare(game, pastPlayerPos) is equal to TARGET_BLANK_SQUARE.
        setSquare(game, pastPlayerPos, TARGET_PLAYER_SQUARE);
    }

    currChestPos->row = currPlayerPos->row;
    currChestPos->col = currPlayerPos->col;

    currPlayerPos->row = pastPlayerPos->row;
    currPlayerPos->col = pastPlayerPos->col;

    disposeMove(pastMove);
}

int getPushedChestRowNumber(int row, char pushDirection) {
    if (pushDirection == DOWN)
        return row + 1;
    else if (pushDirection == UP)
        return row - 1;
    else
        return row;
}

int getPushedChestColNumber(int col, char pushDirection) {
    if (pushDirection == LEFT)
        return col - 1;
    else if (pushDirection == RIGHT)
        return col + 1;
    else
        return col;
}

void setPlayerSquareToBlankSquare(Game *game) {
    if (getSquare(game, game->playerPos) == PLAYER_SQUARE) {
        setSquare(game, game->playerPos, BLANK_SQUARE);
    } else {
        // getSquare(game, game->playerPos) is equal to TARGET_PLAYER_SQUARE.
        setSquare(game, game->playerPos, TARGET_BLANK_SQUARE);
    }
}

void setChestSquareToPlayerSquare(Game *game, Position *chestPos) {
    if (isTargetChestSquare(getSquare(game, chestPos))) {
        setSquare(game, chestPos, TARGET_PLAYER_SQUARE);
    } else {
        // getSquare(game, chestPos) is a non-target chest square.
        setSquare(game, chestPos, PLAYER_SQUARE);
    }
}

void setBlankSquareToChestSquare(Game *game, Position *chestPos, int chestNum) {
    if (getSquare(game, chestPos) == BLANK_SQUARE) {
        setSquare(game, chestPos, getChestName(chestNum, BLANK_SQUARE));
    } else {
        // getSquare(game, chestPos) is equal to TARGET_BLANK_SQUARE
        setSquare(game, chestPos, getChestName(chestNum, TARGET_BLANK_SQUARE));
    }
}

void executePushCommand(Game *game, PushCommand *pushComm, MoveStack *stack) {
    Position *chestPos = getChestPosition(game, pushComm->chestNum);

    setPlayerSquareToBlankSquare(game);
    setChestSquareToPlayerSquare(game, chestPos);

    push(stack,
         getNewMove(pushComm->chestNum, getNewPosition(game->playerPos->row,
                                                       game->playerPos->col)));

    game->playerPos->row = chestPos->row;
    game->playerPos->col = chestPos->col;

    chestPos->row = getPushedChestRowNumber(chestPos->row, pushComm->direction);
    chestPos->col = getPushedChestColNumber(chestPos->col, pushComm->direction);

    setBlankSquareToChestSquare(game, chestPos, pushComm->chestNum);
}

// TargetPlayerPosition is the position where player have to be to execute
// push command.
void initTargetPlayerPosition(Game *game, PushCommand *pushComm,
                              Position *targetPlayerPos) {
    Position *chestToPushPos = getChestPosition(game, pushComm->chestNum);

    int targetRow = chestToPushPos->row;
    if (pushComm->direction == DOWN) {
        targetRow--;
    } else if (pushComm->direction == UP) {
        targetRow++;
    }

    int targetCol = chestToPushPos->col;
    if (pushComm->direction == LEFT) {
        targetCol++;
    } else if (pushComm->direction == RIGHT) {
        targetCol--;
    }

    targetPlayerPos->row = targetRow;
    targetPlayerPos->col = targetCol;
}

// Computes and initializes coordinates of position where chest will be
// pushed if move is possible.
void initTargetChestPosition(Game *game, PushCommand *pushComm,
                             Position *targetChestPos) {
    Position *chestToPushPos = getChestPosition(game, pushComm->chestNum);

    int targetRow = chestToPushPos->row;
    if (pushComm->direction == DOWN) {
        targetRow++;
    } else if (pushComm->direction == UP) {
        targetRow--;
    }

    int targetCol = chestToPushPos->col;
    if (pushComm->direction == LEFT) {
        targetCol--;
    } else if (pushComm->direction == RIGHT) {
        targetCol++;
    }

    targetChestPos->row = targetRow;
    targetChestPos->col = targetCol;
}

bool isPositionInRange(Board *board, Position *pos) {
    if (pos->row < 0 || pos->row >= board->size) {
        return false;
    } else {
        return 0 <= pos->col && pos->col < board->rows[pos->row]->size;
    }
}

void markSquareVisited(Game *game, Position *pos) {
    if (getSquare(game, pos) == BLANK_SQUARE) {
        setSquare(game, pos, VISITED_BLANK_SQUARE);
    } else if (getSquare(game, pos) == TARGET_BLANK_SQUARE) {
        setSquare(game, pos, VISITED_TARGET_BLANK_SQUARE);
    } else if (getSquare(game, pos) == PLAYER_SQUARE) {
        setSquare(game, pos, VISITED_PLAYER_SQUARE);
    } else if (getSquare(game, pos) == TARGET_PLAYER_SQUARE) {
        setSquare(game, pos, VISITED_TARGET_PLAYER_SQUARE);
    }
}

void unmarkSquareIfVisited(Game *game, Position *pos) {
    if (getSquare(game, pos) == VISITED_BLANK_SQUARE) {
        setSquare(game, pos, BLANK_SQUARE);
    } else if (getSquare(game, pos) == VISITED_TARGET_BLANK_SQUARE) {
        setSquare(game, pos, TARGET_BLANK_SQUARE);
    } else if (getSquare(game, pos) == VISITED_PLAYER_SQUARE) {
        setSquare(game, pos, PLAYER_SQUARE);
    } else if (getSquare(game, pos) == VISITED_TARGET_PLAYER_SQUARE) {
        setSquare(game, pos, TARGET_PLAYER_SQUARE);
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

// Adds given position to queue if it is not visited yet and can form a valid
// path.
void addPositionIfLegal(Game *game, Position *pos, PositionQueue *queue) {
    if (isPositionInRange(game->board, pos) && isLegalSquare(getSquare(game, pos))) {
        markSquareVisited(game, pos);
        pushBack(queue, pos);
    } else {
        free(pos);
    }
}

// Adds all neighbor positions to queue which are not visited yet and can form a
// valid path.
void addNeighborsIfLegal(Game *game, Position *pos, PositionQueue *queue) {
    Position *upperNeighbor = getNewPosition(pos->row - 1, pos->col);
    addPositionIfLegal(game, upperNeighbor, queue);

    Position *rightNeighbor = getNewPosition(pos->row, pos->col + 1);
    addPositionIfLegal(game, rightNeighbor, queue);

    Position *downNeighbor = getNewPosition(pos->row + 1, pos->col);
    addPositionIfLegal(game, downNeighbor, queue);

    Position *leftNeighbor = getNewPosition(pos->row, pos->col - 1);
    addPositionIfLegal(game, leftNeighbor, queue);
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

// Checks if specified chest can be pushed in given direction.
bool isChestPushPossible(Game *game, PushCommand *pushComm) {
    Position targetChestPos;
    initTargetChestPosition(game, pushComm, &targetChestPos);

    return isPositionInRange(game->board, &targetChestPos) &&
           isLegalSquare(getSquare(game, &targetChestPos));
}

// Checks if player can go up to chest in order to push it.
bool isApproachPossible(Game *game, PushCommand *pushComm) {
    Position targetPlayerPos;
    initTargetPlayerPosition(game, pushComm, &targetPlayerPos);

    return isPositionInRange(game->board, &targetPlayerPos) &&
           isLegalSquare(getSquare(game, &targetPlayerPos)) &&
           doesPathExist(game, &targetPlayerPos);
}

bool isPushCommandPossible(Game *game, PushCommand *pushComm) {
    return isChestPushPossible(game, pushComm) &&
           isApproachPossible(game, pushComm);
}

void readAndExecuteCommands(Game *game) {
    MoveStack stack;
    initMoveStack(&stack);

    int c = getchar();
    while (c != END_OF_DATA) {
        if (c == UNDO_COMMAND) {
            if (!isMoveStackEmpty(&stack)) {
                executeUndoCommand(game, &stack);
            }
        } else {
            PushCommand pushComm;
            pushComm.chestNum = getChestNum(c);
            pushComm.direction = getchar();
            if (isPushCommandPossible(game, &pushComm)) {
                executePushCommand(game, &pushComm, &stack);
            }
        }
        printBoard(game->board);
        getchar();
        c = getchar();
    }

    clearMoveStack(&stack);
}

void findChestsPositions(Board *board, Position *chestsPos[]) {
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->rows[i]->size; j++) {
            char square = board->rows[i]->squares[j];
            if (isChestSquare(square)) {
                int chestNum = getChestNum(square);
                chestsPos[chestNum] = getNewPosition(i, j);
            }
        }
    }
}

void findPlayerPosition(Board *board, Position *playerPos) {
    bool isFound = false;
    for (int i = 0; i < board->size && !isFound; i++) {
        for (int j = 0; j < board->rows[i]->size && !isFound; j++) {
            char square = board->rows[i]->squares[j];
            if (isPlayerSquare(square)) {
                isFound = true;
                playerPos->row = i;
                playerPos->col = j;
            }
        }
    }
}

int main() {
    Board board;
    initBoard(&board);
    readInitialBoardState(&board);
    printBoard(&board);

    Position playerPos;
    findPlayerPosition(&board, &playerPos);

    Game game;
    game.board = &board;
    game.playerPos = &playerPos;
    initChestsPositions(game.chestsPos);
    findChestsPositions(&board, game.chestsPos);

    readAndExecuteCommands(&game);

    disposeGame(&game);

    return 0;
}
