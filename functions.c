#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include "functions.h"


void getValidClerkId(int* clerkId) {
	int validInput = 0;
	while (!validInput) {
		printf("Enter Clerk ID: ");
		if (scanf("%d", clerkId) == 1) {
			validInput = 1;
		}
		else {
			printf("Invalid input. Please enter a numeric value.\n");
			while (getchar() != '\n');
		}
	}
}

int isNumeric(const char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (!isdigit(str[i])) {
			return 0;
		}
	}
	return 1;
}

char* getStrExactName(const char* msg)
{
	char* str;
	char temp[MAXSIZE];
	printf("%s\t", msg);
	myGets(temp, MAXSIZE);

	str = getDynStr(temp);
	return str;
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;

	strcpy(theStr, str);
	return theStr;
}

char* myGets(char* buffer, int size)
{
	char* ok;
	if (buffer != NULL && size > 0)
	{
		do { //skip only '\n' strings
			ok = fgets(buffer, size, stdin);
		} while (ok && ((strlen(buffer) <= 1) && (isspace(buffer[0]))));
		if (ok)
		{
			char* back = buffer + strlen(buffer);
			//trim end spaces
			while ((buffer < back) && (isspace(*--back)));
			*(back + 1) = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

void generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element))
{
	for (int i = 0; i < size; i++)
		func((char*)(arr)+i * typeSize);

}
