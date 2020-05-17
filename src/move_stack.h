#ifndef MOVE_STACK_H
#define MOVE_STACK_H

#include <stdbool.h>

#include "move_node.h"

struct MoveStack {
    MoveNode *top;
};

typedef struct MoveStack MoveStack;

static inline void initMoveStack(MoveStack *stack) {
    stack->top = NULL;
}

static inline bool isMoveStackEmpty(MoveStack *stack) {
    return stack->top == NULL;
}

static inline Move *top(MoveStack *stack) {
    return stack->top->move;
}

static inline Move *pop(MoveStack *stack) {
    Move *topMove = top(stack);
    MoveNode *topNode = stack->top;
    stack->top = topNode->next;
    free(topNode);
    return topMove;
}

static inline void push(MoveStack *stack, Move *move) {
    MoveNode *node = getNewMoveNode(move, stack->top);
    stack->top = node;
}

static inline void clearMoveStack(MoveStack *stack) {
    while (!isMoveStackEmpty(stack)) {
        Move *move = pop(stack);
        disposeMove(move);
    }
}

#endif // MOVE_STACK_H
