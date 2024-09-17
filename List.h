#ifndef LIST_H
#define LIST_H
#include <stddef.h> 


typedef struct LNode {
    void* data;
    struct LNode* next;
} LNode;

typedef struct {
    LNode* head;
    LNode* tail;
    int size;
} List;


List* createList(LNode* node);
LNode* createNode(void* data, size_t typeSize);
void addNodeToList(List* list, void* data, size_t typeSize);
void removeNodeFromList(List* list, LNode* node);
void deleteList(List* list);
void deleteNode(LNode* node);
void* getNodeData(LNode* node);
LNode* findNode(List* list, void* data, int (*compare)(void*, void*));

#endif // LIST_H
