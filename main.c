#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data_types.h"
#include "file_io.h"
#include "queue.h"
#include "stack.h"

#define INPUT_LENGTH 256
#define MAX_RECORDS 10000

/* strips the trailing newline that fgets leaves in the buffer */
static void trimNewline(char* text) {
    if (text != NULL) {
        text[strcspn(text, "\n")] = '\0';
    }
}

/* prints a prompt and reads one line from stdin into buffer */
static int readLine(const char* prompt, char* buffer, size_t length) {
    if (prompt != NULL) {
        printf("%s", prompt);
    }

    if (fgets(buffer, (int)length, stdin) == NULL) {
        return 0;
    }

    trimNewline(buffer);
    return 1;
}

/* keeps asking until the user types something non-empty */
static void readRequiredString(const char* prompt, char* destination, size_t length) {
    char buffer[INPUT_LENGTH];

    do {
        readLine(prompt, buffer, sizeof(buffer));
        if (buffer[0] == '\0') {
            printf("This field cannot be empty.\n");
        }
    } while (buffer[0] == '\0');

    strncpy(destination, buffer, length - 1);
    destination[length - 1] = '\0';
}

/* reads an integer and loops until it's within [minValue, maxValue] */
static int readIntRange(const char* prompt, int minValue, int maxValue) {
    char buffer[INPUT_LENGTH];
    char* end;
    long value;

    while (1) {
        readLine(prompt, buffer, sizeof(buffer));
        errno = 0;
        value = strtol(buffer, &end, 10);
        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (errno == 0 && end != buffer && *end == '\0' && value >= minValue && value <= maxValue) {
            return (int)value;
        }

        printf("Enter an integer from %d to %d.\n", minValue, maxValue);
    }
}

/* asks a y/n question, returns 1 for yes, 0 for no */
static int readYesNo(const char* prompt) {
    char buffer[INPUT_LENGTH];

    while (1) {
        readLine(prompt, buffer, sizeof(buffer));
        if (buffer[0] == 'y' || buffer[0] == 'Y') {
            return 1;
        }
        if (buffer[0] == 'n' || buffer[0] == 'N') {
            return 0;
        }

        printf("Enter y or n.\n");
    }
}

/* gets today's date from the system clock */
static Date currentDate(void) {
    time_t rawTime = time(NULL);
    struct tm* local = localtime(&rawTime);
    Date today = {8, 5, 2026};

    if (local != NULL) {
        today.day = local->tm_mday;
        today.month = local->tm_mon + 1;
        today.year = local->tm_year + 1900;
    }

    return today;
}

/* leap year check (divisible by 400, or by 4 but not 100) */
static int isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

/* returns how many days a given month has, handles february + leap year */
static int daysInMonth(int month, int year) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && isLeapYear(year)) {
        return 29;
    }

    return days[month - 1];
}

/* returns 1 if date is after today */
static int isFutureDate(Date date, Date today) {
    if (date.year != today.year) {
        return date.year > today.year;
    }
    if (date.month != today.month) {
        return date.month > today.month;
    }
    return date.day > today.day;
}

/* checks that day/month/year make sense as a real date */
static int isValidDate(Date date) {
    if (date.year < 1900 || date.month < 1 || date.month > 12 || date.day < 1) {
        return 0;
    }

    return date.day <= daysInMonth(date.month, date.year);
}

/* asks for day/month/year and loops until it's a valid past date */
static Date readDateOfBirth(void) {
    Date date;
    Date today = currentDate();

    while (1) {
        date.day = readIntRange("Birth day: ", 1, 31);
        date.month = readIntRange("Birth month: ", 1, 12);
        date.year = readIntRange("Birth year: ", 1900, today.year);

        if (isValidDate(date) && !isFutureDate(date, today)) {
            return date;
        }

        printf("Invalid or future date. Please enter the date again.\n");
    }
}

