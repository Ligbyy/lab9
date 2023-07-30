
#include <stdlib.h>
#include <stdio.h>

// RecordType
struct RecordType
{
    int		key;
    char	name;
    int		order;
};

struct hashNode{//node for hashtype

    struct RecordType record;
    struct hashNode *next;
};

// Fill out this structure
struct HashType
{
    struct hashNode **next;
    int capacity;
};

// Compute the hash function
int hash(int x, int capacity)
{
    return x % capacity;
}

void insert(struct HashType *table, struct RecordType record){
    int key = hash(record.key,table->capacity);
    struct hashNode *newNode = (struct hashNode*) malloc(sizeof (struct hashNode));
    newNode->record = record;
    newNode->next = NULL;

    if(table->next[key] == NULL){
        table->next[key] = newNode;
    } else{
        struct hashNode* curr = table->next[key];
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = newNode;
    }
}
// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->key = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].key, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable, int hashSz)
{

    for (int i = 0; i < hashSz; ++i)
    {
        if (hashTable->next[i] != NULL)
        {
            printf("index %d ->\n", i);
            struct hashNode *current = hashTable->next[i];

            while (current != NULL)
            {
                struct RecordType record = current->record;

                printf("index x -> %d name: %c order: %d ->\n", record.key, record.name, record.order);
                current = current->next;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    recordSz = parseData("C:\\Users\\blueg\\CLionProjects\\9_lab\\input.txt", &pRecords);

    struct HashType hashTable;
    hashTable.capacity = recordSz;
    hashTable.next = (struct hashNode**)malloc(sizeof(struct hashNode*) * recordSz);
    for (int i = 0; i < recordSz; i++) {
        hashTable.next[i] = NULL;
    }
    for (int i = 0; i < recordSz; i++) {
        insert(&hashTable, pRecords[i]);
    }
    displayRecordsInHash(&hashTable, hashTable.capacity);
    printRecords(pRecords, recordSz);

    // Clean up and free memory
    for (int i = 0; i < recordSz; i++) {
        struct hashNode* current = hashTable.next[i];
        while (current != NULL) {
            struct hashNode* next = current->next;
            free(current);
            current = next;
        }
    }
    free(hashTable.next);
    free(pRecords);

    return 0;
}
