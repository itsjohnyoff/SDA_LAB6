#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stddef.h>

/* max lengths for citizen string fields */
#define NAME_LENGTH 50
#define SURNAME_LENGTH 50
#define CITY_LENGTH 50
#define STREET_LENGTH 50
#define POST_CODE_LENGTH 20

/* age-based citizen category: child (<18), adult (18-59), senior (60+) */
typedef enum {
    CATEGORY_CHILD = 1,
    CATEGORY_ADULT = 2,
    CATEGORY_SENIOR = 3
} CitizenCategory;

/* day/month/year for date of birth */
typedef struct {
    int day;
    int month;
    int year;
} Date;

/* city, street, postal code — used for home and work */
typedef struct {
    char city[CITY_LENGTH];
    char street[STREET_LENGTH];
    char postCode[POST_CODE_LENGTH];
} Address;

/* one citizen record with personal data, addresses, and computed fields */
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

/* doubly-linked list node, holds a Citizen and optional priority */
typedef struct Node {
    Citizen data;
    int priority;
    struct Node* next;
    struct Node* prev;
} Node;

/* print helpers for citizen records */
const char* citizenCategoryName(int category);
void printCitizenBrief(const Citizen* citizen, size_t position, int priority, int showPriority);
void printCitizenDetails(const Citizen* citizen);

#endif
