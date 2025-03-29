#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "KeyValuePair.h"

//Node struct - contain key (data) and pointer to next Node initialize by NULL.
typedef struct Node_s {
    Element key;
    struct Node_s* next;
} Node ;

//LinkedList structure - contain pointer to Node and length of List
//LinkedList is a generic ADT - contain copy, free, print and equal funcs to give interface for programmers.
struct LinkedList_s {
    Node *head; //the first Node will be the head
    int length; //Lengt of linkedList (number of nodes)
    CopyFunction copyFunc;
    FreeFunction freeFunc;
    PrintFunction printFunc;
    EqualFunction equalFunc;
} LinkedList_s ;

//helper func to build a node Linked list is built by nodes
Node* createNode(Element key) {
    if (!key) {
        return NULL;
    }
    Node* node = (Node*)malloc(sizeof(Node)); //create place in heap for NODE Struct
    if (!node) {
        return NULL;
    }
    node->key = key; //initialize key of node and the next element to NULL
    node->next = NULL;
    return node; //return the node i created
}

//create linked list function
LinkedList createLinkedList(CopyFunction copyFunc, FreeFunction freeFunc, PrintFunction printFunc, EqualFunction equalFunc) {
    if (!copyFunc || !freeFunc || !printFunc || !equalFunc) { //validation checking
        return NULL;
    }

    LinkedList new_list = (LinkedList)malloc(sizeof(LinkedList_s)); // allocate place in heap for linkedLIst stracture
    if (!new_list) {
        return NULL;
    }

    //initialize all fields of that structure
    new_list->head = NULL; // Set the head to NULL
    new_list->length = 0; // Set the length to 0
    new_list->copyFunc = copyFunc; // Set the copy func
    new_list->freeFunc = freeFunc; // Set the free func
    new_list->printFunc = printFunc; // Set the print func
    new_list->equalFunc = equalFunc; // Set the equal func

    return new_list; //return new linked LISt
}

// Function to destroy a LinkedList and free all its resources.
status destroyList(LinkedList linkedList_p) {
    if (!linkedList_p) {
        return failure; // return failure if the LinkedList is not valid
    }
    Node* currNode = linkedList_p->head; // start from the head node
    while (currNode) {
        Node* nodeToDel = currNode; // store the current node to delete
        currNode = currNode->next; // move to the next node in list

        if (linkedList_p->freeFunc) {
            linkedList_p->freeFunc(nodeToDel->key); //free data in node
        }
        free(nodeToDel); // free the node structure
    }

    linkedList_p->head = NULL;
    free(linkedList_p); // free linked list structure after freeing all nodes
    linkedList_p = NULL;
    return success;
}

// Function to append a new Node with the given element to the LinkedList.
status appendNode(LinkedList linkedList_p, Element element) {
    if (!linkedList_p || !element) {
        return failure;
    }
    Element newKey = linkedList_p->copyFunc(element); // create a copy of the element using the user copy function
    if (!newKey) {
        return failure;
    }
    Node* newNode = createNode(newKey);
    if (!newNode) {
        return failure;
    }

    if (linkedList_p->length == 0) {
        linkedList_p->head = newNode; //in case of an empty list - set the first node to be the new node (the head)

    }else { // list in no empty - find the last node to add in the end
        Node* currNode = linkedList_p->head;
        while (currNode->next) {
            currNode = currNode->next;
        }
        currNode->next = newNode;
    }
    linkedList_p->length ++; // increase length by 1
    return success;
}

// Function to delete a Node with the specified element from the LinkedList.
status deleteNode(LinkedList linkedList_p, Element element) {
    if (!linkedList_p || linkedList_p->length == 0 || !element) { //checking if linked list is null or if linked list is an empty list
        return failure;
    }

    // handle the case where the list has only one Node
    Node* currNode = linkedList_p->head;
    if (linkedList_p->length == 1) {
        if (linkedList_p->equalFunc(currNode->key, element)) {
            linkedList_p->freeFunc(currNode->key);
            free(currNode);
            currNode = NULL;
            linkedList_p->head = NULL;
            linkedList_p->length --;
            return success;
        }
        return failure;
    } //in case list length is greater than 1
    if (linkedList_p->equalFunc(linkedList_p->head->key, element)) { //in case the first Node is equal the key (delete the head)
        Node* temp = linkedList_p->head;
        linkedList_p->head = linkedList_p->head->next;
        linkedList_p->freeFunc(temp->key);
        free(temp);
        temp = NULL;
        linkedList_p->length--;
        return success;
    } //in case node to delete is not the head:
    currNode = linkedList_p->head->next;
    Node* prevNode = linkedList_p->head;

    for (int i = 0; i < linkedList_p->length - 1; i ++) {
        if (linkedList_p->equalFunc(currNode->key, element)) {
            prevNode->next = currNode->next;
            linkedList_p->freeFunc(currNode->key);
            free(currNode);
            currNode = NULL;
            linkedList_p->length--; //decrease length by 1 after removing node
            return success;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    return failure;
}

// Function to display all elements in the LinkedList.
status displayList(LinkedList linkedList_p) {
    if (!linkedList_p) {
        return failure;
    }
    Node* currNode = linkedList_p->head; // start from the head Node

    while (currNode) {
        linkedList_p->printFunc(currNode->key); //print the curr node according to print func the user add
        currNode = currNode->next; // move to the next node
    }
    return success;
}

// Function to retrieve the data of a Node at a specified index in the LinkedList.
void* getDataByIndex(LinkedList linkedList_p, int index) {
    if (!linkedList_p || index <= 0 || linkedList_p->length < index){ //check for validation
        return NULL;
    }
    Node* currNode = linkedList_p->head;
    for (int i = 1; i < index; i ++) {
        currNode = currNode->next;
    }
    return currNode->key; // return data of the node in the input data
}

int getLengthList(LinkedList linkedList_p) {
    if (!linkedList_p) {
        return -1; //expect getting a positive number - a negative number will point into a problem
    }
    return linkedList_p->length;
}

// Function to search for an element in the given LinkedList
Element searchByKeyInList(LinkedList linkedList_p, Element element) {
    if (!linkedList_p || !element) {
        return NULL;
    }
    Node* currNode = linkedList_p->head;
    for (int i = 0; i < linkedList_p->length; i++) {
        if (linkedList_p->equalFunc(currNode->key, element)) {
            return currNode->key; // returns the element if found, or NULL if not found.
        }
        currNode = currNode->next;
    }
    return NULL;
}