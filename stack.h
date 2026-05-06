#ifndef STACK_H
#define STACK_H

#include "data_types.h"

// The Stack structure tracks the top node and current size
typedef struct {
    Node* top;
    int size;
} Stack;

// Function prototypes for Stack operations (Requirement I, II, III, IV)
void initStack(Stack* s);
int isStackEmpty(Stack* s);
void push(Stack* s, Citizen data);
Citizen pop(Stack* s);
void displayStack(Stack* s);
void searchStackBySurname(Stack* s, const char* surname);
void clearStack(Stack* s);

#endif