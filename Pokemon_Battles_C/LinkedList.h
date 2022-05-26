#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include "Defs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//////////////////// generic ADT of Linked List 
/* this generic ADT includs 2 structs: Linked List and Node */
typedef struct LinkedList_s* LinkedList;
typedef struct Node_s* Node;

////////////////////////////////////// Functions Declaration

LinkedList createLinkedList(copyFunction, freeFunction, equalFunction, printFunction);
Node createNode(LinkedList, element);
status destroyList(LinkedList);
status appendNode(LinkedList, element);
status deleteNode(LinkedList, element);
status displayList(LinkedList);
element searchByKeyInList(LinkedList, element);


#endif // _LINKEDLIST_H_