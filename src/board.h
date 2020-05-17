#ifndef BOARD_H
#define BOARD_H

#include "row.h"
#include "position.h"

struct Board {
    Row **rows;
    int size;
    int capacity;
};

typedef struct Board Board;

static inline void initBoard(Board *board) {
    board->size = 0;
    board->capacity = INITIAL_CAPACITY;
    board->rows = malloc(board->capacity * sizeof(Row *));
    assert(board->rows != NULL);
}

static inline void reallocBoard(Board *board) {
    board->capacity *= GROWTH_FACTOR;
    board->rows = realloc(board->rows, board->capacity * sizeof(Row *));
    assert(board->rows != NULL);
}

static inline void addToBoard(Board *board, Row *row) {
    if (board->size == board->capacity) {
        reallocBoard(board);
    }
    board->rows[board->size] = row;
    board->size++;
}

static inline void printBoard(Board *board) {
    for (int i = 0; i < board->size; i++) {
        printRow(board->rows[i]);
        printf("\n");
    }
}

static inline void readInitialBoardState(Board *board) {
    int c = getchar();
    while (c != '\n') {
        Row *row = getNewRow();
        loadLineToRow(row, c);
        addToBoard(board, row);
        c = getchar();
    }
}

static inline bool isPositionInRange(Board *board, Position *pos) {
    return (0 <= pos->row && pos->row < board->size)
           && (0 <= pos->col && pos->col < board->rows[pos->row]->size);
}

static inline void disposeBoard(Board *board) {
    for (int i = 0; i < board->size; i++) {
        disposeRow(board->rows[i]);
        free(board->rows[i]);
    }
    free(board->rows);
}

#endif // BOARD_H
