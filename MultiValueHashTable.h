#ifndef MULTIVALUEHASHTABLE_H
#define MULTIVALUEHASHTABLE_H
#include "Defs.h"
#include "LinkedList.h"

/**
* * This ADT defines a hash table where each key have a linked list of values.
 * It provides functions to create, destroy, add, remove, and look up key-value pairs.
 */
typedef struct MultiValueHashTable_s* multiValueHashTable;

/**
 * create multi hash Func gets:
 * @param copyKey
 * @param freeKey
 * @param printKey
 * @param copyValue
 * @param freeValue
 * @param printValue
 * @param equalKey
 * @param equalValueFULLOrPart
 * @param transformIntoNumber
 * @param hashNumber
 * @return new Multi Hash structure
 */
multiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey, EqualFunction equalValueFULLOrPart, TransformIntoNumberFunction transformIntoNumber, int hashNumber);

/**
 * destroy Func - gets pointer to multi hash structure
 * @return status
 */
status destroyMultiValueHashTable(multiValueHashTable);

/**
 * add to multi hash func - gets:
 * @param key
 * @param value
 * @return status
 */
status addToMultiValueHashTable(multiValueHashTable, Element key, Element value);

/**
 * look up in multi hash func - gets:
 * @param key
 * @return values linked list in case of the key is in multi hash and NULL otherwise
 */
LinkedList lookUpInMultiValueHashTable(multiValueHashTable, Element key);

/**
 * remove from multi hash func - gets:
 * @param key
 * @param value
 * @return status
 */
status removeFromMultiValueHashTable(multiValueHashTable, Element key, Element value);

/**
 * display multi hash elements by key - gets:
 * @param key
 * @return status (also print the values list in case of success)
 */
status displayMultiValueHashElementsByKey(multiValueHashTable, Element key);

#endif //MULTIVALUEHASHTABLE_H