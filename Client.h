#ifndef CLIENT_H
#define CLIENT_H

#define ID_SIZE 10 // 9 digits + '\0'
#define BANK_ID_ACC_SIZE 9 // 8 digits + '\0'

typedef struct BankAccount {
    char accountNumber[BANK_ID_ACC_SIZE];
    double balance;
    struct BankAccount* nextAccount;
    struct BankAccount* prevAccount;
} BankAccount;

typedef struct {
    char ID[ID_SIZE];
    char* name;
    BankAccount account; // The first account
    int accCount;
} client;

void updateAccount(client* client, char* accNum);
BankAccount* findAccountInClient(client* client, char* accNum);
client* createClient();
void viewClient(client* clientToView);
client* updateClient(client* clientToUpdate);
BankAccount* createAccount();
void initAccountForClient(client* client, BankAccount* account);
void viewAccount(BankAccount* accountToView);
void deleteAccount(client* client, char* accNum);
void deleteClient(client* clientToDelete);
void freeClient(client* clientToDelete);

#endif // CLIENT_H
