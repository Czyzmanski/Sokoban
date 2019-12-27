
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

void disposePosition(Position *pos)
{
    free(pos);
}

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
    disposePosition(move->prevChestPos);
    disposePosition(move->prevPlayerPos);
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

bool isPlayerSquare(char square)
{
    return square == PLAYER_SQUARE || square == TARGET_PLAYER_SQUARE;
}

bool isTargetChestSquare(char square)
{
    return 'A' <= square && square <= 'Z';
}

bool isNonTargetChestSquare(char square)
{
    return 'a' <= square && square <= 'z';
}

bool isChestSquare(char square)
{
    return isNonTargetChestSquare(square) || isTargetChestSquare(square);
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

void executeUndoCommand(Game *sokoban, MoveStack *stack)
{
    // TODO
}

void executePushCommand(Game *sokoban, PositionQueue *queue, MoveStack *stack)
{
    // TODO
}

void executeCommands(Game *sokoban)
{
    int c = getchar();
    while (c != '.')
    {
        if (c == UNDO_COMMAND)
        {
            executeUndoCommand(board, chestsPos, playerPos);
        }
        else
        {
        }
        c = getchar();
    }
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

    Game sokoban;
    sokoban.board = &board;
    sokoban.playerPos = &playerPos;
    sokoban.chestsPos = chestsPos;

    executeCommands(&sokoban);

    disposeBoard(&board);
    disposeChestsPositions(chestsPos);

    return 0;
}
