
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define GROWTH_FACTOR 2
#define INITIAL_CAPACITY 4

#define NUM_OF_CHESTS 26
#define MISSING_POSITION NULL

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

typedef struct
{
    char *squares;
    int size;
    int capacity;
} Row;

void initRow(Row *row)
{
    row->size = 0;
    row->capacity = INITIAL_CAPACITY;
    row->squares = malloc(row->capacity * sizeof(char));
    assert(row->squares != NULL);
}

Row *getNewRow()
{
    Row *newRow = malloc(sizeof(Row));
    assert(newRow != NULL);
    initRow(newRow);
    return newRow;
}

void reallocRow(Row *row)
{
    row->capacity *= GROWTH_FACTOR;
    row->squares = realloc(row->squares, row->capacity * sizeof(char));
    assert(row->squares != NULL);
}

void addToRow(Row *row, char square)
{
    if (row->size == row->capacity)
    {
        reallocRow(row);
    }
    row->squares[row->size] = square;
    row->size++;
}

void printRow(Row *row)
{
    for (int i = 0; i < row->size; i++)
    {
        printf("%c", row[i]);
    }
}

void disposeRow(Row *row)
{
    free(row->squares);
}

typedef struct
{
    Row **rows;
    int size;
    int capacity;
} Board;

void initBoard(Board *board)
{
    board->size = 0;
    board->capacity = INITIAL_CAPACITY;
    board->rows = malloc(board->capacity * sizeof(Row *));
    assert(board->rows != NULL);
}

void reallocBoard(Board *board)
{
    board->capacity *= GROWTH_FACTOR;
    board->rows = realloc(board->rows, board->capacity * sizeof(Row *));
    assert(board->rows != NULL);
}

void addToBoard(Board *board, Row *row)
{
    if (board->size == board->capacity)
    {
        reallocBoard(board);
    }
    board->rows[board->size] = row;
    board->size++;
}

void printBoard(Board *board)
{
    for (int i = 0; i < board->size; i++)
    {
        printRow(board->rows[i]);
        printf("\n");
    }
}

void disposeBoard(Board *board)
{
    for (int i = 0; i < b->size; i++)
    {
        disposeRow(board->rows[i]);
        free(board->rows[i]);
    }
    free(rows);
}

typedef struct
{
    int row;
    int col;
} Position;

Position *getNewPosition(int row, int col)
{
    Position *newPos = malloc(sizeof(Position));
    assert(newPos != NULL);
    newPos->row = row;
    newPos->col = col;
    return newPos;
}

typedef struct
{
    int chestNum;
    char direction;
} PushCommand;

typedef struct
{
    Board *board;
    Position *playerPos;
    Position *chestsPos[NUM_OF_CHESTS];
} Game;

// comment on this
typedef struct
{
    int chestNum;
    Position *prevChestPos;
    Position *prevPlayerPos;
} Move;

Move *getNewMove(int chestNum, Position *prevChestPos, Position *prevPlayerPos)
{
    Move *newMove = malloc(sizeof(Move));
    assert(newMove != NULL);
    newMove->chestNum = chestNum;
    newMove->prevChestPos = prevChestPos;
    newMove->prevPlayerPos = prevPlayerPos;
    return newMove;
}

void disposeMove(Move *move)
{
    free(move->prevChestPos);
    free(move->prevPlayerPos);
    free(move);
}

typedef struct
{
    Move *move;
    MoveNode *next;
} MoveNode;

MoveNode *getNewMoveNode(Move *move, MoveNode *next)
{
    MoveNode *newMoveNode = malloc(sizeof(MoveNode));
    assert(newMoveNode != NULL);
    newMoveNode->move = move;
    newMoveNode->next = next;
}

void disposeMoveNode(MoveNode *node)
{
    disposeMove(node->move);
    free(node);
}

typedef struct
{
    MoveNode *top;
} MoveStack;

void initMoveStack(MoveStack *stack)
{
    stack->top = NULL;
}

bool isMoveStackEmpty(MoveStack *stack)
{
    return stack->top == NULL;
}

Move *top(MoveStack *stack)
{
    return stack->top->move;
}

Move *pop(MoveStack *stack)
{
    Move *topMove = top(stack);
    MoveNode *topNode = stack->top;
    stack->top = topNode->next;
    free(topNode);
    return topMove;
}

void push(MoveStack *stack, Move *move)
{
    MoveNode *node = getNewMoveNode(move, stack->top);
    stack->top = node;
}

void clearMoveStack(MoveStack *stack)
{
    while (!isPositionStackEmpty(stack))
    {
        MoveNode *node = pop(stack);
        disposeMoveNode(node);
    }
}

typedef struct
{
    Position *pos;
    PositionNode *next;
} PositionNode;

PositionNode *getNewPositionNode(Position *pos, PositionNode *next)
{
    PositionNode *newNode = malloc(sizeof(PositionNode));
    assert(newNode != NULL);
    newNode->pos = pos;
    newNode->next = next;
    return newNode;
}

