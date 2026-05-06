#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void initQueue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int isQueueEmpty(Queue* q) {
    return q->front == NULL;
}

// --- 1. Simple Queue ---
void enqueueSimple(Queue* q, Citizen data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->priority = 0;
    newNode->next = NULL;
    newNode->prev = q->rear;

    if (isQueueEmpty(q)) {
        q->front = newNode;
    } else {
        q->rear->next = newNode;
    }
    q->rear = newNode;
    q->size++;
}

Citizen dequeueSimple(Queue* q) {
    Citizen empty = {0};
    if (isQueueEmpty(q)) {
        printf("Queue Underflow!\n");
        return empty;
    }
    
    Node* temp = q->front;
    Citizen data = temp->data;
    
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    } else {
        q->front->prev = NULL;
    }
    
    free(temp);
    q->size--;
    return data;
}

// --- 2. Double Ended Queue ---
void enqueueFront(Queue* q, Citizen data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->priority = 0;
    newNode->prev = NULL;
    newNode->next = q->front;

    if (isQueueEmpty(q)) {
        q->rear = newNode;
    } else {
        q->front->prev = newNode;
    }
    q->front = newNode;
    q->size++;
}

Citizen dequeueRear(Queue* q) {
    Citizen empty = {0};
    if (isQueueEmpty(q)) {
        printf("Queue Underflow!\n");
        return empty;
    }

    Node* temp = q->rear;
    Citizen data = temp->data;

    q->rear = q->rear->prev;
    if (q->rear == NULL) {
        q->front = NULL;
    } else {
        q->rear->next = NULL;
    }

    free(temp);
    q->size--;
    return data;
}

// --- 3. Circular Queue ---
void enqueueCircular(Queue* q, Citizen data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->priority = 0;

    if (isQueueEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    } else {
        newNode->prev = q->rear;
        newNode->next = q->front;
        q->rear->next = newNode;
        q->front->prev = newNode;
        q->rear = newNode;
    }
    q->size++;
}

Citizen dequeueCircular(Queue* q) {
    Citizen empty = {0};
    if (isQueueEmpty(q)) {
        printf("Queue Underflow!\n");
        return empty;
    }

    Node* temp = q->front;
    Citizen data = temp->data;

    if (q->size == 1) {
        q->front = NULL;
        q->rear = NULL;
    } else {
        q->front = q->front->next;
        q->rear->next = q->front;
        q->front->prev = q->rear;
    }

    free(temp);
    q->size--;
    return data;
}

// --- 4. Priority Queue ---
void enqueuePriority(Queue* q, Citizen data, int priority) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->priority = priority;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (isQueueEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else if (priority < q->front->priority) {
        // Insert at front (Highest priority)
        newNode->next = q->front;
        q->front->prev = newNode;
        q->front = newNode;
    } else {
        // Traverse to find correct position
        Node* current = q->front;
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }
        
        newNode->next = current->next;
        newNode->prev = current;
        
        if (current->next != NULL) {
            current->next->prev = newNode;
        } else {
            q->rear = newNode; // Update rear if inserted at end
        }
        current->next = newNode;
    }
    q->size++;
}

// --- General Operations ---
void displayQueue(Queue* q, int isCircular) {
    if (isQueueEmpty(q)) {
        printf("Queue is empty.\n");
        return;
    }

    Node* current = q->front;
    printf("\n--- Current Queue (Front to Rear) ---\n");
    do {
        printf("Citizen: %s %s | DOB: %02d/%02d/%04d | Priority: %d\n", 
               current->data.name, current->data.surname, 
               current->data.dob.day, current->data.dob.month, current->data.dob.year,
               current->priority);
        current = current->next;
    } while (current != NULL && (!isCircular || current != q->front));
    printf("-------------------------------------\n");
}

void searchQueueBySurname(Queue* q, const char* surname, int isCircular) {
    if (isQueueEmpty(q)) {
        printf("Queue is empty. Cannot search.\n");
        return;
    }

    Node* current = q->front;
    int position = 1;
    int found = 0;

    do {
        if (strcmp(current->data.surname, surname) == 0) {
            printf("Found %s %s at position %d from front.\n", 
                   current->data.name, current->data.surname, position);
            found = 1;
        }
        current = current->next;
        position++;
    } while (current != NULL && (!isCircular || current != q->front));

    if (!found) {
        printf("Citizen with surname '%s' not found in queue.\n", surname);
    }
}

void clearQueue(Queue* q, int isCircular) {
    while (!isQueueEmpty(q)) {
        if (isCircular) {
            dequeueCircular(q);
        } else {
            dequeueSimple(q);
        }
    }
}