#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "List.h"

List* createList(LNode* node)
{
	List* list = (List*)malloc(sizeof(List));
	if (list == NULL) {
		printf("Memory allocation failed\n");
		return NULL;
	}
	list->head = node;
	list->tail = node;
	list->size = (node != NULL) ? 1 : 0;
	return list;
}

LNode* createNode(void* data, size_t typeSize) {
	LNode* node = (LNode*)malloc(sizeof(LNode));
	if (node == NULL) {
		printf("Memory allocation failed\n");
		return NULL;
	}
	node->data = malloc(typeSize);
	if (node->data == NULL) {
		printf("Memory allocation failed\n");
		free(node);
		return NULL;
	}
	memcpy(node->data, data, typeSize);
	node->next = NULL;
	return node;
}

void addNodeToList(List* list, void* data, size_t typeSize) {
	LNode* node = createNode(data, typeSize);
	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	}
	else {
		list->tail->next = node;
		list->tail = node;
	}
	list->size++;
}

void removeNodeFromList(List* list, LNode* node) {
	if (list->head == node) {
		list->head = node->next;
		if (list->head == NULL) {
			list->tail = NULL;
		}
	}
	else {
		LNode* current = list->head;
		while (current != NULL && current->next != node) {
			current = current->next;
		}
		if (current != NULL) {
			current->next = node->next;
			if (current->next == NULL) {
				list->tail = current;
			}
		}
	}
	free(node->data);
	free(node);
	list->size--;
}

void deleteList(List* list) {
	LNode* current = list->head;
	while (current != NULL) {
		LNode* next = current->next;
		if (current->data != NULL) {
			free(current->data);
			current->data = NULL; // Prevent double-free
		}
		free(current);
		current = next;
	}
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}


void deleteNode(LNode* node) {
	free(node->data);
	free(node);
}

void* getNodeData(LNode* node) {
	return node->data;
}

LNode* findNode(List* list, void* data, int (*compare)(void*, void*)) {
	LNode* current = list->head;
	while (current != NULL) {
		if (compare(current->data, data) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}
