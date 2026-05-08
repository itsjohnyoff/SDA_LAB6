#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

/* returns 1 if queue type is circular */
static int isCircularQueue(const Queue* q) {
    return q != NULL && q->type == QUEUE_CIRCULAR;
}

/* allocates a node with citizen data and priority */
static Node* createNode(const Citizen* data, int priority) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }

    node->data = *data;
    node->priority = priority;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

/* walks the queue from front, returns node at position (1-based) */
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

/* enqueue at rear for simple queue and deque */
static int enqueueRearLinear(Queue* q, const Citizen* data, int priority) {
    Node* node = createNode(data, priority);

    if (node == NULL) {
        printf("Memory allocation failed. Queue insert was cancelled.\n");
        return 0;
    }

    node->prev = q->rear;
    if (isQueueEmpty(q)) {
        q->front = node;     /* first element, also becomes front */
    } else {
        q->rear->next = node;
    }

    q->rear = node;
    q->size++;
    return 1;
}

/* enqueue at rear for circular queue,
   maintains the rear->next = front and front->prev = rear loop */
static int enqueueRearCircular(Queue* q, const Citizen* data, int priority) {
    Node* node = createNode(data, priority);

    if (node == NULL) {
        printf("Memory allocation failed. Circular queue insert was cancelled.\n");
        return 0;
    }

    if (isQueueEmpty(q)) {
        /* single node loops to itself */
        q->front = node;
        q->rear = node;
        node->next = node;
        node->prev = node;
    } else {
        /* insert between rear and front, update the circular links */
        node->prev = q->rear;
        node->next = q->front;
        q->rear->next = node;
        q->front->prev = node;
        q->rear = node;
    }

    q->size++;
    return 1;
}

/* inserts node sorted by priority (ascending, lower = higher priority) */
static int enqueueByPriority(Queue* q, const Citizen* data, int priority) {
    Node* node;
    Node* current;

    node = createNode(data, priority);
    if (node == NULL) {
        printf("Memory allocation failed. Priority queue insert was cancelled.\n");
        return 0;
    }

    if (isQueueEmpty(q)) {
        /* empty queue, node is both front and rear */
        q->front = node;
        q->rear = node;
    } else if (priority < q->front->priority) {
        /* lower priority than front, insert before front */
        node->next = q->front;
        q->front->prev = node;
        q->front = node;
    } else {
        /* walk until we find where to insert,
           equal priorities keep FIFO order */
        current = q->front;
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }

        node->next = current->next;
        node->prev = current;
        if (current->next != NULL) {
            current->next->prev = node;
        } else {
            q->rear = node;
        }
        current->next = node;
    }

    q->size++;
    return 1;
}

/* initializes queue with given type, empty state */
void initQueue(Queue* q, QueueType type) {
    if (q == NULL) {
        return;
    }

    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    q->type = type;
}

/* returns 1 if queue has no elements */
int isQueueEmpty(const Queue* q) {
    return q == NULL || q->front == NULL;
}

/* returns element count */
size_t queueSize(const Queue* q) {
    return q == NULL ? 0 : q->size;
}

/* returns the queue type as a display string */
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

/* dispatches to the correct enqueue based on queue type */
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

/* deque-only: inserts a citizen at the front */
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
        q->rear = node;        /* first element is also the rear */
    } else {
        q->front->prev = node;
    }

    q->front = node;
    q->size++;
    return 1;
}

/* removes the front node. for circular queues, updates the rear<->front loop */
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

/* deque-only: removes the rear node */
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

/* returns the citizen at the given queue position */
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

/* scans front-to-rear looking for a matching surname.
   uses a counter because circular queues have no NULL terminator */
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

/* unlinks and frees the node at the given position.
   position 1 is handled by dequeueQueue, rest by pointer patching */
int deleteQueueByPosition(Queue* q, size_t position, Citizen* removed, int* priority) {
    Node* target;

    if (q == NULL || position == 0 || position > q->size) {
        return 0;
    }

    /* front removal, delegate to dequeueQueue */
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

    /* copy data before freeing */
    if (removed != NULL) {
        *removed = target->data;
    }
    if (priority != NULL) {
        *priority = target->priority;
    }

    /* unlink target from the list */
    target->prev->next = target->next;
    if (isCircularQueue(q)) {
        /* circular: next is never NULL, always wraps */
        target->next->prev = target->prev;
        if (target == q->rear) {
            q->rear = target->prev;
        }
    } else if (target->next != NULL) {
        target->next->prev = target->prev;
    } else {
        q->rear = target->prev;       /* target was the tail */
    }

    free(target);
    q->size--;
    return 1;
}

/* finds citizen by surname, then deletes at that position */
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

/* prints all records front to rear.
   iterates by counter (not NULL check) to handle circular queues */
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

/* dequeues everything, freeing all nodes */
void clearQueue(Queue* q) {
    if (q == NULL) {
        return;
    }

    while (!isQueueEmpty(q)) {
        dequeueQueue(q, NULL);
    }
}
