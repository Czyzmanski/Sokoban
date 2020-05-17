#ifndef SQUARES_H
#define SQUARES_H

#include <stdbool.h>

#define BLANK_SQUARE '-'
#define VISITED_BLANK_SQUARE '$'
#define FINAL_BLANK_SQUARE '+'
#define VISITED_FINAL_BLANK_SQUARE '&'
#define PLAYER_SQUARE '@'
#define VISITED_PLAYER_SQUARE '!'
#define FINAL_PLAYER_SQUARE '*'
#define VISITED_FINAL_PLAYER_SQUARE '^'

static inline bool isPlayerSquare(char square) {
    return square == PLAYER_SQUARE || square == FINAL_PLAYER_SQUARE;
}

static inline bool isFinalChestSquare(char square) {
    return 'A' <= square && square <= 'Z';
}

static inline bool isChestSquare(char square) {
    return ('a' <= square && square <= 'z') || ('A' <= square && square <= 'Z');
}

static inline bool isBlankSquare(char square) {
    return square == BLANK_SQUARE || square == FINAL_BLANK_SQUARE;
}

/* Checks if square can form a path to position where player can push chest
 * and if chest can be pushed onto square. */
static inline bool isLegalSquare(char square) {
    return isBlankSquare(square) || isPlayerSquare(square);
}

static inline int getChestNum(char chestName) {
    if (isFinalChestSquare(chestName)) {
        return chestName - 'A';
    } else {
        return chestName - 'a';
    }
}

static inline char getChestName(int chestNum, char square) {
    if (square == BLANK_SQUARE || square == PLAYER_SQUARE) {
        return 'a' + chestNum;
    } else {
        return 'A' + chestNum;
    }
}

#endif // SQUARES_H