/* calculates age in years/months/days from dob, sets category and payment */
static void updateDerivedCitizenFields(Citizen* citizen) {
    Date today;
    int previousMonth;
    int previousYear;

    if (citizen == NULL) {
        return;
    }

    today = currentDate();
    citizen->ageYears = today.year - citizen->dob.year;
    citizen->ageMonths = today.month - citizen->dob.month;
    citizen->ageDays = today.day - citizen->dob.day;

    if (citizen->ageDays < 0) {
        citizen->ageMonths--;
        previousMonth = today.month - 1;
        previousYear = today.year;
        if (previousMonth == 0) {
            previousMonth = 12;
            previousYear--;
        }
        citizen->ageDays += daysInMonth(previousMonth, previousYear);
    }

    if (citizen->ageMonths < 0) {
        citizen->ageYears--;
        citizen->ageMonths += 12;
    }

    if (citizen->ageYears < 18) {
        citizen->category = CATEGORY_CHILD;
    } else if (citizen->ageYears < 60) {
        citizen->category = CATEGORY_ADULT;
    } else {
        citizen->category = CATEGORY_SENIOR;
    }

    if (citizen->category == CATEGORY_CHILD) {
        citizen->amountPaid = 500.0f;
    } else if (citizen->category == CATEGORY_ADULT) {
        citizen->amountPaid = 1000.0f;
    } else {
        citizen->amountPaid = 2000.0f;
    }
}

/* loops until the user enters M, F, or O */
static char readGender(void) {
    char buffer[INPUT_LENGTH];
    char gender;

    while (1) {
        readLine("Gender (M/F/O): ", buffer, sizeof(buffer));
        gender = (char)toupper((unsigned char)buffer[0]);
        if (gender == 'M' || gender == 'F' || gender == 'O') {
            return gender;
        }

        printf("Enter M, F, or O.\n");
    }
}

/* reads city, street, and postal code into an Address struct */
static void inputAddress(const char* title, Address* address) {
    printf("%s\n", title);
    readRequiredString("  City: ", address->city, CITY_LENGTH);
    readRequiredString("  Street: ", address->street, STREET_LENGTH);
    readRequiredString("  Postal code: ", address->postCode, POST_CODE_LENGTH);
}

/* reads all citizen fields from the user, then computes age/category/payment */
static void inputCitizen(Citizen* citizen, size_t ordinal) {
    if (citizen == NULL) {
        return;
    }

    memset(citizen, 0, sizeof(Citizen));
    printf("\nCitizen %zu\n", ordinal);
    readRequiredString("Name: ", citizen->name, NAME_LENGTH);
    readRequiredString("Surname: ", citizen->surname, SURNAME_LENGTH);
    citizen->dob = readDateOfBirth();
    citizen->gender = readGender();
    inputAddress("Home address", &citizen->home);
    inputAddress("Work address", &citizen->work);
    updateDerivedCitizenFields(citizen);
}

/* asks the user for a filename */
static void readFileAddress(char* filename, size_t length) {
    readRequiredString("File address/name: ", filename, length);
}

/* saves the stack to a text or binary file based on user choice */
static void registerStackToFile(const Stack* stack) {
    char filename[INPUT_LENGTH];
    int mode;

    mode = readIntRange("Register stack as 1.Text or 2.Binary: ", 1, 2);
    readFileAddress(filename, sizeof(filename));

    if (mode == 1) {
        saveStackToTextFile(stack, filename);
    } else {
        saveStackToBinaryFile(stack, filename);
    }
}

/* saves the queue to a text or binary file based on user choice */
static void registerQueueToFile(const Queue* queue) {
    char filename[INPUT_LENGTH];
    int mode;

    mode = readIntRange("Register queue as 1.Text or 2.Binary: ", 1, 2);
    readFileAddress(filename, sizeof(filename));

    if (mode == 1) {
        saveQueueToTextFile(queue, filename);
    } else {
        saveQueueToBinaryFile(queue, filename);
    }
}

/* asks if the user wants to save the stack to a file right now */
static void offerStackRegistration(const Stack* stack) {
    if (readYesNo("Register the current stack in a file now? (y/n): ")) {
        registerStackToFile(stack);
    }
}

/* asks if the user wants to save the queue to a file right now */
static void offerQueueRegistration(const Queue* queue) {
    if (readYesNo("Register the current queue in a file now? (y/n): ")) {
        registerQueueToFile(queue);
    }
}

