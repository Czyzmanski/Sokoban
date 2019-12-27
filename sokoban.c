
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define GROWTH_FACTOR 2
#define INITIAL_CAPACITY 4

#define NUM_OF_CHESTS 26
#define MISSING_POSITION NULL

typedef struct {
    char* squares;
    int size;
    int capacity;
} Row;

void initRow(Row* row){
    row->size = 0;
    row->capacity = INITIAL_CAPACITY;
    row->squares = malloc(row->capacity * sizeof(char));
    assert(row->squares != NULL);
}

Row* getNewRow(){
    Row* newRow = malloc(sizeof(Row));
    assert(newRow != NULL);
    initRow(newRow);
    return newRow;
}

void reallocRow(Row* row){
    row->capacity *= GROWTH_FACTOR;
    row->squares = realloc(row->squares, row->capacity * sizeof(char));
    assert(row->squares != NULL);
}

void addToRow(Row* row, char square){
    if(row->size == row->capacity){
        reallocRow(row);
    }
    row->squares[row->size] = square;
    row->size++;
}

void disposeRow(Row* row){
    free(row->squares);
}

typedef struct {
    Row** rows;
    int size;
    int capacity;
} Board;

void initBoard(Board* board){
    board->size = 0;
    board->capacity = INITIAL_CAPACITY;
    board->rows = malloc(board->capacity * sizeof(Row*));
    assert(board->rows != NULL);
}

void reallocBoard(Board* board){
    board->capacity *= GROWTH_FACTOR;
    board->rows = realloc(board->rows, board->capacity * sizeof(Row*));
    assert(board->rows != NULL);
}

void addToBoard(Board* board, Row* row){
    if(board->size == board->capacity){
        reallocBoard(board);
    }
    board->rows[board->size] = row;
    board->size++;
}

void disposeBoard(Board* board){
    for(int i = 0; i < b->size; i++){
        disposeRow(board->rows[i]);
        free(board->rows[i]);
    }
    free(rows);
}

typedef struct {
    int row;
    int col;
} Position;

Position* getNewPosition(int row, int col){
    Position* newPos = malloc(sizeof(Position));
    assert(newPos != NULL);
    newPos->row = row;
    newPos->col = col;
    return newPos;
}

typedef struct {
    Position* pos;
    PositionNode* next;
} PositionNode;

PositionNode* getNewPositionNode(Position* pos, PositionNode* next){
    PositionNode* newNode = malloc(sizeof(PositionNode));
    assert(newNode != NULL);
    newNode->pos = pos;
    newNode->next = next;
    return newNode;
}

typedef struct {
    PositionNode* top;
} PositionStack;

void initPositionStack(PositionStack* stack){
    stack->top = NULL;
}

bool isPositionStackEmpty(PositionStack* stack){
    return stack->top == NULL;
}

Position* top(PositionStack* stack){
    return stack->top->pos;
}

Position* pop(PositionStack* stack){
    Position* topPos = top(stack);
    PositionNode* topNode = stack->top;
    stack->top = topNode->next;
    free(topNode);
    return topPos;
}

void push(PositionStack* stack, Position* pos){
    PositionNode* node = getNewPositionNode(pos, top(stack));
    stack->top = node;
}

void clearPositionStack(PositionStack* stack){
    while(!isPositionStackEmpty(stack)){
        PositionNode* node = pop(stack);
        free(node->pos);
        free(node);
    }
}

typedef struct {
    PositionNode* front;
    PositionNode* back;
} PositionQueue;

void initPositionQueue(PositionQueue* queue){
    queue->front = NULL;
    queue->back = NULL;
}

bool isPositionQueueEmpty(PositionQueue* queue){
    return queue->front == NULL;
}

Position* front(PositionQueue* queue){
    return queue->front->pos;
}

Position* popFront(PositionQueue* queue){
    if(queue->front == queue->back){
        queue->back = NULL;
    }
    Position* frontPos = front(queue);
    PositionNode* frontNode = queue->front;
    queue->front = frontNode->next;
    free(frontNode);
    return frontPos;
}

void pushBack(PositionQueue* queue, Position* pos){
    PositionNode* node = getNewPositionNode(pos, NULL);
    queue->back->next = node;
    queue->back = node;
}

void initChestsPositions(Position* chestsPositions[NUM_OF_CHESTS]){
    for(int i = 0; i < NUM_OF_CHESTS; i++){
        chestsPositions[i] = MISSING_POSITION;
    }
}

void disposeChestsPositions(Position* chestsPositions[NUM_OF_CHESTS]){
    for(int i = 0; i < NUM_OF_CHESTS; i++){
        if(chestsPositions[i] != MISSING_POSITION){
            free(chestsPositions[i]);
        }
    }
}

void loadLineToRow(Row* row, int c, Position* chestsPositions[NUM_OF_CHESTS]){
    do {
        addSquareToRow(row, c);
        c = getchar();
    } while(c != '\n');
}

void readInitBoardState(Board* board, Position* chestsPositions[NUM_OF_CHESTS]){
    int c = getchar();
    while(c != '\n'){
        Row* row = getNewRow();
        loadLineToRow(row, c, chestsPositions);
        addRowToBoard(board, row);
        c = getchar();
    }
}

int main(){
    Board board;
    initBoard(&board);
    
    Position* chestsPositions[NUM_OF_CHESTS];
    initChestsPositions(chestsPositions);
    
    readInitBoardState(&board);
    
    disposeBoard(&board);
    disposeChestsPositions(chestsPositions);
    
    return 0;
}
