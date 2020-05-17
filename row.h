#ifndef ROW_H
#define ROW_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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

#endif // ROW_H
