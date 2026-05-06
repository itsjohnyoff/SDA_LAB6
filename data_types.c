#include <stdio.h>
#include "data_types.h"

const char* citizenCategoryName(int category) {
    switch (category) {
        case CATEGORY_CHILD:
            return "Child";
        case CATEGORY_ADULT:
            return "Adult";
        case CATEGORY_SENIOR:
            return "Senior";
        default:
            return "Unknown";
    }
}

void printCitizenBrief(const Citizen* citizen, size_t position, int priority, int showPriority) {
    if (citizen == NULL) {
        return;
    }

    printf("%3zu. %-16s %-16s | DOB: %02d/%02d/%04d | Age: %02dY %02dM %02dD | %s",
           position,
           citizen->name,
           citizen->surname,
           citizen->dob.day,
           citizen->dob.month,
           citizen->dob.year,
           citizen->ageYears,
           citizen->ageMonths,
           citizen->ageDays,
           citizenCategoryName(citizen->category));

    if (showPriority) {
        printf(" | Priority: %d", priority);
    }

    printf("\n");
}

void printCitizenDetails(const Citizen* citizen) {
    if (citizen == NULL) {
        return;
    }

    printf("\nCitizen record\n");
    printf("Name: %s %s\n", citizen->name, citizen->surname);
    printf("Date of birth: %02d/%02d/%04d\n",
           citizen->dob.day,
           citizen->dob.month,
           citizen->dob.year);
    printf("Gender: %c\n", citizen->gender);
    printf("Home address: %s, %s, %s\n",
           citizen->home.city,
           citizen->home.street,
           citizen->home.postCode);
    printf("Work address: %s, %s, %s\n",
           citizen->work.city,
           citizen->work.street,
           citizen->work.postCode);
    printf("Computed age: %d years, %d months, %d days\n",
           citizen->ageYears,
           citizen->ageMonths,
           citizen->ageDays);
    printf("Category: %s\n", citizenCategoryName(citizen->category));
    printf("Amount paid: %.2f\n", citizen->amountPaid);
}
