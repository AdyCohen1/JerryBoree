#include <stdio.h>
#include <stdlib.h>
#include "MultiValueHashTable.h"
#include "HashTable.h"

// definition of the Multi Value Hash Table structure
typedef struct MultiValueHashTable_s {
    hashTable multiTable;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printKey;
    PrintFunction printValue;
    EqualFunction equalValue;
} MultiValueHashTable_s ;


//Helper Functions to help Copy , Free and Print LinkedList element:

Element copyLinkedList(Element element) {
    if (!element) {
        return NULL;
    }
    LinkedList elemToList = (LinkedList)element;
    return elemToList;
}

status freeLinkedList(Element element) {
    if (!element) {
        return failure;
    }
    LinkedList elemToList = (LinkedList)element;
    return destroyList(elemToList);
}

status printLinkedList(Element element) {
    if (!element) {
        return failure;
    }
    LinkedList elemToList = (LinkedList)element;
    return displayList(elemToList);
}

//create Multi Hash
multiValueHashTable createMultiValueHashTable(
    CopyFunction copyKey,
    FreeFunction freeKey,
    PrintFunction printKey,
    CopyFunction copyValue,
    FreeFunction freeValue,
    PrintFunction printValue,
    EqualFunction equalKey,
    EqualFunction equalValue,
    TransformIntoNumberFunction transformIntoNumber,
    int hashNumber) {

    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue || !printValue || !equalKey || !equalValue || !transformIntoNumber || hashNumber <= 0) {
        return NULL; //validation checks
    }
    multiValueHashTable newMultiHash = (multiValueHashTable)malloc(sizeof(MultiValueHashTable_s));
    if (!newMultiHash) { //allocate space for Hash table
        return NULL;
    }

    newMultiHash->multiTable = createHashTable(
        copyKey,
        freeKey,
        printKey,
        copyLinkedList,
        freeLinkedList,
        printLinkedList,
        equalKey,
        transformIntoNumber,
        hashNumber);
    if (!newMultiHash->multiTable) { // set the hash table structure that pointed from multi hash
        free(newMultiHash);
        return NULL;
    }

    // set all fields of multi hash
    newMultiHash->printKey = printKey;
    newMultiHash->copyValue = copyValue;
    newMultiHash->freeValue = freeValue;
    newMultiHash->printValue = printValue;
    newMultiHash->equalValue = equalValue;
    return newMultiHash;
}
// destroy func frees all memory associated with the hash table and linked lists.
status destroyMultiValueHashTable(multiValueHashTable multiHashTable) {
    if (!multiHashTable) {
        return failure;
    }

    bool flag = destroyHashTable(multiHashTable->multiTable);
    if (flag == failure) {
        return failure;
    }
    free(multiHashTable);
    return success;
}

//add to multi hash
status addToMultiValueHashTable(multiValueHashTable multiHashTable, Element key, Element value) {
    if (!multiHashTable || !key || !value) {
        return failure;
    }

    LinkedList valuesListExist = (LinkedList) lookupInHashTable(multiHashTable->multiTable, key);
    if (valuesListExist == NULL) { // check first if the key is already in multi hash - if not create a new one, else add another value to the key
        LinkedList newValuesList = createLinkedList(
            multiHashTable->copyValue,
            multiHashTable->freeValue,
            multiHashTable->printValue,
            multiHashTable->equalValue);

        if (!newValuesList) { // case of memory problem - allocate fails
            destroyMultiValueHashTable(multiHashTable);
            return failure;
        }

        if (appendNode(newValuesList, value) == failure) { //add the new val to the values list
            destroyMultiValueHashTable(multiHashTable);
            return failure;
        }

        if (addToHashTable(multiHashTable->multiTable, key, newValuesList) == failure) {
            destroyList(newValuesList);
            return failure;
        }

    }
    else {
        if (appendNode(valuesListExist, value) == failure) { // in case key is already in multi hash - add another node to the end of the values list
            return failure;
        }
    }
    return success;
}

// Function to look for values by key in the multi-value hash table
LinkedList lookUpInMultiValueHashTable(multiValueHashTable multiHashTable, Element key) {
    if (!multiHashTable || !key) {
        return NULL;
    }
    Element valuesList = lookupInHashTable(multiHashTable->multiTable, key); //return the value of key - or NULL in case of the key is no in hash table
    if (!valuesList) {
        return NULL;
    }
    return valuesList; // if key is in multi hash the func will return the values list otherwise - NULL
}

// Function to remove a specific value from the values list of a key
status removeFromMultiValueHashTable(multiValueHashTable multiHashTable, Element key, Element value) {
    if (!multiHashTable || !key || !value) {
        return failure;
    }
    LinkedList valuesList = (LinkedList)lookUpInMultiValueHashTable(multiHashTable, key);
    if (!valuesList) { // in case the key is not exist
        return failure;
    }
    Element valToDel = searchByKeyInList(valuesList, value);
    if (!valToDel) {
        return failure;
    }
    if (deleteNode(valuesList, valToDel) == failure) { // delete the node (value) from values list
        return failure;
    }
    if (getLengthList(valuesList) == 0) { // if values list are empty - delete the key from the multi hash
        if (removeFromHashTable(multiHashTable->multiTable, key) == failure) {
            return failure;
        }
    }
    return success;
}
// function that print all values of input key
status displayMultiValueHashElementsByKey(multiValueHashTable multiHashTable, Element key) {
    if (!multiHashTable || !key) {
        return failure;
    }
    LinkedList valuesList = (LinkedList)lookUpInMultiValueHashTable(multiHashTable, key);
    if (!valuesList) {
        return failure;
    }
    multiHashTable->printKey(key);
    displayList(valuesList);
    return success;
}
