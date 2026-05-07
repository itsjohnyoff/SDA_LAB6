#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

/* checks if q is circular type */
static int isCircularQueue(const Queue* q) {
    return q != NULL && q->type == QUEUE_CIRCULAR;
}

/* allocates a node and copies citizen data + priority into it */
static Node* createNode(const Citizen* data, int priority) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }

    node->data = *data;       /* copies the whole struct */
    node->priority = priority;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

/* goes through the list and returns the node at the given position (1-based) */
static Node* queueNodeAt(const Queue* q, size_t position) {
    Node* current;
    size_t index;

    if (q == NULL || position == 0 || position > q->size) {
        return NULL;
    }

    current = q->front;
    for (index = 1; index < position; index++) {
        current = current->next;
    }

    return current;
}

/* adds a node at the back, used for simple queue and deque */
static int enqueueRearLinear(Queue* q, const Citizen* data, int priority) {
    Node* node = createNode(data, priority);

    if (node == NULL) {
        printf("Memory allocation failed. Queue insert was cancelled.\n");
        return 0;
    }

    node->prev = q->rear;
    if (isQueueEmpty(q)) {
        q->front = node;     /* queue was empty, so this is also the front */
    } else {
        q->rear->next = node;
    }

    q->rear = node;
    q->size++;
    return 1;
}

/* adds a node at the back of a circular queue,
   rear->next always wraps to front, front->prev always wraps to rear */
static int enqueueRearCircular(Queue* q, const Citizen* data, int priority) {
    Node* node = createNode(data, priority);

    if (node == NULL) {
        printf("Memory allocation failed. Circular queue insert was cancelled.\n");
        return 0;
    }

    if (isQueueEmpty(q)) {
        /* only node in the queue, so it loops to itself */
        q->front = node;
        q->rear = node;
        node->next = node;
        node->prev = node;
    } else {
        /* put the new node after rear and before front, fix the loop */
        node->prev = q->rear;
        node->next = q->front;
        q->rear->next = node;
        q->front->prev = node;
        q->rear = node;
    }

    q->size++;
    return 1;
}

/* inserts into the right spot so the queue stays sorted by priority (ascending) */
static int enqueueByPriority(Queue* q, const Citizen* data, int priority) {
    Node* node;
    Node* current;

    node = createNode(data, priority);
    if (node == NULL) {
        printf("Memory allocation failed. Priority queue insert was cancelled.\n");
        return 0;
    }

    if (isQueueEmpty(q)) {
        /* nothing in the queue yet */
        q->front = node;
        q->rear = node;
    } else if (priority < q->front->priority) {
        /* new node has lower priority value than front, so it goes first */
        node->next = q->front;
        q->front->prev = node;
        q->front = node;
    } else {
        /* find the spot where priority is still <= ours,
           insert after that so equal priorities stay in order */
        current = q->front;
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }

        node->next = current->next;
        node->prev = current;
        if (current->next != NULL) {
            current->next->prev = node;  /* we're in the middle, fix prev pointer */
        } else {
            q->rear = node;              /* we're at the end, this is the new rear */
        }
        current->next = node;
    }

    q->size++;
    return 1;
}

/* sets front/rear to NULL, size to 0, and stores the queue type */
void initQueue(Queue* q, QueueType type) {
    if (q == NULL) {
        return;
    }

    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    q->type = type;
}

/* if front is NULL then there's nothing in the queue */
int isQueueEmpty(const Queue* q) {
    return q == NULL || q->front == NULL;
}

/* returns the number of elements, 0 if queue is NULL */
size_t queueSize(const Queue* q) {
    return q == NULL ? 0 : q->size;
}

/* gives back a string name for the queue type enum */
const char* queueTypeName(QueueType type) {
    switch (type) {
        case QUEUE_SIMPLE:
            return "Simple Queue";
        case QUEUE_DEQUE:
            return "Double Ended Queue";
        case QUEUE_CIRCULAR:
            return "Circular Queue";
        case QUEUE_PRIORITY:
            return "Priority Queue";
        default:
            return "Unknown Queue";
    }
}

/* calls the correct insert function depending on queue type,
   priority arg only matters for priority queue, the rest ignore it */
int enqueueQueue(Queue* q, const Citizen* data, int priority) {
    if (q == NULL || data == NULL) {
        return 0;
    }

    switch (q->type) {
        case QUEUE_SIMPLE:
        case QUEUE_DEQUE:
            return enqueueRearLinear(q, data, 0);
        case QUEUE_CIRCULAR:
            return enqueueRearCircular(q, data, 0);
        case QUEUE_PRIORITY:
            return enqueueByPriority(q, data, priority);
        default:
            return 0;
    }
}

/* inserts at the front of the deque, does nothing for other queue types */
int enqueueQueueFront(Queue* q, const Citizen* data) {
    Node* node;

    if (q == NULL || data == NULL || q->type != QUEUE_DEQUE) {
        return 0;
    }

    node = createNode(data, 0);
    if (node == NULL) {
        printf("Memory allocation failed. Deque insert was cancelled.\n");
        return 0;
    }

    node->next = q->front;
    if (isQueueEmpty(q)) {
        q->rear = node;        /* was empty, so this is also the rear */
    } else {
        q->front->prev = node;
    }

    q->front = node;
    q->size++;
    return 1;
}

/* takes out the front node, saves its data in *removed if not NULL.
   when circular, we also have to fix the rear<->front loop */
