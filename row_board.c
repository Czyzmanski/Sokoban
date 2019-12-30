
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define GROWTH_FACTOR 2
#define INITIAL_CAPACITY 4

#define NUM_OF_CHESTS 26

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
    Row *newRow = malloc(sizeof(Row));
    assert(newRow != NULL);
    initRow(newRow);
    return newRow;
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

struct Position {
    int row;
    int col;
};

typedef struct Position Position;

Position *getNewPosition(int row, int col) {
    Position *newPos = malloc(sizeof(Position));
    assert(newPos != NULL);
    newPos->row = row;
    newPos->col = col;
    return newPos;
}

struct Game {
    Board *board;
    Position *playerPos;
    Position *chestsPos[NUM_OF_CHESTS];
};

typedef struct Game Game;

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

    disposeBoard(&board);
    disposeChestsPositions(game.chestsPos);

    return 0;
}