#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#include "KeyValuePair.h"
#include "LinkedList.h"

// definition of the Hash Table structure
typedef struct hashTable_s {
    LinkedList* tableList; //initialize an array of linked lists (pointer to lists of pointers to Linked Lists)
    int tableSize; // initialize size
    CopyFunction copyKey;
    CopyFunction copyValue;
    FreeFunction freeKey;
    FreeFunction freeValue;
    PrintFunction printKey;
    PrintFunction printValue;
    EqualFunction equalKey;
    TransformIntoNumberFunction keyToNum;
} hashTable_s ;

//Helper Functions to help Copy , Free , Print and Compare Key Value Pair element:

Element copyKeyValuePair(Element element) {
    if (!element) {
        return NULL;
    }
    keyValuePair pair = (keyValuePair)element;
    return pair;
}

status printPairKVP(Element element) {
    if (!element) {
        return failure;
    }
    keyValuePair pairToPrint = (keyValuePair)element;
    if (displayKey(pairToPrint) == failure) {
        return failure;
    }
    if (displayValue(pairToPrint) == failure) {
        return failure;
    }
    return success;
}

status destroyPairKVP(Element element) {
    if (element == NULL) {
        return failure;
    }
    keyValuePair pair = (keyValuePair)element;
    if (destroyKeyValuePair(pair) == failure) {
        return failure;
    }
    return success;
}

bool pairCompare(Element elem1, Element elem2) {
    if (!elem1 || !elem2) {
        return false;
    }
    keyValuePair pair = (keyValuePair)elem1;
    if (isEqualKey(pair, elem2) == true) {
        return true;
    }
    return false;
}

//create new hash table
hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber, int hashNumber) {
    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue || !printValue || !equalKey || !transformIntoNumber || hashNumber <= 0 ) {
        return NULL;
    }
    hashTable newHashTable = (hashTable)malloc(sizeof(hashTable_s)); //allocate place in memory for the structure
    if (!newHashTable) {
        return NULL;
    }
    newHashTable->tableList = (LinkedList*)malloc(sizeof(LinkedList) * hashNumber); // allocate place in memory for Linked List structure
    if (!newHashTable->tableList) {
        free(newHashTable);
        return NULL;
    }

    for (int i = 0; i < hashNumber; i ++) { // make each table in hash table to point to Linked list structure to handle case of collision by chaining
        newHashTable->tableList[i] = createLinkedList(copyKeyValuePair, destroyPairKVP, printPairKVP, pairCompare);
        if (!newHashTable->tableList[i]) {
            for (int j = 0; j < i; j++) {
                destroyList(newHashTable->tableList[j]);
                newHashTable->tableList[j] = NULL;
            }
            free(newHashTable->tableList);
            newHashTable->tableList = NULL;
            free(newHashTable);
            newHashTable = NULL;
            return NULL;
        }
    }
    // set all fields of hash table (user functions)
    newHashTable->tableSize = hashNumber;
    newHashTable->copyKey = copyKey;
    newHashTable->freeKey = freeKey;
    newHashTable->printKey = printKey;
    newHashTable->copyValue = copyValue;
    newHashTable->freeValue = freeValue;
    newHashTable->printValue = printValue;
    newHashTable->equalKey = equalKey;
    newHashTable->keyToNum = transformIntoNumber;
    return newHashTable; //return the new hash table
}

// func to destroy the structure and all elements inside
status destroyHashTable(hashTable h_table) {
    if (!h_table) {
        return failure;
    }
    for (int i = 0; i < h_table->tableSize; i ++) {
        if (h_table->tableList[i]) { //for each Linked List that pointed by table - destroy with all elements
            destroyList(h_table->tableList[i]);
            h_table->tableList[i] = NULL;
        }
    }
    free(h_table->tableList);
    h_table->tableList = NULL;
    free(h_table); //free the structure
    h_table = NULL;
    return success;
}

// add to hash func
status addToHashTable(hashTable hash_table, Element key, Element value) {
    if (!hash_table || !key || !value) {
        return failure;
    }
    int index = hash_table->keyToNum(key) % hash_table->tableSize; // find the index to store the key according to user func of transform into num

    if (searchByKeyInList(hash_table->tableList[index], key) != NULL) {
        return failure;
    }

    //create new pair to store the new elem in hash table
    keyValuePair newPair = createKeyValuePair((CopyFunction)hash_table->copyKey, (FreeFunction)hash_table->freeKey, (PrintFunction)hash_table->printKey,(CopyFunction)hash_table->copyValue, (FreeFunction)hash_table->freeValue, (PrintFunction)hash_table->printValue, (EqualFunction)hash_table->equalKey, key, value);
    if (!newPair) {
        return failure;
    }

    if (appendNode(hash_table->tableList[index], newPair) == failure) {
        destroyKeyValuePair(newPair);
        return failure;
    }
    return success;
}

//look up in hash table
Element lookupInHashTable(hashTable hash_table, Element key) {
    if (!hash_table || !key) { //validation checks
        return NULL;
    }

    int index = hash_table->keyToNum(key) % hash_table->tableSize;
    if (index < 0) { //in case the func return negative index
        return NULL;
    }
    keyValuePair existPair = searchByKeyInList(hash_table->tableList[index], key); //check index in hash table
    if (!existPair) {
        return NULL;
    }
    return (Element)getValue(existPair); // return the value of that key or NULL if un exist
}

//remove from hash table func
status removeFromHashTable(hashTable hash_table, Element key) {
    if (!hash_table || !key) { // validation checks
        return failure;
    }
    int index = hash_table->keyToNum(key) % hash_table->tableSize;

    return deleteNode(hash_table->tableList[index], key); // delete node return status
}

//display func to print all elements in hash table
status displayHashElements(hashTable h_table) {
    if (!h_table) {
        return failure;
    }
    for (int i = 0; i < h_table->tableSize; i ++) {
        displayList(h_table->tableList[i]);
    }
    return success;
}
