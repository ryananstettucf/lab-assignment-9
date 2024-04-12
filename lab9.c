#include <stdio.h>
#include <stdlib.h>

#define MAX_HASH_SIZE 10

struct RecordType {
    int id;
    char name;
    int order;
    struct RecordType* next;
};

struct HashType {
    struct RecordType* head;
};

int hash(int x, int hashSize) {
    return x % hashSize; 
}

// prints the records
void printRecords(struct RecordType pData[], int dataSize) {
    printf("\nRecords:\n");
    for (int i = 0; i < dataSize; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n");
}
void addNewRecord(struct HashType* hashTable, int hashSize, struct RecordType newRecord) {
    int index = hash(newRecord.id, hashSize);
    struct RecordType* newNode = (struct RecordType*) malloc(sizeof(struct RecordType));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    *newNode = newRecord;
    newNode->next = hashTable[index].head;
    hashTable[index].head = newNode;
}

void displayRecordsInHash(struct HashType* hashTable, int hashSize) {
    for (int i = 0; i < hashSize; ++i) {
        struct RecordType* current = hashTable[i].head;
        printf("Index %d -> ", i);
        while (current != NULL) {
            printf("id: %d, name: %c, order: %d -> ", current->id, current->name, current->order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int parseData(char* inputFileName, struct RecordType** ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    if (inFile == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return 0;
    }

    int dataSize = 0;
    if (fscanf(inFile, "%d\n", &dataSize) != 1) {
        fprintf(stderr, "Failed to read data size\n");
        fclose(inFile);
        return 0;
    }

    *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSize);
    if (*ppData == NULL) {
        fprintf(stderr, "Cannot allocate memory\n");
        fclose(inFile);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dataSize; ++i) {
        if (fscanf(inFile, "%d %c %d", &((*ppData)[i].id), &((*ppData)[i].name), &((*ppData)[i].order)) != 3) {
            fprintf(stderr, "Failed to read record %d\n", i);
            free(*ppData);
            fclose(inFile);
            return 0;
        }
        (*ppData)[i].next = NULL;
    }

    fclose(inFile);
    return dataSize;
}

void freeHashTable(struct HashType* hashTable, int hashSize) {
    for (int i = 0; i < hashSize; i++) {
        struct RecordType* current = hashTable[i].head;
        while (current != NULL) {
            struct RecordType* temp = current;
            current = current->next;
            free(temp);
        }
    }
}

int main(void) {
    struct RecordType *pRecords;
    int recordSz = parseData("input_lab_9.txt", &pRecords);
    if (recordSz == 0) {
        return EXIT_FAILURE; // Error parsing data, already handled in parseData
    }

    printRecords(pRecords, recordSz);
    struct HashType* hashTable = malloc(sizeof(struct HashType) * MAX_HASH_SIZE);
    if (hashTable == NULL) {
        fprintf(stderr, "Memory allocation failed for hash table\n");
        free(pRecords);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < MAX_HASH_SIZE; i++) {
        hashTable[i].head = NULL;
    }

    for (int i = 0; i < recordSz; i++) {
        addNewRecord(hashTable, MAX_HASH_SIZE, pRecords[i]);
    }

    displayRecordsInHash(hashTable, MAX_HASH_SIZE);

    freeHashTable(hashTable, MAX_HASH_SIZE);
    free(hashTable);
    free(pRecords);

    return 0;
}
