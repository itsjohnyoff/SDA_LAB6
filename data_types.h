#ifndef DATA_TYPES_H
#define DATA_TYPES_H

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char city[50];
    char street[50];
    char postCode[20];
} Address;

typedef struct {
    char name[50];
    char surname[50];
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

// Node for our Linked List based Stack and Queues
typedef struct Node {
    Citizen data;
    int priority;      // Specifically for Priority Queue
    struct Node* next;
    struct Node* prev; // Specifically for Double Ended Queue
} Node;

#endif