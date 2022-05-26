#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "BattleByCategory.h"
#include "LinkedList.h"
#include "MaxHeap.h"
#include "Defs.h"

struct battle_s {
    int capacity;
    int numberOfCategories;
    char *categories;

    equalFunction equalElement;
    copyFunction copyElement;
    freeFunction freeElement;
    getCategoryFunction getCategory;
    getAttackFunction getAttack;
    printFunction printElement;
    LinkedList categoriesList;
};

Battle createBattleByCategory(int capacity, int numberOfCategories, char *categories, equalFunction equalElement,
                              copyFunction copyElement, freeFunction freeElement, getCategoryFunction getCategory,
                              getAttackFunction getAttack, printFunction printElement) {
    if (categories == NULL || equalElement == NULL || copyElement == NULL || freeElement == NULL || getAttack == NULL ||
        printElement == NULL)
        return NULL;
    Battle newBattle = (Battle) malloc(sizeof(struct battle_s));
    if (newBattle == NULL)
        return NULL;
    newBattle->categories = (char *) malloc(strlen(categories) + 1);
    if (newBattle->categories == NULL)
        return NULL;

    strcpy(newBattle->categories, categories);
    newBattle->capacity = capacity;
    newBattle->numberOfCategories = numberOfCategories;
    newBattle->equalElement = equalElement;
    newBattle->copyElement = copyElement;
    newBattle->freeElement = freeElement;
    newBattle->getCategory = getCategory;
    newBattle->getAttack = getAttack;
    newBattle->printElement = printElement;

    newBattle->categoriesList = createLinkedList((copyFunction) copyHeap, (freeFunction) destroyHeap,
                                                 (equalFunction) equalHeaps, (printFunction) printHeap);

    MaxHeap heap;

    char *tempWord;
    char* tempCategories = (char*) malloc(strlen(categories) + 1);
    strcpy(tempCategories, newBattle->categories);
    tempWord = strtok(tempCategories, ",");

    int i;
    for (i = 0; i < newBattle->numberOfCategories; i++) {
        heap = createHeap(tempWord, capacity, (copyFunction) newBattle->copyElement,
                          (freeFunction) newBattle->freeElement, (equalFunction) newBattle->equalElement,
                          (printFunction) newBattle->printElement);
        appendNode(newBattle->categoriesList, (element) heap);
        tempWord = strtok(NULL, ",");
    }

    free(tempCategories);
    return newBattle;
}

status destroyBattleByCategory(Battle b) {
    if (b == NULL)
        return failure;
    destroyList(b->categoriesList);
    free(b->categories);
    free(b);
}

status insertObject(Battle b, element elem) {
    if (b == NULL || elem == NULL)
        return failure;

    MaxHeap heap = (MaxHeap) searchByKeyInList(b->categoriesList, b->getCategory(elem));

    if (heap == NULL)
        return failure;

    if (getHeapCurrentSize(heap) >= b->capacity)
        return failure;

    insertToHeap(heap, elem);

    return success;
}

void displayObjectsByCategories(Battle b) {
    if (b == NULL)
        return;
    displayList(b->categoriesList);
}

element removeMaxByCategory(Battle b, char *category) {
    if (b == NULL || category == NULL)
        return NULL;

    MaxHeap heap = (MaxHeap) searchByKeyInList(b->categoriesList, category);
    if (heap == NULL)
        return NULL;

    element max = PopMaxHeap(heap);
    return max;
}

int getNumberOfObjectsInCategory(Battle b, char *category) {
    if (b == NULL || category == NULL)
        return 0;

    MaxHeap heap = (MaxHeap) searchByKeyInList(b->categoriesList, category);
    if (heap == NULL)
        return 0;

    int numberOfObjectsInCategory = getHeapCurrentSize(heap);
    return numberOfObjectsInCategory;
}

element fight(Battle b, element elem) {
    if (b == NULL || elem == NULL || b->numberOfCategories == 0)
        return NULL;

    element strongestFighter = NULL, tempFighter = NULL, tempFighter2;

    int tempAttack1, tempAttack2, fightResult, maxAttack1 = 0, maxAttack2 = 0;
    char *tempCategory = (char *) malloc(strlen(b->categories) + 1);
    if (tempCategory == NULL){
        free(tempCategory);
        return NULL;
    }
        

    strcpy(tempCategory, b->categories);
    char *categoryName = strtok(tempCategory, ",");

    int i;
    for (i = 0; i < b->numberOfCategories; i++) {
        MaxHeap tempHeap = (MaxHeap) searchByKeyInList(b->categoriesList, categoryName);
        if (tempHeap != NULL && getHeapCurrentSize(tempHeap) > 0) {
            tempFighter = TopMaxHeap(tempHeap);
            fightResult = b->getAttack(elem, tempFighter, &tempAttack1, &tempAttack2);

            if (tempAttack2 > maxAttack2) {
                maxAttack2 = tempAttack2;
                maxAttack1 = tempAttack1;
                strongestFighter = tempFighter;
            }
        }

        categoryName = strtok(NULL, ",");
    }
    printf("You choose to fight with:\n");
    b->printElement(elem);
    if (strongestFighter == NULL){
        free(tempCategory);
        return NULL;
    }

    printf("The final battle between:\n");
    b->printElement(elem);
    printf("In this battle his attack is :%d\n\n", maxAttack1);
    printf("against ");
    b->printElement(strongestFighter);
    printf("In this battle his attack is :%d\n\n", maxAttack2);

    if (maxAttack1 == maxAttack2) {
        printf("IT IS A DRAW.\n");
        free(tempCategory);
        return strongestFighter;
    }

    if (maxAttack1 > maxAttack2) {
        printf("THE WINNER IS:\n");
        b->printElement(elem);
        free(tempCategory);
        return elem;
    }

    printf("THE WINNER IS:\n");
    b->printElement(strongestFighter);
    free(tempCategory);

    return strongestFighter;
}