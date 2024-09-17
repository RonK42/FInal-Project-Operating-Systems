#ifndef LOANSYSTEM_H
#define LOANSYSTEM_H

#include "loan.h"
#include "ClientDB.h"

typedef struct {
    Loan** loans; 
    int numberOfLoans;
} LoanSystem;

LoanSystem* createLoanSystem();
Loan* findLoanById(LoanSystem* loanSystem, int loanId);
void addLoan(LoanSystem* loanSystem, ClientDB* DB);
Loan* findLoanByAccount(LoanSystem* loanSystem, char* accountNumber);
void addLoanToClient(Loan* loan, ClientDB* DB);
void saveLoanSystemToFile(LoanSystem* loanSystem, const char* filename);
LoanSystem* loadLoanSystemFromFile(const char* filename);
void saveLoanSystemToBinaryFile(LoanSystem* loanSystem, const char* filename);
LoanSystem* loadLoanSystemFromBinaryFile(const char* filename);
void freeLoanSystem(LoanSystem* loanSystem);
void printLoanByAccNum(LoanSystem* loanSystem);

#endif