/* reads N citizens from the user and rebuilds the stack from them */
static void createStackFromRecords(Stack* stack) {
    Citizen* records;
    size_t count;
    size_t index;

    count = (size_t)readIntRange("Number of stack records: ", 1, MAX_RECORDS);
    records = (Citizen*)malloc(count * sizeof(Citizen));
    if (records == NULL) {
        printf("Memory allocation failed. Stack creation was cancelled.\n");
        return;
    }

    for (index = 0; index < count; index++) {
        inputCitizen(&records[index], index + 1);
    }

    clearStack(stack);
    for (index = 0; index < count; index++) {
        pushStack(stack, &records[index]);
    }

    free(records);
    printf("Stack created with %zu records. The last entered record is on top.\n", stackSize(stack));
    offerStackRegistration(stack);
}

/* reads one citizen and pushes it onto the stack */
static void pushOneCitizen(Stack* stack) {
    Citizen citizen;

    inputCitizen(&citizen, stackSize(stack) + 1);
    if (pushStack(stack, &citizen)) {
        printf("Citizen pushed to the stack.\n");
        offerStackRegistration(stack);
    }
}

/* pops the top citizen and prints their details */
static void popOneCitizen(Stack* stack) {
    Citizen removed;

    if (popStack(stack, &removed)) {
        printf("Removed from stack top:\n");
        printCitizenDetails(&removed);
        offerStackRegistration(stack);
    }
}

/* asks for a position and shows the citizen at that spot in the stack */
static void searchStackPositionMenu(const Stack* stack) {
    Citizen found;
    int position;

    if (isStackEmpty(stack)) {
        printf("Stack is empty.\n");
        return;
    }

    position = readIntRange("Position from top: ", 1, (int)stackSize(stack));
    if (searchStackByPosition(stack, (size_t)position, &found)) {
        printCitizenDetails(&found);
    } else {
        printf("No stack record found at that position.\n");
    }
}

/* asks for a surname and searches the stack for it */
static void searchStackSurnameMenu(const Stack* stack) {
    char surname[SURNAME_LENGTH];
    Citizen found;
    size_t position;

    readRequiredString("Surname to search: ", surname, sizeof(surname));
    if (searchStackBySurname(stack, surname, &found, &position)) {
        printf("Found at position %zu from top.\n", position);
        printCitizenDetails(&found);
    } else {
        printf("No stack record found for surname: %s\n", surname);
    }
}

/* asks for a position and deletes the citizen at that spot */
static void deleteStackPositionMenu(Stack* stack) {
    Citizen removed;
    int position;

    if (isStackEmpty(stack)) {
        printf("Stack is empty.\n");
        return;
    }

    position = readIntRange("Position to delete from top: ", 1, (int)stackSize(stack));
    if (deleteStackByPosition(stack, (size_t)position, &removed)) {
        printf("Deleted stack record:\n");
        printCitizenDetails(&removed);
        offerStackRegistration(stack);
    } else {
        printf("Delete failed.\n");
    }
}

/* asks for a surname and deletes the matching citizen from the stack */
static void deleteStackSurnameMenu(Stack* stack) {
    char surname[SURNAME_LENGTH];
    Citizen removed;
    size_t position;

    readRequiredString("Surname to delete: ", surname, sizeof(surname));
    if (deleteStackBySurname(stack, surname, &removed, &position)) {
        printf("Deleted record at position %zu from top:\n", position);
        printCitizenDetails(&removed);
        offerStackRegistration(stack);
    } else {
        printf("No stack record found for surname: %s\n", surname);
    }
}

/* loads stack data from a binary file, optionally replacing what's there */
static void loadStackMenu(Stack* stack) {
    char filename[INPUT_LENGTH];
    int replace;

    readFileAddress(filename, sizeof(filename));
    replace = readYesNo("Replace the current stack before loading? (y/n): ");
    loadStackFromBinaryFile(stack, filename, replace);
}

