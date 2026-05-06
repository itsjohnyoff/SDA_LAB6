#ifndef QUEUE_H
#define QUEUE_H

#include "data_types.h"

typedef enum {
    QUEUE_SIMPLE = 1,
    QUEUE_DEQUE = 2,
    QUEUE_CIRCULAR = 3,
    QUEUE_PRIORITY = 4
} QueueType;

typedef struct {
    Node* front;
    Node* rear;
    size_t size;
    QueueType type;
} Queue;

void initQueue(Queue* q, QueueType type);
int isQueueEmpty(const Queue* q);
size_t queueSize(const Queue* q);
const char* queueTypeName(QueueType type);

int enqueueQueue(Queue* q, const Citizen* data, int priority);
int dequeueQueue(Queue* q, Citizen* removed);

int enqueueQueueFront(Queue* q, const Citizen* data);
int dequeueQueueRear(Queue* q, Citizen* removed);

int searchQueueByPosition(const Queue* q, size_t position, Citizen* found, int* priority);
int searchQueueBySurname(const Queue* q, const char* surname, Citizen* found, size_t* position, int* priority);

int deleteQueueByPosition(Queue* q, size_t position, Citizen* removed, int* priority);
int deleteQueueBySurname(Queue* q, const char* surname, Citizen* removed, size_t* position, int* priority);

void displayQueue(const Queue* q);
void clearQueue(Queue* q);

#endif
