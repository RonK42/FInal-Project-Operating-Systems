#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClientDB.h"
#include "Client.h"
#include "List.h"
#include "functions.h"


int compareClientByID(void* data, void* ID) {
    client* clientData = (client*)data;
    return strcmp(clientData->ID, (char*)ID);
}

ClientDB* createClientDB() {
    ClientDB* db = (ClientDB*)malloc(sizeof(ClientDB));
    if (db == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    db->clientsList = createList(NULL);
    db->clientCount = 0;
    return db;
}

// Add a client to the database
void addClient(ClientDB* db, client* newClient) {
    addNodeToList(db->clientsList, newClient, sizeof(client));
    db->clientCount++;
}

// Remove a client from the database
void removeClient(ClientDB* db, client* clientToRemove) {
    LNode* node = findNode(db->clientsList, clientToRemove->ID, compareClientByID);
    if (node != NULL) {
        removeNodeFromList(db->clientsList, node);
        db->clientCount--;
    }
    else {
        printf("Client with ID %s not found.\n", clientToRemove->ID);
    }
}

// Find a client by ID
client* findClient(ClientDB* db, const char* ID) {
    LNode* node = findNode(db->clientsList, (void*)ID, compareClientByID);
    if (node != NULL) {
        return (client*)getNodeData(node);
    }
    return NULL;
}

// Delete the entire Client Database
void deleteClientDB(ClientDB* db) {
    deleteList(db->clientsList);
    free(db);
}

// Write client data to a text file
void writeClientData(const ClientDB* db, const char* fileName) {
    FILE* file = fopen(fileName, "w"); // "w" mode will overwrite the file if it exists
    if (file == NULL) {
        printf("Failed to open file %s\n", fileName);
        return;
    }

    LNode* current = db->clientsList->head;
    while (current != NULL) {
        client* clientData = (client*)getNodeData(current);

        fprintf(file, "ID: %s\nName: %s\n", clientData->ID, clientData->name);

        // Write all accounts for this client
        BankAccount* currentAccount = &clientData->account;
        while (currentAccount != NULL)
        {
            fprintf(file, "Account Number: %s\nBalance: %.2lf\n",
                currentAccount->accountNumber, currentAccount->balance);

            // Move to the next account in the linked list
            currentAccount = currentAccount->nextAccount;
        }

        fprintf(file, "\n");
        current = current->next;
    }

    fclose(file);
    printf("File %s written successfully.\n", fileName);
}

void readClientData(ClientDB* db, const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Failed to open file %s\n", fileName);
        return;
    }

    client tempClient;
    while (fscanf(file, "ID: %s\nName: %s\nAccount Number: %s\nBalance: %lf\n\n",
        tempClient.ID, tempClient.name, tempClient.account.accountNumber, &tempClient.account.balance) != EOF) {
        client* newClient = (client*)malloc(sizeof(client));
        if (newClient == NULL) {
            printf("Memory allocation failed\n");
            fclose(file);
            return;
        }
        memcpy(newClient, &tempClient, sizeof(client));
        addClient(db, newClient);
    }

    fclose(file);
}

// Read client data from a binary file
BankAccount* findAccount(ClientDB* db, const char* accountNumber) {
    // Iterate over all clients in the database
    LNode* currentNode = db->clientsList->head;
    while (currentNode != NULL) {
        client* currentClient = (client*)getNodeData(currentNode);

        // Iterate through all accounts in the client
        BankAccount* currentAccount = &currentClient->account;
        while (currentAccount != NULL) {
            if (strcmp(currentAccount->accountNumber, accountNumber) == 0) {
                return currentAccount;
            }
            currentAccount = currentAccount->nextAccount;
        }

        // Move to the next client in the database
        currentNode = currentNode->next;
    }

    // If account was not found
    return NULL;
}