/* the stack submenu loop with all stack operations */
static void versionAStackMenu(Stack* stack) {
    int choice;
    char filename[INPUT_LENGTH];

    while (1) {
        printf("\n================ VERSION A =================\n");
        printf("Dynamic Stack based on List ADT\n");
        printf("1. Create or replace stack from N records\n");
        printf("2. Insert element: push to stack\n");
        printf("3. Delete element: pop from stack\n");
        printf("4. Traverse and display stack\n");
        printf("5. Search by position\n");
        printf("6. Search by surname\n");
        printf("7. Delete by position\n");
        printf("8. Delete by surname\n");
        printf("9. Save stack to text file\n");
        printf("10. Save stack to binary file\n");
        printf("11. Load stack from binary file\n");
        printf("0. Back to main menu\n");

        choice = readIntRange("Choice: ", 0, 11);
        switch (choice) {
            case 1:
                createStackFromRecords(stack);
                break;
            case 2:
                pushOneCitizen(stack);
                break;
            case 3:
                popOneCitizen(stack);
                break;
            case 4:
                displayStack(stack);
                break;
            case 5:
                searchStackPositionMenu(stack);
                break;
            case 6:
                searchStackSurnameMenu(stack);
                break;
            case 7:
                deleteStackPositionMenu(stack);
                break;
            case 8:
                deleteStackSurnameMenu(stack);
                break;
            case 9:
                readFileAddress(filename, sizeof(filename));
                saveStackToTextFile(stack, filename);
                break;
            case 10:
                readFileAddress(filename, sizeof(filename));
                saveStackToBinaryFile(stack, filename);
                break;
            case 11:
                loadStackMenu(stack);
                break;
            case 0:
                return;
            default:
                break;
        }
    }
}

/* lets the user pick which queue type to work with */
static QueueType readQueueType(void) {
    printf("\nQueue type\n");
    printf("1. Simple Queue\n");
    printf("2. Double Ended Queue\n");
    printf("3. Circular Queue\n");
    printf("4. Priority Queue\n");
    return (QueueType)readIntRange("Select queue type: ", 1, 4);
}

/* returns a pointer to the right queue from the array based on type */
static Queue* queueByType(Queue queues[], QueueType type) {
    return &queues[(int)type - 1];
}

/* asks the user for a priority value (1 = highest) */
static int readPriority(void) {
    return readIntRange("Priority (1 = highest priority): ", 1, 1000000);
}

/* reads N citizens (and priorities if needed) and rebuilds the queue */
static void createQueueFromRecords(Queue* queue) {
    Citizen* records;
    int* priorities;
    size_t count;
    size_t index;

    count = (size_t)readIntRange("Number of queue records: ", 1, MAX_RECORDS);
    records = (Citizen*)malloc(count * sizeof(Citizen));
    priorities = (int*)malloc(count * sizeof(int));

    if (records == NULL || priorities == NULL) {
        free(records);
        free(priorities);
        printf("Memory allocation failed. Queue creation was cancelled.\n");
        return;
    }

    for (index = 0; index < count; index++) {
        inputCitizen(&records[index], index + 1);
        priorities[index] = queue->type == QUEUE_PRIORITY ? records[index].category : 0;
    }

    clearQueue(queue);
    for (index = 0; index < count; index++) {
        enqueueQueue(queue, &records[index], priorities[index]);
    }

    free(records);
    free(priorities);
    printf("%s created with %zu records.\n", queueTypeName(queue->type), queueSize(queue));
    offerQueueRegistration(queue);
}

/* reads one citizen and inserts them into the queue.
   for deque: asks front or rear. for priority queue: auto-assigns from category */
static void enqueueOneCitizen(Queue* queue) {
    Citizen citizen;
    int option;
    int priority;

    inputCitizen(&citizen, queueSize(queue) + 1);

    if (queue->type == QUEUE_DEQUE) {
        option = readIntRange("Insert at 1.Front or 2.Rear: ", 1, 2);
        if (option == 1) {
            if (enqueueQueueFront(queue, &citizen)) {
                printf("Citizen inserted at the deque front.\n");
                offerQueueRegistration(queue);
            }
        } else if (enqueueQueue(queue, &citizen, 0)) {
            printf("Citizen inserted at the deque rear.\n");
            offerQueueRegistration(queue);
        }
        return;
    }

    priority = queue->type == QUEUE_PRIORITY ? citizen.category : 0;
    if (enqueueQueue(queue, &citizen, priority)) {
        printf("Citizen inserted into %s.\n", queueTypeName(queue->type));
        offerQueueRegistration(queue);
    }
}

/* removes one citizen from the queue.
   for deque: asks if removing from front or rear */
