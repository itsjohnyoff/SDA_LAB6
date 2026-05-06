#include <stdio.h>
#include <stdlib.h>
#include "file_io.h"

void getFileAddress(char* filenameBuffer) {
    printf("Enter full file address/name (e.g., data.txt or data.bin): ");
    scanf("%s", filenameBuffer);
    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void saveStackToFile(Stack* s, const char* filename, int asBinary) {
    FILE* file = fopen(filename, asBinary ? "wb" : "w");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }

    Node* current = s->top;
    while (current != NULL) {
        if (asBinary) {
            // Write the raw memory block of the struct
            fwrite(&(current->data), sizeof(Citizen), 1, file);
        } else {
            // Write formatted text
            fprintf(file, "%s %s | DOB: %02d/%02d/%04d | Gender: %c\n", 
                    current->data.name, current->data.surname, 
                    current->data.dob.day, current->data.dob.month, current->data.dob.year,
                    current->data.gender);
        }
        current = current->next;
    }

    fclose(file);
    printf("Stack successfully saved to %s\n", filename);
}

void saveQueueToFile(Queue* q, const char* filename, int asBinary, int isCircular) {
    FILE* file = fopen(filename, asBinary ? "wb" : "w");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }

    if (!isQueueEmpty(q)) {
        Node* current = q->front;
        do {
            if (asBinary) {
                fwrite(&(current->data), sizeof(Citizen), 1, file);
            } else {
                fprintf(file, "%s %s | DOB: %02d/%02d/%04d | Priority: %d\n", 
                        current->data.name, current->data.surname, 
                        current->data.dob.day, current->data.dob.month, current->data.dob.year,
                        current->priority);
            }
            current = current->next;
        } while (current != NULL && (!isCircular || current != q->front));
    }

    fclose(file);
    printf("Queue successfully saved to %s\n", filename);
}

void deleteDataFile(const char* filename) {
    if (remove(filename) == 0) {
        printf("File %s deleted successfully.\n", filename);
    } else {
        printf("Error: Unable to delete the file %s.\n", filename);
    }
}