ClientDB* createDBFromTextFile(const char* fileName) {
    ClientDB* db = createClientDB();
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Failed to open file %s\n", fileName);
        return NULL;
    }
    char id[ID_SIZE];
    char name[ID_SIZE];
    char accountNumber[BANK_ID_ACC_SIZE];
    double balance;
    client* newClient = NULL;

    while (fscanf(file, "ID: %s\nName: %s\n", id, name) != EOF) {
        newClient = (client*)malloc(sizeof(client));
        if (newClient == NULL) {
            printf("Memory allocation failed\n");
            fclose(file);
            return NULL;
        }

        // Set up the new client data
        strcpy(newClient->ID, id);
        newClient->name = (char*)malloc(strlen(name) + 1);
        if (newClient->name == NULL) {
            printf("Memory allocation failed\n");
            free(newClient);
            fclose(file);
            return NULL;
        }
        strcpy(newClient->name, name);
        newClient->accCount = 0;

        BankAccount* lastAccount = NULL;
        while (fscanf(file, "Account Number: %s\nBalance: %lf\n", accountNumber, &balance) == 2) {
            BankAccount* newAccount = (BankAccount*)malloc(sizeof(BankAccount));
            if (newAccount == NULL) {
                printf("Memory allocation failed\n");
                fclose(file);
                return NULL;
            }
            strcpy(newAccount->accountNumber, accountNumber);
            newAccount->balance = balance;
            newAccount->nextAccount = NULL;
            newAccount->prevAccount = NULL;

            if (newClient->accCount == 0) {
                // First account
                newClient->account = *newAccount;
                lastAccount = &newClient->account;
            }
            else {
                // Subsequent accounts
                lastAccount->nextAccount = newAccount;
                newAccount->prevAccount = lastAccount;
                lastAccount = newAccount;
            }
            newClient->accCount++;
        }
        addClient(db, newClient);
    }
    fclose(file);
    return db;
}
void updateClientDB(ClientDB* db)
{
    printf("Please choose an option:\n");
    printf("1. Add a new client\n");
    printf("2. Remove a client\n");
    printf("3. Update a client\n");
    printf("4. View a client\n");
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
    {
        client* newClient = createClient();
        addClient(db, newClient);
        break;
    }
    case 2:
    {
        printf("Enter the ID of the client you want to remove: ");
        char ID[ID_SIZE];
        scanf("%s", ID);
        client* clientToRemove = findClient(db, ID);
        if (clientToRemove != NULL) {
            removeClient(db, clientToRemove);
        }
        else {
            printf("Client with ID %s not found.\n", ID);
        }
        break;
    }
    case 3:
    {
        printf("Enter the ID of the client you want to update: ");
        char ID[ID_SIZE];
        scanf("%s", ID);
        client* clientToUpdate = findClient(db, ID);
        if (clientToUpdate != NULL) {
            updateClient(clientToUpdate);
        }
        else {
            printf("Client with ID %s not found.\n", ID);
        }
        break;
    }
    case 4:
    {
        printf("Enter the ID of the client you want to view: ");
        char ID[ID_SIZE];
        scanf("%s", ID);
        client* clientToView = findClient(db, ID);
        if (clientToView != NULL) {
            viewClient(clientToView);
        }
        else {
            printf("Client with ID %s not found.\n", ID);
        }
        break;
    }
    default:
        printf("Invalid choice.\n");
        break;
    }
}
void writeClientDataBinary(const ClientDB* db, const char* fileName) {
    FILE* file = fopen(fileName, "wb"); // "wb" mode for writing binary data
    if (file == NULL) {
        printf("Failed to open file %s\n", fileName);
        return;
    }

    LNode* current = db->clientsList->head;
    while (current != NULL) {
        client* clientData = (client*)getNodeData(current);

        // Write client ID
        fwrite(clientData->ID, sizeof(char), ID_SIZE, file);

        // Write the length of the name and then the name itself
        size_t nameLength = strlen(clientData->name) + 1;
        fwrite(&nameLength, sizeof(size_t), 1, file);
        fwrite(clientData->name, sizeof(char), nameLength, file);

        // Write the account count
        fwrite(&clientData->accCount, sizeof(int), 1, file);

        // Write all accounts for this client
        BankAccount* currentAccount = &clientData->account;
        while (currentAccount != NULL) {
            fwrite(currentAccount, sizeof(BankAccount), 1, file);
            currentAccount = currentAccount->nextAccount;
        }

        current = current->next;
    }

    fclose(file);
    printf("Binary file %s written successfully.\n", fileName);
}
void readClientDataBinary(ClientDB* db, const char* fileName) {
    FILE* file = fopen(fileName, "rb"); // "rb" mode for reading binary data
    if (file == NULL) {
        printf("Failed to open file %s\n", fileName);
        return;
    }

    while (1) {
        client* newClient = (client*)malloc(sizeof(client));
        if (newClient == NULL) {
            printf("Memory allocation failed\n");
            fclose(file);
            return;
        }

        // Read client ID
        if (fread(newClient->ID, sizeof(char), ID_SIZE, file) != ID_SIZE) {
            free(newClient);
            break; // End of file
        }

        // Read the length of the name and then the name itself
        size_t nameLength;
        fread(&nameLength, sizeof(size_t), 1, file);
        newClient->name = (char*)malloc(nameLength);
        if (newClient->name == NULL) {
            printf("Memory allocation failed\n");
            free(newClient);
            fclose(file);
            return;
        }
        fread(newClient->name, sizeof(char), nameLength, file);

        // Read the account count
        fread(&newClient->accCount, sizeof(int), 1, file);

        // Read all accounts for this client
        BankAccount* lastAccount = NULL;
        for (int i = 0; i < newClient->accCount; i++) {
            BankAccount* newAccount = (BankAccount*)malloc(sizeof(BankAccount));
            if (newAccount == NULL) {
                printf("Memory allocation failed\n");
                free(newClient->name);
                free(newClient);
                fclose(file);
                return;
            }
            fread(newAccount, sizeof(BankAccount), 1, file);

            if (i == 0) {
                newClient->account = *newAccount;
                lastAccount = &newClient->account;
            }
            else {
                lastAccount->nextAccount = newAccount;
                newAccount->prevAccount = lastAccount;
                lastAccount = newAccount;
            }
        }

        addClient(db, newClient);
    }

    fclose(file);
    printf("Binary file %s read successfully.\n", fileName);
}
void printClientDB(const ClientDB* db) {
    if (db == NULL || db->clientsList == NULL) {
        printf("Client database is empty or not initialized.\n");
        return;
    }

    printf("Client Database:\n");
    printf("Number of clients: %d\n", db->clientCount);

    LNode* currentNode = db->clientsList->head;
    while (currentNode != NULL) {
        client* currentClient = (client*)getNodeData(currentNode);
        viewClient(currentClient);
        currentNode = currentNode->next;
    }
}

void freeClientDB(ClientDB* db) {
    if (db == NULL || db->clientsList == NULL) return;

    LNode* currentNode = db->clientsList->head;
    while (currentNode != NULL) {
        client* currentClient = (client*)getNodeData(currentNode);
        freeClient(currentClient);
        currentNode = currentNode->next;
    }

    deleteList(db->clientsList);

    free(db);
}


