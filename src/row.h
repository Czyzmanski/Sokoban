#ifndef ROW_H
#define ROW_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define GROWTH_FACTOR 2
#define INITIAL_CAPACITY 16

struct Row {
    char *squares;
    int size;
    int capacity;
};

typedef struct Row Row;

static inline void initRow(Row *row) {
    row->size = 0;
    row->capacity = INITIAL_CAPACITY;
    row->squares = malloc(row->capacity * sizeof(char));
    assert(row->squares != NULL);
}

static inline Row *getNewRow() {
    Row *row = malloc(sizeof(Row));
    assert(row != NULL);
    initRow(row);
    return row;
}

static inline void reallocRow(Row *row) {
    row->capacity *= GROWTH_FACTOR;
    row->squares = realloc(row->squares, row->capacity * sizeof(char));
    assert(row->squares != NULL);
}

static inline void addToRow(Row *row, char square) {
    if (row->size == row->capacity) {
        reallocRow(row);
    }
    row->squares[row->size] = square;
    row->size++;
}

static inline void loadLineToRow(Row *row, int c) {
    do {
        addToRow(row, c);
        c = getchar();
    } while (c != '\n');
}

static inline void printRow(Row *row) {
    for (int i = 0; i < row->size; i++) {
        printf("%c", row->squares[i]);
    }
}

static inline void disposeRow(Row *row) {
    free(row->squares);
}

#endif // ROW_H
