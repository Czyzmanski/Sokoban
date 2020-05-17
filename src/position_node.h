#ifndef POSITION_NODE_H
#define POSITION_NODE_H

#include "position.h"

struct PositionNode {
    Position *pos;
    struct PositionNode *next;
};

typedef struct PositionNode PositionNode;

static inline PositionNode *getNewPositionNode(Position *pos, PositionNode *next) {
    PositionNode *newNode = malloc(sizeof(PositionNode));
    assert(newNode != NULL);
    newNode->pos = pos;
    newNode->next = next;
    return newNode;
}

static inline void disposePositionNode(PositionNode *node) {
    free(node->pos);
    free(node);
}

#endif // POSITION_NODE_H
