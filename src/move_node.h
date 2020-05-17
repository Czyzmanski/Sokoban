#ifndef MOVE_NODE_H
#define MOVE_NODE_H

#include "move.h"

struct MoveNode {
    Move *move;
    struct MoveNode *next;
};

typedef struct MoveNode MoveNode;

static inline MoveNode *getNewMoveNode(Move *move, MoveNode *next) {
    MoveNode *node = malloc(sizeof(MoveNode));
    assert(node != NULL);
    node->move = move;
    node->next = next;
    return node;
}

#endif // MOVE_NODE_H
