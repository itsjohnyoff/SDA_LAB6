#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stddef.h>

/* Field sizes used across citizen input and file output. */
#define NAME_LENGTH 50
#define SURNAME_LENGTH 50
#define CITY_LENGTH 50
#define STREET_LENGTH 50
#define POST_CODE_LENGTH 20

/* Basic age category for a citizen. */
typedef enum {
    CATEGORY_CHILD = 1,
    CATEGORY_ADULT = 2,
    CATEGORY_SENIOR = 3
} CitizenCategory;

/* Simple date record for birth date handling. */
typedef struct {
    int day;
    int month;
    int year;
} Date;

/* Reused for both home and work addresses. */
typedef struct {
    char city[CITY_LENGTH];
    char street[STREET_LENGTH];
    char postCode[POST_CODE_LENGTH];
} Address;

/* Main record processed by the stack and queue menus. */
typedef struct {
    char name[NAME_LENGTH];
    char surname[SURNAME_LENGTH];
    Date dob;
    char gender;
    Address home;
    Address work;

    int ageYears;
    int ageMonths;
    int ageDays;
    int category;
    float amountPaid;
} Citizen;

/* Linked-list node shared by stack and queue implementations. */
typedef struct Node {
    Citizen data;
    int priority;
    struct Node* next;
    struct Node* prev;
} Node;

/* Display helpers used by the menu and file modules. */
const char* citizenCategoryName(int category);
void printCitizenBrief(const Citizen* citizen, size_t position, int priority, int showPriority);
void printCitizenDetails(const Citizen* citizen);

#endif
