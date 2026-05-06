#include <stdio.h>
#include <stdlib.h>
#include "file_io.h"

typedef struct {
    Citizen data;
    int priority;
} QueueFileRecord;

static void writeCitizenText(FILE* file, const Citizen* citizen, int priority, int showPriority) {
    fprintf(file,
            "%s %s | DOB: %02d/%02d/%04d | Gender: %c | Age: %dY %dM %dD | Category: %s | Paid: %.2f",
            citizen->name,
            citizen->surname,
            citizen->dob.day,
            citizen->dob.month,
            citizen->dob.year,
            citizen->gender,
            citizen->ageYears,
            citizen->ageMonths,
            citizen->ageDays,
            citizenCategoryName(citizen->category),
            citizen->amountPaid);

    if (showPriority) {
        fprintf(file, " | Priority: %d", priority);
    }

    fprintf(file,
            " | Home: %s, %s, %s | Work: %s, %s, %s\n",
            citizen->home.city,
            citizen->home.street,
            citizen->home.postCode,
            citizen->work.city,
            citizen->work.street,
            citizen->work.postCode);
}

int createEmptyDataFile(const char* filename, int binaryMode) {
    FILE* file;

    if (filename == NULL) {
        return 0;
    }

    file = fopen(filename, binaryMode ? "wb" : "w");
    if (file == NULL) {
        printf("Could not create file: %s\n", filename);
        return 0;
    }

    fclose(file);
    printf("File created: %s\n", filename);
    return 1;
}

int saveStackToTextFile(const Stack* s, const char* filename) {
    FILE* file;
    Node* current;
    size_t position;

    if (s == NULL || filename == NULL) {
        return 0;
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open file for text write: %s\n", filename);
        return 0;
    }

    fprintf(file, "VERSION A - Dynamic Stack based on List ADT\n");
    fprintf(file, "Records: %zu\n\n", s->size);

    current = s->top;
    position = 1;
    while (current != NULL) {
        fprintf(file, "%zu. ", position);
        writeCitizenText(file, &current->data, 0, 0);
        current = current->next;
        position++;
    }

    fclose(file);
    printf("Stack saved in text mode: %s\n", filename);
    return 1;
}

int saveStackToBinaryFile(const Stack* s, const char* filename) {
    FILE* file;
    Node* current;

    if (s == NULL || filename == NULL) {
        return 0;
    }

    file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Could not open file for binary write: %s\n", filename);
        return 0;
    }

    current = s->top;
    while (current != NULL) {
        fwrite(&current->data, sizeof(Citizen), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Stack saved in binary mode: %s\n", filename);
    return 1;
}

int loadStackFromBinaryFile(Stack* s, const char* filename, int replaceExisting) {
    FILE* file;
    Citizen* records;
    Citizen* resized;
    Citizen item;
    size_t count;
    size_t capacity;
    size_t index;

    if (s == NULL || filename == NULL) {
        return 0;
    }

    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Could not open binary stack file: %s\n", filename);
        return 0;
    }

    capacity = 8;
    count = 0;
    records = (Citizen*)malloc(capacity * sizeof(Citizen));
    if (records == NULL) {
        fclose(file);
        printf("Memory allocation failed while loading stack file.\n");
        return 0;
    }

    while (fread(&item, sizeof(Citizen), 1, file) == 1) {
        if (count == capacity) {
            capacity *= 2;
            resized = (Citizen*)realloc(records, capacity * sizeof(Citizen));
            if (resized == NULL) {
                free(records);
                fclose(file);
                printf("Memory allocation failed while expanding stack load buffer.\n");
                return 0;
            }
            records = resized;
        }

        records[count++] = item;
    }

    fclose(file);

    if (replaceExisting) {
        clearStack(s);
    }

    for (index = count; index > 0; index--) {
        if (!pushStack(s, &records[index - 1])) {
            free(records);
            printf("Stack load stopped because an insert failed.\n");
            return 0;
        }
    }

    free(records);
    printf("Loaded %zu stack records from: %s\n", count, filename);
    return 1;
}

