#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Client.h"
#include "functions.h"

client* head = NULL;
client* tail = NULL;

client* createClient() {
    client* newClient = (client*)malloc(sizeof(client));
    if (newClient == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    do {
        printf("Enter the ID of the client: ");
        scanf("%s", newClient->ID);
        if (strlen(newClient->ID) != ID_SIZE - 1) { 
            printf("The ID must be 9 digits long, please try again:\n");
        }
        else if ((isNumeric(newClient->ID)) == 0) {
            printf("The ID must contain only digits, please try again:\n");
        }
    } while (strlen(newClient->ID) != ID_SIZE - 1 || (isNumeric(newClient->ID)) == 0);

    newClient->name = (char*)malloc(ID_SIZE * sizeof(char));
    if (newClient->name == NULL) {
        printf("Memory allocation failed\n");
        free(newClient);
        exit(1);
    }
    printf("Enter the name of the client: ");
    scanf("%s", newClient->name);
    newClient->account = *createAccount();  
    newClient->accCount = 1;  
    return newClient;
}

BankAccount* createAccount() {
    BankAccount* newAccount = (BankAccount*)malloc(sizeof(BankAccount));
    if (newAccount == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    do {
        printf("Enter the account number: ");
        scanf("%s", newAccount->accountNumber);
        if (strlen(newAccount->accountNumber) != BANK_ID_ACC_SIZE - 1) { 
            printf("The account number must be 8 digits long, please try again:\n");
        }
        else if (!isNumeric(newAccount->accountNumber)) {
            printf("The account number must contain only digits, please try again:\n");
        }
    } while (strlen(newAccount->accountNumber) != BANK_ID_ACC_SIZE - 1 || !isNumeric(newAccount->accountNumber));

    printf("Enter the balance: ");
    scanf("%lf", &newAccount->balance);

	newAccount->nextAccount = NULL;
	newAccount->prevAccount = NULL;
    return newAccount;
}


void initAccountForClient(client* client, BankAccount* account) {
    if (client->accCount == 0) {
        client->account = *account;
        client->account.nextAccount = NULL;
        client->account.prevAccount = NULL;
    }
    else {
        BankAccount* lastAccount = &client->account;
        while (lastAccount->nextAccount != NULL) {
            lastAccount = lastAccount->nextAccount;
        }

        lastAccount->nextAccount = account;
        account->prevAccount = lastAccount;
        account->nextAccount = NULL;  
    }

    client->accCount++;
}
void viewClient(client* clientToView) {
    printf("Client ID: %s\n", clientToView->ID);
    printf("Client name: %s\n", clientToView->name);
    printf("Number of accounts: %d\n", clientToView->accCount);

    BankAccount* temp = &clientToView->account;
    while (temp != NULL) {
        viewAccount(temp);
        temp = temp->nextAccount;
    }
}

client* updateClient(client* clientToUpdate)
{
	printf("Please choose what you want to update:\n");
    printf("1. ID\n");
    printf("2. Name\n");
	printf("3. Add or delete accounts\n ");
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        do {
            printf("Enter the ID of the client: ");
            scanf("%s", clientToUpdate->ID);
            if (strlen(clientToUpdate->ID) != ID_SIZE - 1) {  // -1 because ID_SIZE includes '\0'
                printf("The ID must be 9 digits long, please try again:\n");
            }
            else if ((isNumeric(clientToUpdate->ID)) == 0) {
                printf("The ID must contain only digits, please try again:\n");
            }
        } while (strlen(clientToUpdate->ID) != ID_SIZE - 1 || (isNumeric(clientToUpdate->ID)) == 0);
        break;
    case 2:
        printf("Enter the name of the client: ");
        scanf("%s", clientToUpdate->name);
        break;
    case 3:
        printf("Do you want to add or delete an account?\n");
        printf("1. Add account\n");
        printf("2. Delete account\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) {
            BankAccount* newAccount = createAccount(clientToUpdate);
            initAccountForClient(clientToUpdate, newAccount);
            printf("Account added successfully.\n");
        }
        if (choice == 2) {
            printf("Enter the account number you want to delete: ");
            char accNum[BANK_ID_ACC_SIZE];
            scanf("%s", accNum);
            deleteAccount(clientToUpdate, accNum);
        }
        else {
            printf("Invalid choice.\n");
        }
        break;
    }
	return clientToUpdate;
}
void viewAccount(BankAccount* accountToView) {
	printf("Account number: %s\n", accountToView->accountNumber);
	printf("Balance: %.2lf\n", accountToView->balance);

}
void deleteClient(client* clientToDelete) {
    	free(clientToDelete->name);
	    free(clientToDelete);
}

BankAccount* findAccountInClient(client* client, char* accNum) {
    BankAccount* temp = &client->account;
    while (temp != NULL) {
        if (strcmp(temp->accountNumber, accNum) == 0) {
            return temp;
        }
        temp = temp->nextAccount;
    }
    return NULL;
}

void deleteAccount(client* client, char* accNum) {
    BankAccount* accountToDelete = findAccountInClient(client, accNum);

    if (accountToDelete == NULL) {
        printf("Account not found.\n");
        return;
    }

    if (client->accCount == 1) {
        printf("Cannot delete the only account.\n");
        return;
    }

    if (accountToDelete == &client->account) {
        client->account = *accountToDelete->nextAccount;
        client->account.prevAccount = NULL;
    }
    else {
        if (accountToDelete->prevAccount != NULL) {
            accountToDelete->prevAccount->nextAccount = accountToDelete->nextAccount;
        }
        if (accountToDelete->nextAccount != NULL) {
            accountToDelete->nextAccount->prevAccount = accountToDelete->prevAccount;
        }
    }

    free(accountToDelete);
    client->accCount--;
}

void updateAccount(client* client, char* accNum) {
    BankAccount* temp = findAccountInClient(client, accNum);
    if (temp == NULL) {
        printf("Account not found.\n");
        return;
    }
    printf("Please choose what you want to update:\n");
    printf("1. Account number\n");
    printf("2. Balance\n");
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        do {
            printf("Enter the account number: ");
            scanf("%s", temp->accountNumber);
            if (strlen(temp->accountNumber) != BANK_ID_ACC_SIZE - 1) {  // -1 because BANK_ID_ACC_SIZE includes '\0'
                printf("The account number must be 8 digits long, please try again:\n");
            }
            else if ((isNumeric(temp->accountNumber)) == 0) {
                printf("The account number must contain only digits, please try again:\n");
            }
        } while (strlen(temp->accountNumber) != BANK_ID_ACC_SIZE - 1 || (isNumeric(temp->accountNumber)) == 0);
        break;
    case 2:
        printf("Enter the balance: ");
        scanf("%lf", &temp->balance);
        break;
    }
}
void freeClient(client* clientToDelete) {
    if (clientToDelete == NULL) return;

    if (clientToDelete->name != NULL) {
        free(clientToDelete->name);
    }

    BankAccount* currentAccount = clientToDelete->account.nextAccount;
    while (currentAccount != NULL) {
        BankAccount* temp = currentAccount;
        currentAccount = currentAccount->nextAccount;
        free(temp);
    }

    free(clientToDelete);
}