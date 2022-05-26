#ifndef _MAXHEAP_H_
#define _MAXHEAP_H_

#include "Defs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//////////////////// generic ADT of Max Heap

typedef struct MaxHeap_s* MaxHeap;


////////////////////////////////////// Functions Declaration

MaxHeap createHeap(char* name, int maxSize, copyFunction copy, freeFunction free, equalFunction equals, printFunction print);
status destroyHeap(MaxHeap);
status insertToHeap(MaxHeap heap, element elem);
status printHeap(MaxHeap);
element PopMaxHeap(MaxHeap heap);
element TopMaxHeap(MaxHeap heap);
char* getHeapId(MaxHeap heap);
int getHeapCurrentSize(MaxHeap heap);

/* Heapify functions that save the order of our max heap */
void heapifyUp(MaxHeap heap, int index); // heapify function to save the order of the heap after insertion.
void heapify(MaxHeap heap , int index); // heapify function to save the order of the heap after extraction.

MaxHeap copyHeap(MaxHeap);
int equalHeaps(char*, MaxHeap);


#endif // _MAXHEAP_H_