int dequeueQueue(Queue* q, Citizen* removed) {
    Node* temp;

    if (isQueueEmpty(q)) {
        printf("%s underflow. The queue is empty.\n", q == NULL ? "Queue" : queueTypeName(q->type));
        return 0;
    }

    temp = q->front;
    if (removed != NULL) {
        *removed = temp->data;
    }

    if (q->size == 1) {
        q->front = NULL;
        q->rear = NULL;
    } else if (isCircularQueue(q)) {
        q->front = temp->next;
        q->front->prev = q->rear;
        q->rear->next = q->front;
    } else {
        q->front = temp->next;
        q->front->prev = NULL;
    }

    free(temp);
    q->size--;
    return 1;
}

/* removes from the back of the deque, only works for QUEUE_DEQUE */
int dequeueQueueRear(Queue* q, Citizen* removed) {
    Node* temp;

    if (q == NULL || q->type != QUEUE_DEQUE) {
        return 0;
    }

    if (isQueueEmpty(q)) {
        printf("Deque underflow. The deque is empty.\n");
        return 0;
    }

    temp = q->rear;
    if (removed != NULL) {
        *removed = temp->data;
    }

    if (q->size == 1) {
        q->front = NULL;
        q->rear = NULL;
    } else {
        q->rear = temp->prev;
        q->rear->next = NULL;
    }

    free(temp);
    q->size--;
    return 1;
}

/* finds the citizen at the given position and copies their data out */
int searchQueueByPosition(const Queue* q, size_t position, Citizen* found, int* priority) {
    Node* node = queueNodeAt(q, position);

    if (node == NULL) {
        return 0;
    }

    if (found != NULL) {
        *found = node->data;
    }
    if (priority != NULL) {
        *priority = node->priority;
    }

    return 1;
}

/* goes through the queue comparing surnames, stops at the first match.
   uses a counter to iterate because in circular mode there's no NULL at the end */
int searchQueueBySurname(const Queue* q, const char* surname, Citizen* found, size_t* position, int* priority) {
    Node* current;
    size_t index;

    if (q == NULL || surname == NULL) {
        return 0;
    }

    current = q->front;
    for (index = 1; index <= q->size; index++) {
        if (strcmp(current->data.surname, surname) == 0) {
            if (found != NULL) {
                *found = current->data;
            }
            if (position != NULL) {
                *position = index;
            }
            if (priority != NULL) {
                *priority = current->priority;
            }
            return 1;
        }

        current = current->next;
    }

    return 0;  /* not found */
}

/* removes the node at the given position.
   if position is 1 we just dequeue from front.
   otherwise we unlink the node and patch the prev/next pointers around it */
int deleteQueueByPosition(Queue* q, size_t position, Citizen* removed, int* priority) {
    Node* target;

    if (q == NULL || position == 0 || position > q->size) {
        return 0;
    }

    /* position 1 = front, just use dequeueQueue */
    if (position == 1) {
        target = q->front;
        if (priority != NULL) {
            *priority = target->priority;
        }
        return dequeueQueue(q, removed);
    }

    target = queueNodeAt(q, position);
    if (target == NULL) {
        return 0;
    }

    /* save data before we free the node */
    if (removed != NULL) {
        *removed = target->data;
    }
    if (priority != NULL) {
        *priority = target->priority;
    }

    /* skip over the target node */
    target->prev->next = target->next;
    if (isCircularQueue(q)) {
        /* in circular mode next is never NULL, it wraps */
        target->next->prev = target->prev;
        if (target == q->rear) {
            q->rear = target->prev;
        }
    } else if (target->next != NULL) {
        target->next->prev = target->prev;
    } else {
        q->rear = target->prev;       /* was the tail, so rear moves back */
    }

    free(target);
    q->size--;
    return 1;
}

/* finds citizen by surname, gets their position, then deletes at that position.
   'ignored' is just a dummy variable, we only need the position from the search */
int deleteQueueBySurname(Queue* q, const char* surname, Citizen* removed, size_t* position, int* priority) {
    Citizen ignored;
    size_t foundPosition;

    if (!searchQueueBySurname(q, surname, &ignored, &foundPosition, priority)) {
        return 0;
    }

    if (position != NULL) {
        *position = foundPosition;
    }

    return deleteQueueByPosition(q, foundPosition, removed, priority);
}

/* prints every citizen from front to rear.
   iterates with a counter, not by checking NULL, because circular queues loop.
   shows priority column only when queue type is QUEUE_PRIORITY */
void displayQueue(const Queue* q) {
    Node* current;
    size_t position;
    int showPriority;

    if (isQueueEmpty(q)) {
        printf("%s is empty.\n", q == NULL ? "Queue" : queueTypeName(q->type));
        return;
    }

    showPriority = q->type == QUEUE_PRIORITY;
    printf("\nVERSION B - %s based on List ADT\n", queueTypeName(q->type));
    printf("Records: %zu\n", q->size);
    printf("Traversal order: front to rear\n");
    printf("--------------------------------------------------------------------------\n");

    current = q->front;
    for (position = 1; position <= q->size; position++) {
        printCitizenBrief(&current->data, position, current->priority, showPriority);
        current = current->next;
    }

    printf("--------------------------------------------------------------------------\n");
}

/* keeps dequeueing until the queue is empty, frees all nodes */
void clearQueue(Queue* q) {
    if (q == NULL) {
        return;
    }

    while (!isQueueEmpty(q)) {
        dequeueQueue(q, NULL);
    }
}
