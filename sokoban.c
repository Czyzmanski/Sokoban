
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

#define DOWN '2'
#define UP '8'
#define LEFT '4'
#define RIGHT '6'

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

void addToRow(Row *row, char sqr) {
    if (row->size == row->capacity) {
        reallocRow(row);
    }
    row->squares[row->size] = sqr;
    row->size++;
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

// comment on this
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

bool isPlayerSquare(char sqr) {
    return sqr == PLAYER_SQUARE || sqr == TARGET_PLAYER_SQUARE;
}

bool isTargetChestSquare(char sqr) {
    return 'A' <= sqr && sqr <= 'Z';
}

bool isChestSquare(char sqr) {
    return ('a' <= sqr && sqr <= 'z') || ('A' <= sqr && sqr <= 'Z');
}

bool isBlankSquare(char sqr) {
    return sqr == BLANK_SQUARE || sqr == TARGET_BLANK_SQUARE;
}

bool isPathSquare(char sqr) {
    return isBlankSquare(sqr) || isPlayerSquare(sqr);
}

/*
Checks if player can push chest from square sqr or if player
can push chest onto square sqr.
*/
bool isLegalSquare(char sqr) {
    return isBlankSquare(sqr) || isPlayerSquare(sqr);
}

int getChestNum(char chestName) {
    if (isTargetChestSquare(chestName)) {
        return chestName - 'A';
    } else {
        return chestName - 'a';
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

void loadLineToRow(Row *row, int c) {
    do {
        addToRow(row, c);
        c = getchar();
    } while (c != '\n');
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

char getChestName(int chestNum, char square) {
    if (square == BLANK_SQUARE || square == PLAYER_SQUARE) {
        return 'a' + chestNum;
    } else {
        return 'A' + chestNum;
    }
}

void executeUndoCommand(Game *game, MoveStack *stack) {
    Move *pastMove = pop(stack);
    int chestNum = pastMove->chestNum;

    Position *currPlayerPos = game->playerPos;
    Row *currPlayerRow = game->board->rows[currPlayerPos->row];

    Position *currChestPos = game->chestsPos[chestNum];
    Row *currChestRow = game->board->rows[currChestPos->row];

    Position *pastPlayerPos = pastMove->prevPlayerPos;
    Row *pastPlayerRow = game->board->rows[pastPlayerPos->row];

    char *currChestSquare = &(currChestRow->squares[currChestPos->col]);
    if (isTargetChestSquare(*currChestSquare))
        *currChestSquare = TARGET_BLANK_SQUARE;
    else
        *currChestSquare = BLANK_SQUARE;

    char *currPlayerSquare = &(currPlayerRow->squares[currPlayerPos->col]);
    if (*currPlayerSquare == PLAYER_SQUARE)
        *currPlayerSquare = getChestName(chestNum, PLAYER_SQUARE);
    else  // *currPlayerSquare == TARGET_PLAYER_SQUARE
        *currPlayerSquare = getChestName(chestNum, TARGET_PLAYER_SQUARE);

    char *pastPlayerSquare = &(pastPlayerRow->squares[pastPlayerPos->col]);
    if (*pastPlayerSquare == BLANK_SQUARE)
        *pastPlayerSquare = PLAYER_SQUARE;
    else  // *pastPlayerSquare == TARGET_BLANK_SQUARE
        *pastPlayerSquare = TARGET_PLAYER_SQUARE;

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

void executePushCommand(Game *game, PushCommand *pushComm, MoveStack *stack) {
    int chestNum = pushComm->chestNum;

    Position *playerPos = game->playerPos;
    Position *chestPos = game->chestsPos[chestNum];

    Row *playerRow = game->board->rows[playerPos->row];
    Row *chestRow = game->board->rows[chestPos->row];

    Position *copiedPlayerPos = getNewPosition(playerPos->row, playerPos->col);
    Move *pushMove = getNewMove(chestNum, copiedPlayerPos);
    push(stack, pushMove);

    char *playerSquare = &(playerRow->squares[playerPos->col]);
    if (*playerSquare == PLAYER_SQUARE)
        *playerSquare = BLANK_SQUARE;
    else  // *playerSquare == TARGET_PLAYER_SQUARE
        *playerSquare = TARGET_BLANK_SQUARE;

    char *chestSquare = &(chestRow->squares[chestPos->col]);
    if (isTargetChestSquare(*chestSquare))
        *chestSquare = TARGET_PLAYER_SQUARE;
    else
        *chestSquare = PLAYER_SQUARE;

    playerPos->row = chestPos->row;
    playerPos->col = chestPos->col;

    char pushDirection = pushComm->direction;
    chestPos->row = getPushedChestRowNumber(chestPos->row, pushDirection);
    chestPos->col = getPushedChestColNumber(chestPos->col, pushDirection);

    Row *pushedChestRow = game->board->rows[chestPos->row];
    char *pushedChestSquare = &(pushedChestRow->squares[chestPos->col]);
    if (*pushedChestSquare == BLANK_SQUARE)
        *pushedChestSquare = getChestName(chestNum, BLANK_SQUARE);
    else
        *pushedChestSquare = getChestName(chestNum, TARGET_BLANK_SQUARE);
}

// returns position on which player must stand to execute push
Position *getTargetPlayerPosition(Game *game, PushCommand *pushComm) {
    char pushDirection = pushComm->direction;
    Position *chestToPushPos = game->chestsPos[pushComm->chestNum];

    int targetRow = chestToPushPos->row;
    if (pushDirection == DOWN)
        targetRow--;
    else if (pushDirection == UP)
        targetRow++;

    int targetCol = chestToPushPos->col;
    if (pushDirection == LEFT)
        targetCol++;
    else if (pushDirection == RIGHT)
        targetCol--;

    return getNewPosition(targetRow, targetCol);
}

// returns position to which chest will be pushed if push is possible
Position *getTargetChestPosition(Game *game, PushCommand *pushComm) {
    char pushDirection = pushComm->direction;
    Position *chestToPushPos = game->chestsPos[pushComm->chestNum];

    int targetRow = chestToPushPos->row;
    if (pushDirection == DOWN)
        targetRow++;
    else if (pushDirection == UP)
        targetRow--;

    int targetCol = chestToPushPos->col;
    if (pushDirection == LEFT)
        targetCol--;
    else if (pushDirection == RIGHT)
        targetCol++;

    return getNewPosition(targetRow, targetCol);
}

bool isPositionInRange(Board *board, Position *pos) {
    if (pos->row < 0 || pos->row >= board->size) {
        return false;
    } else {
        return 0 <= pos->col && pos->col < board->rows[pos->row]->size;
    }
}

void markSquareVisited(char *sq) {
    if (*sq == BLANK_SQUARE) {
        *sq = VISITED_BLANK_SQUARE;
    } else if (*sq == TARGET_BLANK_SQUARE) {
        *sq = VISITED_TARGET_BLANK_SQUARE;
    } else if (*sq == PLAYER_SQUARE) {
        *sq = VISITED_PLAYER_SQUARE;
    } else if (*sq == TARGET_PLAYER_SQUARE) {
        *sq = VISITED_TARGET_PLAYER_SQUARE;
    }
}

void unmarkSquareIfVisited(char *sq) {
    if (*sq == VISITED_BLANK_SQUARE) {
        *sq = BLANK_SQUARE;
    } else if (*sq == VISITED_TARGET_BLANK_SQUARE) {
        *sq = TARGET_BLANK_SQUARE;
    } else if (*sq == VISITED_PLAYER_SQUARE) {
        *sq = PLAYER_SQUARE;
    } else if (*sq == VISITED_TARGET_PLAYER_SQUARE) {
        *sq = TARGET_PLAYER_SQUARE;
    }
}

void unmarkVisitedSquares(Board *board) {
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->rows[i]->size; j++) {
            char *sq = &(board->rows[i]->squares[j]);
            unmarkSquareIfVisited(sq);
        }
    }
}

// TODO comment
void processPosition(Board *board, Position *pos, PositionQueue *queue) {
    if (isPositionInRange(board, pos)) {
        Row *posRow = board->rows[pos->row];
        char *posSquare = &(posRow->squares[pos->col]);
        if (isPathSquare(*posSquare)) {
            markSquareVisited(posSquare);
            pushBack(queue, pos);
        } else {
            free(pos);
        }

    } else {
        free(pos);
    }
}

void addNeighbors(Board *board, Position *pos, PositionQueue *queue) {
    Position *upNeighbor = getNewPosition(pos->row - 1, pos->col);
    processPosition(board, upNeighbor, queue);

    Position *rightNeighbor = getNewPosition(pos->row, pos->col + 1);
    processPosition(board, rightNeighbor, queue);

    Position *downNeighbor = getNewPosition(pos->row + 1, pos->col);
    processPosition(board, downNeighbor, queue);

    Position *leftNeighbor = getNewPosition(pos->row, pos->col - 1);
    processPosition(board, leftNeighbor, queue);
}

bool isPath(Game *game, Position *targetPlayerPos) {
    PositionQueue queue;
    initPositionQueue(&queue);

    Position *playerPos = game->playerPos;
    Position *copiedPlayerPos = getNewPosition(playerPos->row, playerPos->col);

    bool isPathFound = false;
    processPosition(game->board, copiedPlayerPos, &queue);

    while (!isPositionQueueEmpty(&queue) && !isPathFound) {
        Position *currPos = popFront(&queue);
        if (arePositionsEqual(currPos, targetPlayerPos)) {
            isPathFound = true;
        } else {
            addNeighbors(game->board, currPos, &queue);
        }
        free(currPos);
    }

    free(targetPlayerPos);
    clearPositionQueue(&queue);
    unmarkVisitedSquares(game->board);

    return isPathFound;
}

// TODO comment
bool isChestPushPossible(Game *game, PushCommand *pushComm) {
    bool isPosInRange = false;
    bool isLegalSqr = false;
    Position *targetChestPos = getTargetChestPosition(game, pushComm);

    if (isPositionInRange(game->board, targetChestPos)) {
        isPosInRange = true;
        Row *targetChestRow = game->board->rows[targetChestPos->row];
        char targetChestSqr = targetChestRow->squares[targetChestPos->col];
        isLegalSqr = isLegalSquare(targetChestSqr);
    }

    free(targetChestPos);

    return isPosInRange && isLegalSqr;
}

// TODO comment
bool isApproachPossible(Game *game, PushCommand *pushComm) {
    Position *targetPlayerPos = getTargetPlayerPosition(game, pushComm);
    if (!isPositionInRange(game->board, targetPlayerPos)) {
        free(targetPlayerPos);
        return false;
    } else {
        Row *targetPlayerRow = game->board->rows[targetPlayerPos->row];
        char targetPlayerSqr = targetPlayerRow->squares[targetPlayerPos->col];
        if (!isLegalSquare(targetPlayerSqr)) {
            free(targetPlayerPos);
            return false;
        } else {
            return isPath(game, targetPlayerPos);
        }
    }
}

bool isPushCommandPossible(Game *game, PushCommand *pushComm) {
    if (!isChestPushPossible(game, pushComm)) {
        return false;
    } else {
        return isApproachPossible(game, pushComm);
    }
}

void readAndExecuteCommands(Game *game) {
    MoveStack stack;
    initMoveStack(&stack);

    int c = getchar();
    while (c != '.') {
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

    disposeBoard(&board);
    disposeChestsPositions(game.chestsPos);

    return 0;
}