static void dequeueOneCitizen(Queue* queue) {
    Citizen removed;
    int option;

    if (queue->type == QUEUE_DEQUE) {
        option = readIntRange("Delete from 1.Front or 2.Rear: ", 1, 2);
        if (option == 2) {
            if (dequeueQueueRear(queue, &removed)) {
                printf("Removed from deque rear:\n");
                printCitizenDetails(&removed);
                offerQueueRegistration(queue);
            }
            return;
        }
    }

    if (dequeueQueue(queue, &removed)) {
        printf("Removed from queue front:\n");
        printCitizenDetails(&removed);
        offerQueueRegistration(queue);
    }
}

/* asks for a position and shows the citizen at that spot in the queue */
static void searchQueuePositionMenu(const Queue* queue) {
    Citizen found;
    int priority;
    int position;

    if (isQueueEmpty(queue)) {
        printf("%s is empty.\n", queueTypeName(queue->type));
        return;
    }

    position = readIntRange("Position from front: ", 1, (int)queueSize(queue));
    if (searchQueueByPosition(queue, (size_t)position, &found, &priority)) {
        if (queue->type == QUEUE_PRIORITY) {
            printf("Priority: %d\n", priority);
        }
        printCitizenDetails(&found);
    } else {
        printf("No queue record found at that position.\n");
    }
}

/* asks for a surname and searches the queue for it */
static void searchQueueSurnameMenu(const Queue* queue) {
    char surname[SURNAME_LENGTH];
    Citizen found;
    size_t position;
    int priority;

    readRequiredString("Surname to search: ", surname, sizeof(surname));
    if (searchQueueBySurname(queue, surname, &found, &position, &priority)) {
        printf("Found at position %zu from front.\n", position);
        if (queue->type == QUEUE_PRIORITY) {
            printf("Priority: %d\n", priority);
        }
        printCitizenDetails(&found);
    } else {
        printf("No queue record found for surname: %s\n", surname);
    }
}

/* asks for a position and deletes the citizen at that spot in the queue */
static void deleteQueuePositionMenu(Queue* queue) {
    Citizen removed;
    int priority;
    int position;

    if (isQueueEmpty(queue)) {
        printf("%s is empty.\n", queueTypeName(queue->type));
        return;
    }

    position = readIntRange("Position to delete from front: ", 1, (int)queueSize(queue));
    if (deleteQueueByPosition(queue, (size_t)position, &removed, &priority)) {
        printf("Deleted queue record:\n");
        if (queue->type == QUEUE_PRIORITY) {
            printf("Priority: %d\n", priority);
        }
        printCitizenDetails(&removed);
        offerQueueRegistration(queue);
    } else {
        printf("Delete failed.\n");
    }
}

/* asks for a surname and deletes the matching citizen from the queue */
static void deleteQueueSurnameMenu(Queue* queue) {
    char surname[SURNAME_LENGTH];
    Citizen removed;
    size_t position;
    int priority;

    readRequiredString("Surname to delete: ", surname, sizeof(surname));
    if (deleteQueueBySurname(queue, surname, &removed, &position, &priority)) {
        printf("Deleted record at position %zu from front:\n", position);
        if (queue->type == QUEUE_PRIORITY) {
            printf("Priority: %d\n", priority);
        }
        printCitizenDetails(&removed);
        offerQueueRegistration(queue);
    } else {
        printf("No queue record found for surname: %s\n", surname);
    }
}

/* loads queue data from a binary file, optionally replacing what's there */
static void loadQueueMenu(Queue* queue) {
    char filename[INPUT_LENGTH];
    int replace;

    readFileAddress(filename, sizeof(filename));
    replace = readYesNo("Replace the current queue before loading? (y/n): ");
    loadQueueFromBinaryFile(queue, filename, replace);
}

