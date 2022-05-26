#include <stdio.h>
#include <stdlib.h>

#include "LinkedList.h"
#include "Defs.h"

/////////////////////////////////////////////////// Function declaration

Node getNode(LinkedList, element); // function to find a node in list if exist.

/////////////////////////////////////////////////// Structs definision

struct Node_s
{
    element data;
    struct Node_s *next;
};

struct LinkedList_s
{
    Node head;
    Node end;

    copyFunction copyElement;
    freeFunction freeElement;
    equalFunction equalsElements;
    printFunction printElement;
};

/////////////////////////////////////////////////// Create functions

Node createNode(LinkedList list, element elem)
{
    Node newNode = (Node)malloc(sizeof(struct Node_s));
    if (newNode == NULL)
        return NULL;

    newNode->data = elem;
    newNode->next = NULL;
    return newNode;
}

LinkedList createLinkedList(copyFunction copy, freeFunction free, equalFunction equals, printFunction print)
{
    if (copy == NULL || free == NULL || equals == NULL || print == NULL)
        return NULL;
    LinkedList newList = (LinkedList)malloc(sizeof(struct LinkedList_s));
    if (newList == NULL)
        return NULL;

    newList->head = NULL;
    newList->end = NULL;

    newList->copyElement = copy;
    newList->freeElement = free;
    newList->equalsElements = equals;
    newList->printElement = print;

    return newList;
}

/////////////////////////////////////////////////// Functions

status appendNode(LinkedList list, element elem)
{
    if (list == NULL || elem == NULL)
        return failure;

    Node newNode = createNode(list, elem);
    if (newNode == NULL)
        return failure;

    if (list->head == NULL)
    {
        list->head = newNode;
        list->head->next = list->end;
        return success;
    }
    if (list->end == NULL)
    {
        list->end = newNode;
        list->head->next = list->end;
        list->end->next = NULL;
        return success;
    }
    list->end->next = newNode;
    list->end = list->end->next;
    newNode->next = NULL;
    return success;
}

status destroyList(LinkedList list)
{
    if (list == NULL)
        return failure;
    Node temp = list->head;
    while (temp != NULL)
    {
        Node toDestroy = temp;
        temp = temp->next;
        deleteNode(list, toDestroy->data);
    }
    list->head = NULL;
    list->end = NULL;
    free(list);
    return success;
}

status deleteNode(LinkedList list, element elemToDelete)
{
    if (list == NULL || elemToDelete == NULL)
        return failure;

    Node nodeToDelete = getNode(list, elemToDelete);
    if (nodeToDelete == NULL)
        return not_exist;

    Node temp = list->head;
    while (temp->next != NULL)
    {
        if (nodeToDelete == temp->next)
        {
            if (nodeToDelete == list->end)
                temp = list->end;

            temp->next = nodeToDelete->next;
            list->freeElement(nodeToDelete->data);
            free(nodeToDelete);
            return success;
        }
        temp = temp->next;
    }
    list->head = list->head->next;
    list->freeElement(nodeToDelete->data);
    free(nodeToDelete);

    return success;
}

element searchByKeyInList(LinkedList list, element elem)
{
    if (list == NULL || elem == NULL)
        return NULL;
    Node temp = list->head;
    while (temp != NULL)
    {
        if (list->equalsElements(elem, temp->data) == 0)
            return temp->data;

        temp = temp->next;
    }
    return NULL;
}

status displayList(LinkedList list)
{
    if (list == NULL)
        return failure;
    Node temp = list->head;
    while (temp != NULL)
    {
        list->printElement(temp->data);
        temp = temp->next;
    }
    return success;
}

Node getNode(LinkedList list, element elem)
{
    if (list == NULL || elem == NULL)
        return NULL;
    Node temp = list->head;
    while (temp != NULL)
    {
        if (temp->data = elem)
            return temp;

        temp = temp->next;
    }
    return NULL;
}