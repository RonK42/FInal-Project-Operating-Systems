#ifndef ClientDB_H
#define ClientDB_H

#include "Client.h"
#include "List.h"

typedef struct {
	List* clientsList;
	int clientCount;
} ClientDB;

void updateClientDB(ClientDB* db);
ClientDB* createDBFromTextFile(const char* fileName);
int compareClientByID(void* data, void* ID);
ClientDB* createClientDB();
void addClient(ClientDB* db, client* client);
void removeClient(ClientDB* db, client* client);
client* findClient(ClientDB* db, const char* ID);
void deleteClientDB(ClientDB* db);
void writeClientData(const ClientDB* db, const char* fileName);
void readClientData(ClientDB* db, const char* fileName);
BankAccount* findAccount(ClientDB* db, const char* accountNumber);
void writeClientDataBinary(const ClientDB* db, const char* fileName);
void readClientDataBinary(ClientDB* db, const char* fileName);
void printClientDB(const ClientDB* db);
void freeClientDB(ClientDB* db);

#endif // ClientDB_H