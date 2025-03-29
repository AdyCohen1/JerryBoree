#include <stdio.h>
#include <stdlib.h>
#include "KeyValuePair.h"

//defined KVP struct
typedef struct KeyValuePair_s {
    Element key;
    Element value;
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalFunc;
} KeyValuePair_s ;

// create KVP - gets all function according to user uses:
keyValuePair createKeyValuePair(
    CopyFunction copyKey,
    FreeFunction freeKey,
    PrintFunction printKey,
    CopyFunction copyValue,
    FreeFunction freeValue,
    PrintFunction printValue,
    EqualFunction equalFunc,
    Element key,
    Element value){

    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue || !printValue) { // validation checks
        return NULL;
    }
    keyValuePair newKeyValuePair = (keyValuePair)malloc(sizeof(KeyValuePair_s)); // allocate place in memory for KVP structure
    if (!newKeyValuePair) {
        return NULL;
    }

    newKeyValuePair->key = copyKey(key); // set key as copy!
    if (!newKeyValuePair->key) {
        free(newKeyValuePair);
        return NULL;
    }

    newKeyValuePair->value = value; //set value as copy
    if (!newKeyValuePair->value) {
        freeKey(newKeyValuePair->key);
        free(newKeyValuePair);
        return NULL;
    }
    // set all user functions for this ADT
    newKeyValuePair->copyKey = copyKey;
    newKeyValuePair->freeKey = freeKey;
    newKeyValuePair->printKey = printKey;
    newKeyValuePair->copyValue = copyValue;
    newKeyValuePair->freeValue = freeValue;
    newKeyValuePair->printValue = printValue;
    newKeyValuePair->equalFunc = equalFunc;
    return newKeyValuePair; //return the new structure
}

//destroy func
status destroyKeyValuePair(Element pair){
    if (!pair) {
        return failure;
    }
    keyValuePair pairDest = (keyValuePair)pair; // by getting Element - casting to Pair
    if (pairDest->key) {
        if (pairDest->freeKey) {
            pairDest->freeKey(pairDest->key); // free key and set to NULL for safety
        }
        pairDest->key = NULL;
    }

    if (pairDest->value) {
        if (pairDest->freeValue) {
            pairDest->freeValue(pairDest->value);  // free value and set to NULL for safety
        }
        pairDest->value = NULL;
    }
    free(pairDest); //free the structure
    return success;
}

status displayValue(keyValuePair pair) {
    if (!pair) {
        return failure;
    }
    pair->printValue(pair->value); //print value according to print function of user
    return success;
}

status displayKey(keyValuePair pair) {
    if (!pair) {
        return failure;
    }
    pair->printKey(pair->key); // print key according to user's print func
    return success;
}

Element getValue(keyValuePair pair) {
    if (!pair) {
        return NULL;
    }
    return pair->value; // return pointer to value's data
}

Element getKey(keyValuePair pair) {
    if (!pair) {
        return NULL;
    }
    return pair->key; // return pointer to key's data
}

bool isEqualKey(Element pair, Element key) {
    if (!pair || !key) {
        return false;
    }
    keyValuePair pairKey = (keyValuePair)pair;
    bool flag = pairKey->equalFunc(pairKey->key, key); //checks for equal keys according to equal function the user add
    if (flag == true) {
        return true;
    }
    return false;
}
