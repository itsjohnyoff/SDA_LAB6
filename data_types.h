#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stddef.h>

#define NAME_LENGTH 50
#define SURNAME_LENGTH 50
#define CITY_LENGTH 50
#define STREET_LENGTH 50
#define POST_CODE_LENGTH 20

typedef enum {
    CATEGORY_CHILD = 1,
    CATEGORY_ADULT = 2,
    CATEGORY_SENIOR = 3
} CitizenCategory;

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char city[CITY_LENGTH];
    char street[STREET_LENGTH];
    char postCode[POST_CODE_LENGTH];
} Address;

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

typedef struct Node {
    Citizen data;
    int priority;
    struct Node* next;
    struct Node* prev;
} Node;

const char* citizenCategoryName(int category);
void printCitizenBrief(const Citizen* citizen, size_t position, int priority, int showPriority);
void printCitizenDetails(const Citizen* citizen);

#endif