/* the queue submenu loop, user picks a queue type and does operations on it */
static void versionBQueueMenu(Queue queues[]) {
    QueueType activeType = readQueueType();
    Queue* activeQueue = queueByType(queues, activeType);
    int choice;
    char filename[INPUT_LENGTH];

    while (1) {
        printf("\n================ VERSION B =================\n");
        printf("%s based on List ADT\n", queueTypeName(activeType));
        printf("1. Switch queue type\n");
        printf("2. Create or replace selected queue from N records\n");
        printf("3. Insert element into selected queue\n");
        printf("4. Delete element from selected queue\n");
        printf("5. Traverse and display selected queue\n");
        printf("6. Search by position\n");
        printf("7. Search by surname\n");
        printf("8. Delete by position\n");
        printf("9. Delete by surname\n");
        printf("10. Save selected queue to text file\n");
        printf("11. Save selected queue to binary file\n");
        printf("12. Load selected queue from binary file\n");
        printf("0. Back to main menu\n");

        choice = readIntRange("Choice: ", 0, 12);
        switch (choice) {
            case 1:
                activeType = readQueueType();
                activeQueue = queueByType(queues, activeType);
                break;
            case 2:
                createQueueFromRecords(activeQueue);
                break;
            case 3:
                enqueueOneCitizen(activeQueue);
                break;
            case 4:
                dequeueOneCitizen(activeQueue);
                break;
            case 5:
                displayQueue(activeQueue);
                break;
            case 6:
                searchQueuePositionMenu(activeQueue);
                break;
            case 7:
                searchQueueSurnameMenu(activeQueue);
                break;
            case 8:
                deleteQueuePositionMenu(activeQueue);
                break;
            case 9:
                deleteQueueSurnameMenu(activeQueue);
                break;
            case 10:
                readFileAddress(filename, sizeof(filename));
                saveQueueToTextFile(activeQueue, filename);
                break;
            case 11:
                readFileAddress(filename, sizeof(filename));
                saveQueueToBinaryFile(activeQueue, filename);
                break;
            case 12:
                loadQueueMenu(activeQueue);
                break;
            case 0:
                return;
            default:
                break;
        }
    }
}

/* submenu for file operations: create, open, view, or delete data files */
static void fileToolsMenu(void) {
    int choice;
    char filename[INPUT_LENGTH];

    while (1) {
        printf("\n================ FILE TOOLS =================\n");
        printf("1. Create empty text file\n");
        printf("2. Create empty binary file\n");
        printf("3. Open or reopen text file\n");
        printf("4. Open or reopen binary stack file\n");
        printf("5. Open or reopen binary queue file\n");
        printf("6. Delete file\n");
        printf("0. Back to main menu\n");

        choice = readIntRange("Choice: ", 0, 6);
        switch (choice) {
            case 1:
                readFileAddress(filename, sizeof(filename));
                createEmptyDataFile(filename, 0);
                break;
            case 2:
                readFileAddress(filename, sizeof(filename));
                createEmptyDataFile(filename, 1);
                break;
            case 3:
                readFileAddress(filename, sizeof(filename));
                displayTextFile(filename);
                break;
            case 4:
                readFileAddress(filename, sizeof(filename));
                displayStackBinaryFile(filename);
                break;
            case 5:
                readFileAddress(filename, sizeof(filename));
                displayQueueBinaryFile(filename);
                break;
            case 6:
                readFileAddress(filename, sizeof(filename));
                deleteDataFile(filename);
                break;
            case 0:
                return;
            default:
                break;
        }
    }
}

/* main entry point: sets up the stack and 4 queue types, runs the menu loop */
int main(void) {
    Stack stackVersionA;
    Queue queuesVersionB[4];   /* one for each queue type: simple, deque, circular, priority */
    int choice;
    size_t index;

    initStack(&stackVersionA);
    initQueue(&queuesVersionB[0], QUEUE_SIMPLE);
    initQueue(&queuesVersionB[1], QUEUE_DEQUE);
    initQueue(&queuesVersionB[2], QUEUE_CIRCULAR);
    initQueue(&queuesVersionB[3], QUEUE_PRIORITY);

    while (1) {
        printf("\n================ MAIN MENU =================\n");
        printf("1. Version A: Dynamic Stack based on List ADT\n");
        printf("2. Version B: Dynamic Queues based on List ADT\n");
        printf("3. File create/open/reopen/delete tools\n");
        printf("0. Exit\n");

        choice = readIntRange("Choice: ", 0, 3);
        switch (choice) {
            case 1:
                versionAStackMenu(&stackVersionA);
                break;
            case 2:
                versionBQueueMenu(queuesVersionB);
                break;
            case 3:
                fileToolsMenu();
                break;
            case 0:
                /* free everything before exiting */
                clearStack(&stackVersionA);
                for (index = 0; index < 4; index++) {
                    clearQueue(&queuesVersionB[index]);
                }
                printf("Memory released. Program finished.\n");
                return 0;
            default:
                break;
        }
    }
}
