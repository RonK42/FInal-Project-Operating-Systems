#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAXSIZE 254

void getValidClerkId(int* clerkId);
int isNumeric(const char* str);
char* getStrExactName(const char* msg);
char* getDynStr(char* str);
char* myGets(char* buffer, int size);
void generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element));

#endif // FUNCTIONS_H
