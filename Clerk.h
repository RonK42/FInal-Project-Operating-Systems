#ifndef CLERK_H
#define CLERK_H


typedef enum {
    JUNIOR,
    SENIOR,
    MANAGER,
    DIRECTOR
} PositionLevel;


typedef struct {
    int clerkId;
    char* name;
    PositionLevel positionLevel;
} Clerk;

Clerk* createClerk();
void promoteClerk(void* element);
void demoteClerk(Clerk* clerk);
int compareClerkByID(const void* c1, const void* c2);
int compareClerkByName(const void* c1, const void* c2);
int compareClerkByPosition(const void* c1, const void* c2);
void printClerk(void* element);
void freeClerk(void* element);

#endif