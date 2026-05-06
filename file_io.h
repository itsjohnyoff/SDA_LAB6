#ifndef FILE_IO_H
#define FILE_IO_H

#include "stack.h"
#include "queue.h"

int createEmptyDataFile(const char* filename, int binaryMode);

int saveStackToTextFile(const Stack* s, const char* filename);
int saveStackToBinaryFile(const Stack* s, const char* filename);
int loadStackFromBinaryFile(Stack* s, const char* filename, int replaceExisting);

int saveQueueToTextFile(const Queue* q, const char* filename);
int saveQueueToBinaryFile(const Queue* q, const char* filename);
int loadQueueFromBinaryFile(Queue* q, const char* filename, int replaceExisting);

int displayTextFile(const char* filename);
int displayStackBinaryFile(const char* filename);
int displayQueueBinaryFile(const char* filename);
int deleteDataFile(const char* filename);

#endif