int saveQueueToTextFile(const Queue* q, const char* filename) {
    FILE* file;
    Node* current;
    size_t position;
    int showPriority;

    if (q == NULL || filename == NULL) {
        return 0;
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open file for text write: %s\n", filename);
        return 0;
    }

    showPriority = q->type == QUEUE_PRIORITY;
    fprintf(file, "VERSION B - %s based on List ADT\n", queueTypeName(q->type));
    fprintf(file, "Records: %zu\n\n", q->size);

    current = q->front;
    for (position = 1; position <= q->size; position++) {
        fprintf(file, "%zu. ", position);
        writeCitizenText(file, &current->data, current->priority, showPriority);
        current = current->next;
    }

    fclose(file);
    printf("Queue saved in text mode: %s\n", filename);
    return 1;
}

int saveQueueToBinaryFile(const Queue* q, const char* filename) {
    FILE* file;
    Node* current;
    QueueFileRecord record;
    size_t position;

    if (q == NULL || filename == NULL) {
        return 0;
    }

    file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Could not open file for binary write: %s\n", filename);
        return 0;
    }

    current = q->front;
    for (position = 1; position <= q->size; position++) {
        record.data = current->data;
        record.priority = current->priority;
        fwrite(&record, sizeof(QueueFileRecord), 1, file);
        current = current->next;
    }

    fclose(file);
    printf("Queue saved in binary mode: %s\n", filename);
    return 1;
}

int loadQueueFromBinaryFile(Queue* q, const char* filename, int replaceExisting) {
    FILE* file;
    QueueFileRecord record;
    size_t count;

    if (q == NULL || filename == NULL) {
        return 0;
    }

    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Could not open binary queue file: %s\n", filename);
        return 0;
    }

    if (replaceExisting) {
        clearQueue(q);
    }

    count = 0;
    while (fread(&record, sizeof(QueueFileRecord), 1, file) == 1) {
        if (!enqueueQueue(q, &record.data, record.priority)) {
            fclose(file);
            printf("Queue load stopped because an insert failed.\n");
            return 0;
        }
        count++;
    }

    fclose(file);
    printf("Loaded %zu queue records from: %s\n", count, filename);
    return 1;
}

int displayTextFile(const char* filename) {
    FILE* file;
    int ch;

    if (filename == NULL) {
        return 0;
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open text file: %s\n", filename);
        return 0;
    }

    printf("\n--- Text file: %s ---\n", filename);
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    printf("\n--- End of text file ---\n");

    fclose(file);
    return 1;
}

int displayStackBinaryFile(const char* filename) {
    FILE* file;
    Citizen item;
    size_t position;

    if (filename == NULL) {
        return 0;
    }

    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Could not open binary stack file: %s\n", filename);
        return 0;
    }

    printf("\n--- Binary stack file: %s ---\n", filename);
    position = 1;
    while (fread(&item, sizeof(Citizen), 1, file) == 1) {
        printCitizenBrief(&item, position, 0, 0);
        position++;
    }
    printf("--- End of binary stack file ---\n");

    fclose(file);
    return 1;
}

int displayQueueBinaryFile(const char* filename) {
    FILE* file;
    QueueFileRecord record;
    size_t position;

    if (filename == NULL) {
        return 0;
    }

    file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Could not open binary queue file: %s\n", filename);
        return 0;
    }

    printf("\n--- Binary queue file: %s ---\n", filename);
    position = 1;
    while (fread(&record, sizeof(QueueFileRecord), 1, file) == 1) {
        printCitizenBrief(&record.data, position, record.priority, 1);
        position++;
    }
    printf("--- End of binary queue file ---\n");

    fclose(file);
    return 1;
}

int deleteDataFile(const char* filename) {
    if (filename == NULL) {
        return 0;
    }

    if (remove(filename) == 0) {
        printf("File deleted: %s\n", filename);
        return 1;
    }

    printf("Could not delete file: %s\n", filename);
    return 0;
}
