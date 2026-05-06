#ifndef QUEUE_H
#define QUEUE_H

#include "data_types.h"

// The Queue structure tracks the front, rear, and size
typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;

// Initialization and Utility
void initQueue(Queue* q);
int isQueueEmpty(Queue* q);

// 1. Simple Queue (FIFO)
void enqueueSimple(Queue* q, Citizen data);
Citizen dequeueSimple(Queue* q);

// 2. Double Ended Queue (Deque)
// Note: enqueueRear is identical to enqueueSimple, dequeueFront is identical to dequeueSimple.
void enqueueFront(Queue* q, Citizen data);
Citizen dequeueRear(Queue* q);

// 3. Circular Queue
void enqueueCircular(Queue* q, Citizen data);
Citizen dequeueCircular(Queue* q);

// 4. Priority Queue
void enqueuePriority(Queue* q, Citizen data, int priority);
// Note: dequeuePriority is identical to dequeueSimple since highest priority is always at the front.

// General Operations (Requirement I & III)
void displayQueue(Queue* q, int isCircular);
void searchQueueBySurname(Queue* q, const char* surname, int isCircular);
void clearQueue(Queue* q, int isCircular);

#endif