#ifndef STACK_H
#define STACK_H

#include "data_types.h"

typedef struct {
    Node* top;
    size_t size;
} Stack;

void initStack(Stack* s);
int isStackEmpty(const Stack* s);
size_t stackSize(const Stack* s);

int pushStack(Stack* s, const Citizen* data);
int popStack(Stack* s, Citizen* removed);

int searchStackByPosition(const Stack* s, size_t position, Citizen* found);
int searchStackBySurname(const Stack* s, const char* surname, Citizen* found, size_t* position);

int deleteStackByPosition(Stack* s, size_t position, Citizen* removed);
int deleteStackBySurname(Stack* s, const char* surname, Citizen* removed, size_t* position);

void displayStack(const Stack* s);
void clearStack(Stack* s);

#endif
