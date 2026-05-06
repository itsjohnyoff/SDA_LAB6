#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

// Requirement I: Creating
void initStack(Stack* s) {
    s->top = NULL;
    s->size = 0;
}

int isStackEmpty(Stack* s) {
    return s->top == NULL;
}

// Requirement II: Insert an element
void push(Stack* s, Citizen data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return;
    }
    newNode->data = data;
    newNode->priority = 0; // Not used for standard stack
    newNode->next = s->top;
    newNode->prev = NULL; 
    
    // Connect the old top's prev pointer back to the new node
    if (s->top != NULL) {
        s->top->prev = newNode;
    }
    
    s->top = newNode;
    s->size++;
}

// Requirement IV: Deleting an element
Citizen pop(Stack* s) {
    Citizen emptyCitizen = {0}; // Fallback struct to return if empty
    
    if (isStackEmpty(s)) {
        printf("Stack Underflow! Cannot pop from an empty stack.\n");
        return emptyCitizen;
    }
    
    Node* temp = s->top;
    Citizen poppedData = temp->data;
    
    s->top = s->top->next;
    
    if (s->top != NULL) {
        s->top->prev = NULL;
    }
    
    free(temp); // Critical: frees the dynamically allocated memory
    s->size--;
    
    return poppedData;
}

// Requirement I: Crossing and displaying
void displayStack(Stack* s) {
    if (isStackEmpty(s)) {
        printf("Stack is empty.\n");
        return;
    }
    
    Node* current = s->top;
    printf("\n--- Current Stack (Top to Bottom) ---\n");
    while (current != NULL) {
        printf("Citizen: %s %s | DOB: %02d/%02d/%04d\n", 
               current->data.name, current->data.surname, 
               current->data.dob.day, current->data.dob.month, current->data.dob.year);
        current = current->next;
    }
    printf("-------------------------------------\n");
}

// Requirement III: Search for an element by value
void searchStackBySurname(Stack* s, const char* surname) {
    if (isStackEmpty(s)) {
        printf("Stack is empty. Cannot search.\n");
        return;
    }
    
    Node* current = s->top;
    int position = 1;
    int found = 0;
    
    while (current != NULL) {
        if (strcmp(current->data.surname, surname) == 0) {
            printf("Found %s %s at position %d from top.\n", 
                   current->data.name, current->data.surname, position);
            found = 1;
        }
        current = current->next;
        position++;
    }
    
    if (!found) {
        printf("Citizen with surname '%s' not found in stack.\n", surname);
    }
}

// Housekeeping: Prevents memory leaks when the program exits
void clearStack(Stack* s) {
    while (!isStackEmpty(s)) {
        pop(s);
    }
}