void disposePositionNode(PositionNode *node)
{
    disposePosition(node->pos);
    free(node);
}

typedef struct
{
    PositionNode *front;
    PositionNode *back;
} PositionQueue;

void initPositionQueue(PositionQueue *queue)
{
    queue->front = NULL;
    queue->back = NULL;
}

bool isPositionQueueEmpty(PositionQueue *queue)
{
    return queue->front == NULL;
}

Position *front(PositionQueue *queue)
{
    return queue->front->pos;
}

Position *popFront(PositionQueue *queue)
{
    if (queue->front == queue->back)
    {
        queue->back = NULL;
    }
    Position *frontPos = front(queue);
    PositionNode *frontNode = queue->front;
    queue->front = frontNode->next;
    free(frontNode);
    return frontPos;
}

void pushBack(PositionQueue *queue, Position *pos)
{
    PositionNode *node = getNewPositionNode(pos, NULL);
    if (isPositionQueueEmpty(queue))
    {
        queue->front = node;
    }
    else
    {
        queue->back->next = node;
    }
    queue->back = node;
}

void clearPositionQueue(PositionQueue *queue)
{
    while (!isPositionQueueEmpty(queue))
    {
        PositionNode *node = popFront(queue);
        disposePositionNode(node);
    }
}

bool isPlayerSquare(char sq)
{
    return sq == PLAYER_SQUARE || sq == TARGET_PLAYER_SQUARE;
}

bool isTargetChestSquare(char sq)
{
    return 'A' <= sq && sq <= 'Z';
}

bool isChestSquare(char sq)
{
    return 'a' <= sq && sq <= 'z' || 'A' <= sq && sq <= 'Z';
}

bool isBlankSquare(char sq)
{
    return sq == BLANK_SQUARE || sq == TARGET_BLANK_SQUARE;
}

int getChestNum(char chestName)
{
    if (isTargetChestSquare(chestName))
    {
        return chestName - 'A';
    }
    else
    {
        return chestName - 'a';
    }
}

void findPlayerPosition(Board *board, Position *playerPos)
{
    bool isFound = false;
    for (int i = 0; i < board->size && !isFound; i++)
    {
        for (int j = 0; j < board->rows[i]->size && !isFound; j++)
        {
            char square = board->rows[i]->squares[j];
            if (isPlayerSquare(square))
            {
                isFound = true;
                playerPos->row = i;
                playerPos->col = j;
            }
        }
    }
}

void findChestsPositions(Board *board, Position *chestsPos[])
{
    for (int i = 0; i < board->size; i++)
    {
        for (int j = 0; j < board->rows[i]->size; j++)
        {
            char square = board->rows[i]->squares[j];
            if (isChestSquare(square))
            {
                int chestNum = getChestNum(square);
                chestsPos[chestNum] = getNewPosition(i, j);
            }
        }
    }
}

void initChestsPositions(Position *chestsPos[])
{
    for (int i = 0; i < NUM_OF_CHESTS; i++)
    {
        chestsPositions[i] = MISSING_POSITION;
    }
}

void disposeChestsPositions(Position *chestsPos[])
{
    for (int i = 0; i < NUM_OF_CHESTS; i++)
    {
        if (chestsPositions[i] != MISSING_POSITION)
        {
            free(chestsPositions[i]);
        }
    }
}

void loadLineToRow(Row *row, int c)
{
    do
    {
        addSquareToRow(row, c);
        c = getchar();
    } while (c != '\n');
}

void readInitialBoardState(Board *board)
{
    int c = getchar();
    while (c != '\n')
    {
        Row *row = getNewRow();
        loadLineToRow(row, c);
        addRowToBoard(board, row);
        c = getchar();
    }
}

char getChestName(int chestNum, char square)
{
    if (square == BLANK_SQUARE || square == PLAYER_SQUARE)
        return 'a' + chestNum;
    else
        return 'A' + chestNum;
}

void executeUndoCommand(Game *game, MoveStack *stack)
{
    Position *playerPos = game->playerPos;
    Position *chestPos = game->chestsPos[pushComm->chestNum];
    Row *playerRow = game->board->rows[playerPos->row];
    Row *chestRow = game->board->rows[chestPos->row];

    char *playerSquare = &(playerRow->squares[playerPos->col]);
    if (*playerSquare == PLAYER_SQUARE)
        *playerSquare = getChestName(chestNum, PLAYER_SQUARE);
    else // *playerSquare == TARGET_PLAYER_SQUARE
        *playerSquare = getChestName(chestNum, TARGET_PLAYER_SQUARE);

    char *chestSquare = &(chestRow->squares[chestPos->col]);
    if (isTargetChestSquare(*chestSquare))
        *chestSquare = TARGET_BLANK_SQUARE;
    else
        *chestSquare = BLANK_SQUARE;

    Move *pastMove = pop(stack);
    Position *pastPlayerPos = pastMove->prevPlayerPos;
    Position *pastChestPos = pastMove->prevChestPos;
    Row *pastPlayerRow = game->board->rows[pastPlayerPos->row];

    char *pastPlayerSquare = &(pastPlayerRow->squares[pastPlayerPos->col]);
    if (*pastPlayerSquare == BLANK_SQUARE)
        *pastPlayerSquare = PLAYER_SQUARE;
    else // *pastPlayerSquare == TARGET_BLANK_SQUARE
        *pastPlayerSquare = TARGET_PLAYER_SQUARE;

    playerPos->row = pastPlayerPos->row;
    playerPos->col = pastPlayerPos->col;

    chestPos->row = pastChestPos->row;
    chestPos->col = pastChestPos->col;

    disposeMove(pastMove);
}

