#ifndef FILE_IO_H
#define FILE_IO_H

#include "stack.h"
#include "queue.h"

/* creates an empty text or binary file on disk */
int createEmptyDataFile(const char* filename, int binaryMode);

/* saves stack to a text file, one citizen per line */
int saveStackToTextFile(const Stack* s, const char* filename);

/* saves stack to a binary file using raw Citizen structs */
int saveStackToBinaryFile(const Stack* s, const char* filename);

/* loads citizen records from a binary file into the stack */
int loadStackFromBinaryFile(Stack* s, const char* filename, int replaceExisting);

/* saves queue to a text file, front to rear */
int saveQueueToTextFile(const Queue* q, const char* filename);

/* saves queue to a binary file (citizen + priority per record) */
int saveQueueToBinaryFile(const Queue* q, const char* filename);

/* loads queue records from a binary file */
int loadQueueFromBinaryFile(Queue* q, const char* filename, int replaceExisting);

/* opens and prints a text file to the console */
int displayTextFile(const char* filename);

/* opens a stack binary file and prints its records */
int displayStackBinaryFile(const char* filename);

/* opens a queue binary file and prints its records */
int displayQueueBinaryFile(const char* filename);

/* deletes the given file from disk */
int deleteDataFile(const char* filename);

#endif
