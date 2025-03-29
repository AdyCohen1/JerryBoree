#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Defs.h"

/**
 * LinkedList.h:
 * This linked list supports storing generic elements.
 * It provides operations such as creation, destruction, appending, deleting, and searching.
 */
typedef struct LinkedList_s* LinkedList;

/**
 * Create Linked List - a function to create the ADT.
 * @param copyFunc
 * @param freeFunc
 * @param printFunc
 * @param equalFunc
 * @return LinkedList structure
 */
LinkedList createLinkedList(CopyFunction copyFunc, FreeFunction freeFunc, PrintFunction printFunc, EqualFunction equalFunc);

/**
 * Destroy Linked List func
 * @param linkedList_p
 * @return status (success or failure)
 */
status destroyList(LinkedList linkedList_p);

/**
 * Append Node Func
 * @param linkedList_p
 * @param element
 * @return status (success or failure)
 */
status appendNode(LinkedList linkedList_p, Element element);

/**
 * Delete Node func
 * @param linkedList_p
 * @param element
 * @return (success or failure)
 */
status deleteNode(LinkedList linkedList_p, Element element);

/**
 * Display Linked List func - prints all elements in Linked List
 * @param linkedList_p
 * @return (success or failure)
 */
status displayList(LinkedList linkedList_p);

/**
 * get Data By Index func- start by 1
 * @param linkedList_p
 * @param index
 * @return element saved in input index or NULL if not exist
 */
void* getDataByIndex(LinkedList linkedList_p, int index);

/**
 * get Length List func
 * @param linkedList_p
 * @return number of elements in List
 */
int getLengthList(LinkedList linkedList_p);

/**
 * search By Key In List func
 * @param linkedList_p
 * @param element
 * @returnPointer to the element if found, NULL otherwise
 */
Element searchByKeyInList(LinkedList linkedList_p, Element element);

#endif //LINKEDLIST_H