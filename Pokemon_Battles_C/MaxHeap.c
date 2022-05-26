#include <stdio.h>
#include <stdlib.h>

#include "MaxHeap.h"
#include "Defs.h"

/////////////////////////////////////////////////// Function declaration

element getParent(MaxHeap heap, int index);                    // function to get the parent of element in the heap.
element getLeftChild(MaxHeap heap, int index);                 // function to get the left child of element in the heap.
element getRightChild(MaxHeap heap, int index);                // function to get the right child of element in the heap.
void swapElements(MaxHeap, int, int); // function to swap between two elements.

/////////////////////////////////////////////////// Structs definision

struct MaxHeap_s
{
    element *array;    // The max heap is implemented as an array of elements.
    int numOfElements; // The current number of elements in the heap.
    char *name;
    int maxSize;
    copyFunction copyElement;
    freeFunction freeElement;
    equalFunction equalsElements;
    printFunction printElement;
};

MaxHeap createHeap(char *name, int maxSize, copyFunction copy, freeFunction free, equalFunction equals, printFunction print)
{
    if (copy == NULL || free == NULL || equals == NULL || print == NULL)
        return NULL;
    MaxHeap newHeap = (MaxHeap)malloc(sizeof(struct MaxHeap_s));
    if (newHeap == NULL){
        printf("No memory available.\n");
        return NULL;
    }
        
    newHeap->name = (char *)malloc(strlen(name) + 1);
    if (newHeap->name == NULL){
        printf("No memory available.\n");
        return NULL;
    }
    newHeap->array = (element *)malloc(sizeof(element) * maxSize);
    if (newHeap->array == NULL){
        printf("No memory available.\n");
        return NULL;
    }

    newHeap->numOfElements = 0;
    newHeap->copyElement = copy;
    newHeap->equalsElements = equals;
    newHeap->freeElement = free;
    newHeap->printElement = print;
    newHeap->maxSize = maxSize;
    strcpy(newHeap->name, name);
    return newHeap;
}

status destroyHeap(MaxHeap heap)
{
    if (heap == NULL)
        return failure;

    int i;
    for (i = 0; i < heap->numOfElements; i++)
    {
        if (heap->array[i] != NULL)
        {
            heap->freeElement(heap->array[i]);
            heap->array[i] = NULL;
        }
    }
    if (heap->array != NULL) {
        free(heap->array);
        heap->array = NULL;
    }
    free(heap->name);
    heap->name = NULL;
    free(heap);

    return success;
}

status insertToHeap(MaxHeap heap, element elem)
{
    if (heap == NULL || heap->array == NULL)
        return failure;
    if (heap->numOfElements >= heap->maxSize)
        return failure;

    if (heap->numOfElements == 0){
        heap->array[0] = elem;
        heap->numOfElements++;
        return success;
    }

    heap->array[heap->numOfElements] = elem;
    heap->numOfElements++;
    int i;
    int elements = heap->numOfElements;
    for (i = ((elements / 2) - 1); i >= 0; i--)
        heapify(heap, i);

    return success;
}

void heapifyUp(MaxHeap heap, int index)
{
    if (index < 1)
        return;

    element parent = getParent(heap, index);
    if (heap->equalsElements(heap->array[index], parent) == bigger) // check if element is bigger then his parent.
    {
        swapElements(heap, index, ((index - 1) / 2));
        heapifyUp(heap, (index - 1) / 2);
    }

    return;
}

status printHeap(MaxHeap heap)
{
    if (heap == NULL || heap->array == NULL)
        return failure;
    int tempNumOfElements = heap->numOfElements;
    element tempArray[tempNumOfElements];

    printf("%s:\n", heap->name);

    if (tempNumOfElements == 0){
        puts("No elements.\n");
        return success;
    }

    int i;
    for (i=0; i<tempNumOfElements; i++)
        tempArray[i] = PopMaxHeap(heap);

    for (i = 0; i < tempNumOfElements; i++)
    {
        printf("%d. ", (i + 1));
        heap->printElement(tempArray[i]);
        insertToHeap(heap, tempArray[i]);
    }

    return success;
}

void heapify(MaxHeap heap, int index)
{
    if (index >= heap->numOfElements)
        return;

    element largest = heap->array[index];
    element leftChild = getLeftChild(heap, index);
    element rightChild = getRightChild(heap, index);
    int leftChildIndex = index * 2 + 1;  // the left child index
    int rightChildIndex = index * 2 + 2; // the right child index
    int largestIndex = index;

    if(leftChild != NULL){
        if (leftChildIndex < heap->numOfElements && heap->equalsElements(leftChild, largest) == 1)
        {
            largest = leftChild;
            largestIndex = leftChildIndex;
        }
    }

    if (rightChild != NULL){
        if (rightChildIndex < heap->numOfElements && heap->equalsElements(rightChild, largest) == 1)
        {
        largest = rightChild;
        largestIndex = rightChildIndex;
        }
    }

    if (largestIndex != index)
    {
        swapElements(heap, index, largestIndex);
        heapify(heap, largestIndex);
    }
}

element PopMaxHeap(MaxHeap heap)
{
    if (heap == NULL || heap->array == NULL || heap->numOfElements == 0)
        return NULL;

    element max = heap->array[0];
    swapElements(heap, 0, heap->numOfElements-1);
    heap->numOfElements--;
    heap->array[heap->numOfElements] = NULL;
    int i;
    int elements = heap->numOfElements;
    for (i = ((elements / 2) - 1); i >= 0; i--)
        heapify(heap, i);

    return max;
}

element TopMaxHeap(MaxHeap heap)
{
    if (heap == NULL || heap->array == NULL || heap->numOfElements == 0)
        return NULL;

    return heap->array[0];
}

char *getHeapId(MaxHeap heap)
{
    if (heap == NULL)
        return NULL;

    return heap->name;
}

int getHeapCurrentSize(MaxHeap heap)
{
    if (heap == NULL)
        return 0;

    return heap->numOfElements;
}

/////////////////////////////////////////////////// Helpers function

element getParent(MaxHeap heap, int index)
{
    return heap->array[(index - 1) / 2];
}

element getLeftChild(MaxHeap heap, int index)
{
    return heap->array[index * 2 + 1];
}

element getRightChild(MaxHeap heap, int index)
{
    return heap->array[index * 2 + 2];
}

void swapElements(MaxHeap heap, int index1, int index2)
{
    element temp = heap->array[index1];
    heap->array[index1] = heap->array[index2];
    heap->array[index2] = temp;
}

MaxHeap copyHeap(MaxHeap heapToCopy)
{
    if (heapToCopy == NULL)
        return NULL;

    MaxHeap copiedHeap = createHeap(heapToCopy->name, heapToCopy->maxSize, heapToCopy->copyElement, heapToCopy->freeElement, heapToCopy->equalsElements, heapToCopy->printElement);
    if (copiedHeap == NULL)
        return NULL;

    int i;
    int elements = heapToCopy->numOfElements;
    for (i = 0; i < elements; i++)
        insertToHeap(copiedHeap, heapToCopy->array[i]);

    return copiedHeap;
}

int equalHeaps(char* name, MaxHeap heap2)
{
    return strcmp(name, heap2->name);
}
