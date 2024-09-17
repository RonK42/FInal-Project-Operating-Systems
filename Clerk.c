#define _CRT_SECURE_NO_WARNINGS
#include "clerk.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "functions.h"

Clerk* createClerk() {
    Clerk* clerk = (Clerk*)malloc(sizeof(Clerk));
    if (!clerk)
    {
        printf("ERROR");
        return NULL;
    }
    getValidClerkId(&clerk->clerkId);
    clerk->name = getStrExactName("Enter clerk name: ");
    int position;
    printf("Enter Clerk Position (0 for JUNIOR, 1 for SENIOR, 2 for MANAGER, 3 for DIRECTOR): ");
    scanf("%d", &position);

    if (position < JUNIOR || position > DIRECTOR) {
        printf("ERROR: Invalid position level.\n");
        free(clerk->name);
        free(clerk);
        return NULL;
    }

    clerk->positionLevel = (PositionLevel)position;

    return clerk;
}

void promoteClerk(void* element) {
    Clerk* clerk = *((Clerk**)element);
    if (clerk->positionLevel < DIRECTOR) {
        clerk->positionLevel += 1;
    }
    else {
        printf("Clerk ID: %d is already at the maximum position level.\n", clerk->clerkId);
    }
}

void demoteClerk(Clerk* clerk) {
    if (clerk->positionLevel > JUNIOR) {
        clerk->positionLevel--;
        printf("Clerk demoted to level %d.\n", clerk->positionLevel);
    }
    else {
        printf("Clerk is already at the lowest level.\n");
    }
}

int compareClerkByID(const void* c1, const void* c2) {
    const Clerk* clerk1 = *(const Clerk**)c1;
    const Clerk* clerk2 = *(const Clerk**)c2;
    return clerk1->clerkId - clerk2->clerkId;
}

int compareClerkByName(const void* c1, const void* c2) {
    const Clerk* clerk1 = *(const Clerk**)c1;
    const Clerk* clerk2 = *(const Clerk**)c2;
    return strcmp(clerk1->name, clerk2->name);
}

int compareClerkByPosition(const void* c1, const void* c2) {
    const Clerk* clerk1 = *(const Clerk**)c1;
    const Clerk* clerk2 = *(const Clerk**)c2;
    return clerk1->positionLevel - clerk2->positionLevel;
}
void printClerk(void* element) {
    Clerk* clerk = *((Clerk**)element); 
    printf("Clerk ID: %d, Name: %s, Position: %d\n", clerk->clerkId, clerk->name, clerk->positionLevel);
}

void freeClerk(void* element) {
    Clerk* clerk = *((Clerk**)element); 
    free(clerk);
}