int getPushedChestRowNumber(int row, char pushDirection)
{
    if (pushDirection == DOWN)
        return row + 1;
    else if (pushDirection == UP)
        return row - 1;
    else
        return row;
}

int getPushedChestColNumber(int col, char pushDirection)
{
    if (pushDirection == LEFT)
        return col - 1;
    else if (pushDirection == RIGHT)
        return col + 1;
    else
        return col;
}

void executePushCommand(Game *game, PushCommand *pushComm, MoveStack *stack)
{
    int chestNum = pushComm->chestNum;
    Position *playerPos = game->playerPos;
    Position *chestPos = game->chestsPos[chestNum];
    Row *playerRow = game->board->rows[playerPos->row];
    Row *chestRow = game->board->rows[chestPos->row];

    Position *copiedPlayerPos = getNewPosition(playerPos->row, playerPos->col);
    Position *copiedChestPos = getNewPosition(chestPos->row, chestPos->col);
    Move *pushMove = getNewMove(chestNum, copiedChestPos, copiedPlayerPos);
    push(stack, pushMove);

    char *playerSquare = &(playerRow->squares[playerPos->col]);
    if (*playerSquare == PLAYER_SQUARE)
        *playerSquare = BLANK_SQUARE;
    else // *playerSquare == TARGET_PLAYER_SQUARE
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

Position *getTargetPosition(Game *game, PushCommand *pushComm)
{
    char pushDirection = pushComm->direction;
    Position *chestToPushPos = game->chestsPos[pushComm->chestNum];

    int targetRow = chestToPushPos->row;
    if (pushDirection == DOWN)
        targetRow--;
    else if (pushDirection == UP)
        targetRow++;

    int targetCol = chestToPushPos->col;
    if (pushDirection == LEFT)
        targetCol--;
    else if (pushDirection == RIGHT)
        targetCol++;

    return getNewPosition(targetRow, targetCol);
}

bool isPath(Game *game, Position *targetPlayerPos)
{
    PositionQueue queue;
    initPositionQueue(&queue);

    Position *playerPos = game->playerPos;
    Position *copiedPlayerPos = getNewPosition(playerPos->row, playerPos->col);

    clearPositionQueue(&queue);
}

bool isApproachPossible(Game *game, PushCommand *pushComm)
{
    Position *targetPlayerPos = getTargetPosition(game, pushComm);
    if (!isPositionInRange(game, targetPlayerPos))
    {
        free(targetPlayerPos);
        return false;
    }
    else
    {
        Row *targetPlayerRow = game->board->rows[targetPlayerPos->row];
        char targetPlayerSquare = targetRow->squares[targetPlayerPos->col];
        return isBlankSquare(targetPlayerSquare) && isPath(game, targetPlayerPos);
    }
}

bool isPushCommandPossible(Game *game, PushCommand *pushComm)
{
    if (!isChestPushPossible(game, pushComm))
        return false;
    else
        return isApproachPossible(game, pushComm);
}

void executeCommands(Game *game)
{
    MoveStack stack;
    initMoveStack(&stack);

    int c = getchar();
    while (c != '.')
    {
        if (c == UNDO_COMMAND)
        {
            if (!isMoveStackEmpty(&stack))
            {
                executeUndoCommand(board, chestsPos, playerPos);
            }
        }
        else
        {
            PushCommand pushComm;
            pushComm.chestNum = getChestNum(c);
            pushComm.direction = getchar();
            if (isPushCommandPossible(game, &pushComm))
            {
                executePushCommand(game, &pushComm, &stack);
            }
        }
        printBoard(game->board);
        c = getchar();
    }

    clearMoveStack(&stack);
}

int main()
{
    Board board;
    initBoard(&board);
    readInitialBoardState(&board);

    Position playerPos;
    findPlayerPosition(&board, &playerPos);

    Position *chestsPos[NUM_OF_CHESTS];
    initChestsPositions(chestsPos);
    findChestsPositions(&board, chestsPos);

    Game game;
    game.board = &board;
    game.playerPos = &playerPos;
    game.chestsPos = chestsPos;

    executeCommands(&game);

    disposeBoard(&board);
    disposeChestsPositions(chestsPos);

    return 0;
}
