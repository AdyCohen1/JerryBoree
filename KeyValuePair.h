#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H
#include "Defs.h"

/**
 * KeyValuePair.h:
 * This generic ADT support storing data of key and value inside one structure
 */
typedef struct KeyValuePair_s* keyValuePair;

/**
 * create function - gets:
 * @param copyKey 
 * @param freeKey 
 * @param printKey 
 * @param copyValue 
 * @param freeValue 
 * @param printValue 
 * @param equalFunc 
 * @param key 
 * @param value 
 * @return pointer to KeyValuePair object
 */
keyValuePair createKeyValuePair(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue, EqualFunction equalFunc,Element key, Element value);

/**
 * destroy func
 * @param pair 
 * @return status (success or failure)
 */
status destroyKeyValuePair(Element pair);

/**
 * display func
 * @param pair 
 * @return status and prints the value's data stored in a pair
 */
status displayValue(keyValuePair pair);

/**
 * display func
 * @param pair 
 * @return status and prints the key's data stored in a pair
 */
status displayKey(keyValuePair pair);

/**
 * get value func
 * @param pair 
 * @return pointer to value's data 
 */
Element getValue(keyValuePair pair);

/**
 * get key func
 * @param pair 
 * @return pointer to key's data
 */
Element getKey(keyValuePair pair);

/**
 * equal key func
 * @param pair 
 * @param key 
 * @return bool (true or false) in case both have the same key or not
 */
bool isEqualKey(Element pair, Element key);


#endif //KEYVALUEPAIR_H
