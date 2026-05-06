#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_types.h"
#include "stack.h"
#include "queue.h"
#include "file_io.h"

// Helper to quickly generate a dummy Citizen for testing
Citizen createTestCitizen(const char* name, const char* surname, int d, int m, int y) {
    Citizen c = {0}; // Initialize all to 0
    strcpy(c.name, name);
    strcpy(c.surname, surname);
    c.dob.day = d;
    c.dob.month = m;
    c.dob.year = y;
    c.gender = 'U';
    return c;
}

int main() {
    Stack myStack;
    initStack(&myStack);

    Queue myQueue;
    initQueue(&myQueue);
    
    int choice;
    char filename[256];
    char searchName[50];

    while (1) {
        printf("\n=== MAIN MENU ===\n");
        printf("1. Push to Stack\n");
        printf("2. Pop from Stack\n");
        printf("3. Display Stack\n");
        printf("4. Enqueue to Simple Queue\n");
        printf("5. Dequeue from Simple Queue\n");
        printf("6. Display Queue\n");
        printf("7. Search in Stack by Surname\n");
        printf("8. Save Stack to Text File\n");
        printf("9. Delete a File\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        
        if (scanf("%d", &choice) != 1) {
            // Handle invalid input
            while(getchar() != '\n'); 
            continue;
        }

        switch (choice) {
            case 1:
                printf("Pushing dummy data (John Doe) to stack...\n");
                push(&myStack, createTestCitizen("John", "Doe", 1, 1, 1990));
                break;
            case 2:
                pop(&myStack);
                printf("Popped top element.\n");
                break;
            case 3:
                displayStack(&myStack);
                break;
            case 4:
                printf("Enqueueing dummy data (Jane Smith) to queue...\n");
                enqueueSimple(&myQueue, createTestCitizen("Jane", "Smith", 15, 6, 1995));
                break;
            case 5:
                dequeueSimple(&myQueue);
                printf("Dequeued front element.\n");
                break;
            case 6:
                displayQueue(&myQueue, 0); // 0 means not circular
                break;
            case 7:
                printf("Enter surname to search: ");
                scanf("%s", searchName);
                searchStackBySurname(&myStack, searchName);
                break;
            case 8:
                getFileAddress(filename);
                saveStackToFile(&myStack, filename, 0); // 0 for text mode
                break;
            case 9:
                getFileAddress(filename);
                deleteDataFile(filename);
                break;
            case 0:
                printf("Cleaning up memory and exiting...\n");
                clearStack(&myStack);
                clearQueue(&myQueue, 0);
                return 0;
            default:
                printf("Invalid option.\n");
        }
    }
    return 0;
}