#ifndef POSITION_QUEUE_H
#define POSITION_QUEUE_H

#include "position_node.h"

struct PositionQueue {
    PositionNode *front;
    PositionNode *back;
};

typedef struct PositionQueue PositionQueue;

static inline void initPositionQueue(PositionQueue *queue) {
    queue->front = NULL;
    queue->back = NULL;
}

static inline bool isPositionQueueEmpty(PositionQueue *queue) {
    return queue->front == NULL;
}

static inline Position *front(PositionQueue *queue) {
    return queue->front->pos;
}

static inline Position *popFront(PositionQueue *queue) {
    if (queue->front == queue->back) {
        queue->back = NULL;
    }
    Position *frontPos = front(queue);
    PositionNode *frontNode = queue->front;
    queue->front = frontNode->next;
    free(frontNode);
    return frontPos;
}

static inline void pushBack(PositionQueue *queue, Position *pos) {
    PositionNode *node = getNewPositionNode(pos, NULL);
    if (isPositionQueueEmpty(queue)) {
        queue->front = node;
    } else {
        queue->back->next = node;
    }
    queue->back = node;
}

static inline void clearPositionQueue(PositionQueue *queue) {
    while (!isPositionQueueEmpty(queue)) {
        Position *pos = popFront(queue);
        free(pos);
    }
}

#endif // POSITION_QUEUE_H
