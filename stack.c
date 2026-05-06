#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

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

static Node* nodeAtPosition(const Stack* s, size_t position) {
    Node* current;
    size_t index;

    if (s == NULL || position == 0 || position > s->size) {
        return NULL;
    }

    current = s->top;
    for (index = 1; index < position; index++) {
        current = current->next;
    }

    return current;
}

void initStack(Stack* s) {
    if (s == NULL) {
        return;
    }

    s->top = NULL;
    s->size = 0;
}

int isStackEmpty(const Stack* s) {
    return s == NULL || s->top == NULL;
}

size_t stackSize(const Stack* s) {
    return s == NULL ? 0 : s->size;
}

int pushStack(Stack* s, const Citizen* data) {
    Node* node;

    if (s == NULL || data == NULL) {
        return 0;
    }

    node = createNode(data, 0);
    if (node == NULL) {
        printf("Memory allocation failed. Stack insert was cancelled.\n");
        return 0;
    }

    node->next = s->top;
    if (s->top != NULL) {
        s->top->prev = node;
    }

    s->top = node;
    s->size++;
    return 1;
}

int popStack(Stack* s, Citizen* removed) {
    Node* temp;

    if (isStackEmpty(s)) {
        printf("Stack underflow. The stack is empty.\n");
        return 0;
    }

    temp = s->top;
    if (removed != NULL) {
        *removed = temp->data;
    }

    s->top = temp->next;
    if (s->top != NULL) {
        s->top->prev = NULL;
    }

    free(temp);
    s->size--;
    return 1;
}

int searchStackByPosition(const Stack* s, size_t position, Citizen* found) {
    Node* node = nodeAtPosition(s, position);

    if (node == NULL) {
        return 0;
    }

    if (found != NULL) {
        *found = node->data;
    }

    return 1;
}

int searchStackBySurname(const Stack* s, const char* surname, Citizen* found, size_t* position) {
    Node* current;
    size_t index;

    if (s == NULL || surname == NULL) {
        return 0;
    }

    current = s->top;
    index = 1;

    while (current != NULL) {
        if (strcmp(current->data.surname, surname) == 0) {
            if (found != NULL) {
                *found = current->data;
            }
            if (position != NULL) {
                *position = index;
            }
            return 1;
        }

        current = current->next;
        index++;
    }

    return 0;
}

int deleteStackByPosition(Stack* s, size_t position, Citizen* removed) {
    Node* target;

    if (s == NULL || position == 0 || position > s->size) {
        return 0;
    }

    if (position == 1) {
        return popStack(s, removed);
    }

    target = nodeAtPosition(s, position);
    if (target == NULL) {
        return 0;
    }

    if (removed != NULL) {
        *removed = target->data;
    }

    if (target->prev != NULL) {
        target->prev->next = target->next;
    }
    if (target->next != NULL) {
        target->next->prev = target->prev;
    }

    free(target);
    s->size--;
    return 1;
}

int deleteStackBySurname(Stack* s, const char* surname, Citizen* removed, size_t* position) {
    Citizen ignored;
    size_t foundPosition;

    if (!searchStackBySurname(s, surname, &ignored, &foundPosition)) {
        return 0;
    }

    if (position != NULL) {
        *position = foundPosition;
    }

    return deleteStackByPosition(s, foundPosition, removed);
}

void displayStack(const Stack* s) {
    Node* current;
    size_t position;

    if (isStackEmpty(s)) {
        printf("Stack is empty.\n");
        return;
    }

    printf("\nVERSION A - Dynamic Stack based on List ADT\n");
    printf("Records: %zu\n", s->size);
    printf("Traversal order: top to bottom\n");
    printf("--------------------------------------------------------------------------\n");

    current = s->top;
    position = 1;
    while (current != NULL) {
        printCitizenBrief(&current->data, position, 0, 0);
        current = current->next;
        position++;
    }

    printf("--------------------------------------------------------------------------\n");
}

void clearStack(Stack* s) {
    if (s == NULL) {
        return;
    }

    while (!isStackEmpty(s)) {
        popStack(s, NULL);
    }
}
