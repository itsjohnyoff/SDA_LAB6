#ifndef FILE_IO_H
#define FILE_IO_H

#include "stack.h"
#include "queue.h"

// Requirement V: File operations
void getFileAddress(char* filenameBuffer);
void saveStackToFile(Stack* s, const char* filename, int asBinary);
void saveQueueToFile(Queue* q, const char* filename, int asBinary, int isCircular);
void deleteDataFile(const char* filename);

#endif