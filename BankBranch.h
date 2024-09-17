#ifndef BANKBRANCH_H
#define BANKBRANCH_H

#include "client.h"
#include "clerk.h"
#include "ClientDB.h"
#include "LoanSystem.h" 


typedef enum {
    eNone,
    eClerkID,
    eClerkName,
    eClerkPosition,
    eNofSortOpt
} eSortOption;


typedef struct {
    int branchId;
    char address[150];
    Clerk** clerks;
    eSortOption clerkSortOpt;
    int numberOfClerks;
    ClientDB* DB;
} BankBranch;


BankBranch* createBankBranch(int branchId, const char* address);
void addClerk(BankBranch* branch, Clerk* newClerk);
void deposit(BankBranch* branch, const char* accountNumber, double amount);
void withdraw(BankBranch* branch,const char* accountNumber, double amount);
Clerk* findClerkById(BankBranch* branch, int clerkId);
void saveBankBranchToFile(const BankBranch* branch, const char* filename);
BankBranch* loadBankBranchFromFile(const char* filename);
void saveBankBranchToFileBin(const BankBranch* branch, const char* filename);
BankBranch* loadBankBranchFromFileBin(const char* filename);
void sortClerks(BankBranch* branch);
void findClerk(const BankBranch* branch);
void printBankBranch(const BankBranch* branch);
void printClerksArr(Clerk** clerks, int size);
void freeClerksArr(Clerk** clerks, int size);
void freeBankBranch(BankBranch* branch);
void promoteAllClerks(Clerk** clerks, int size);
void updateClerks(BankBranch* branch);
void manageClerksInBankSystem(BankBranch* branch);
#endif // BANKBRANCH_H