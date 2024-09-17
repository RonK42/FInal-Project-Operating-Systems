#define _CRT_SECURE_NO_WARNINGS
#include "loansystem.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include "functions.h"

LoanSystem* createLoanSystem() {
    LoanSystem* loanSystem = (LoanSystem*)malloc(sizeof(LoanSystem));
    if (!loanSystem)
    {
        printf("ERROR");
        return NULL;
    }
    loanSystem->loans = NULL;
    loanSystem->numberOfLoans = 0;
    return loanSystem;
}

void addLoan(LoanSystem* loanSystem,ClientDB* DB) {
	printf("Enter account number: ");
    char accountNumber[BANK_ID_ACC_SIZE];
    scanf("%s", accountNumber);
	Loan* loan = createLoan(accountNumber);
    if (findLoanById(loanSystem, loan->loanId) != NULL) {
        printf("Error: Loan with ID %d already exists.\n", loan->loanId);
        return;
    }
    if (findLoanByAccount(loanSystem, loan->accountNumber) != NULL) {
        printf("Error: Loan for account number %s already exists.\n", loan->accountNumber);
        return;
    }

    Loan** temp = (Loan**)realloc(loanSystem->loans, sizeof(Loan*) * (loanSystem->numberOfLoans + 1));
    if (temp != NULL) {
        loanSystem->loans = temp;
        loanSystem->loans[loanSystem->numberOfLoans++] = loan;
    }
    else {
        printf("Error: Memory allocation failed while adding a new loan.\n");
    }
	addLoanToClient(loan, DB);
	printf("Loan successfully added to the system.\n");
}
void addLoanToClient(Loan* loan, ClientDB* DB) {
    BankAccount* acc = findAccount(DB, loan->accountNumber);
	if (acc == NULL) {
		printf("Error: Account with number %s not found.\n", loan->accountNumber);
		return;
	}
    else {
		acc->balance += loan->principalAmount;
		printf("Account balance updated after loan addition. New balance: %.2f\n", acc->balance);
    }
}

Loan* findLoanById(LoanSystem* loanSystem, int loanId) {
    for (int i = 0; i < loanSystem->numberOfLoans; i++) {
        if (loanSystem->loans[i]->loanId == loanId) {
            return loanSystem->loans[i];
        }
    }
	return NULL; // Not found ID
}

Loan* findLoanByAccount(LoanSystem* loanSystem, char* accountNumber) {
    for (int i = 0; i < loanSystem->numberOfLoans; i++) {
        if (strcmp(loanSystem->loans[i]->accountNumber, accountNumber) == 0) {
            return loanSystem->loans[i];
        }
    }
    return NULL; // Not found ID
}
void saveLoanSystemToFile(LoanSystem* loanSystem, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }

    fprintf(file, "%d\n", loanSystem->numberOfLoans);  // Write the number of loans first

    for (int i = 0; i < loanSystem->numberOfLoans; i++) {
        Loan* loan = loanSystem->loans[i];
        fprintf(file, "%d %s %.2f %d\n",
            loan->loanId,
            loan->accountNumber,
            loan->principalAmount,
            loan->interestRate);
    }

    fclose(file);
    printf("Loan system saved successfully to %s.\n", filename);
}
LoanSystem* loadLoanSystemFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return NULL;
    }

    LoanSystem* loanSystem = createLoanSystem();
    if (!loanSystem) {
        fclose(file);
        return NULL;
    }

    int numberOfLoans;
    fscanf(file, "%d", &numberOfLoans);  // Read the number of loans

    for (int i = 0; i < numberOfLoans; i++) {
        Loan* loan = (Loan*)malloc(sizeof(Loan));
        if (!loan) {
            printf("Error: Memory allocation failed while loading loans.\n");
            fclose(file);
            return loanSystem;  // Return what we have loaded so far
        }

        fscanf(file, "%d %s %lf %d",
            &loan->loanId,
            loan->accountNumber,
            &loan->principalAmount,
            (int*)&loan->interestRate);

        Loan** temp = (Loan**)realloc(loanSystem->loans, sizeof(Loan*) * (loanSystem->numberOfLoans + 1));
        if (temp != NULL) {
            loanSystem->loans = temp;
            loanSystem->loans[loanSystem->numberOfLoans++] = loan;
        }
        else {
            printf("Error: Memory allocation failed while loading loans.\n");
            free(loan);
        }
    }

    fclose(file);
    printf("Loan system loaded successfully from %s.\n", filename);
    return loanSystem;
}
void saveLoanSystemToBinaryFile(LoanSystem* loanSystem, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }

    fwrite(&loanSystem->numberOfLoans, sizeof(int), 1, file);  // Write the number of loans first

    for (int i = 0; i < loanSystem->numberOfLoans; i++) {
        Loan* loan = loanSystem->loans[i];
        fwrite(loan, sizeof(Loan), 1, file);  // Write each Loan struct
    }

    fclose(file);
    printf("Loan system saved successfully to %s (binary format).\n", filename);
}
LoanSystem* loadLoanSystemFromBinaryFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return NULL;
    }

    LoanSystem* loanSystem = createLoanSystem();
    if (!loanSystem) {
        fclose(file);
        return NULL;
    }

    fread(&loanSystem->numberOfLoans, sizeof(int), 1, file);  // Read the number of loans

    loanSystem->loans = (Loan**)malloc(sizeof(Loan*) * loanSystem->numberOfLoans);
    if (!loanSystem->loans) {
        printf("Error: Memory allocation failed while loading loans.\n");
        fclose(file);
        free(loanSystem);
        return NULL;
    }

    for (int i = 0; i < loanSystem->numberOfLoans; i++) {
        Loan* loan = (Loan*)malloc(sizeof(Loan));
        if (!loan) {
            printf("Error: Memory allocation failed while loading loans.\n");
            fclose(file);
            return loanSystem;  // Return what we have loaded so far
        }

        fread(loan, sizeof(Loan), 1, file);  // Read each Loan struct
        loanSystem->loans[i] = loan;
    }

    fclose(file);
    printf("Loan system loaded successfully from %s (binary format).\n", filename);
    return loanSystem;
}
void freeLoanSystem(LoanSystem* loanSystem) {
    if (loanSystem == NULL) {
        return;
    }

    for (int i = 0; i < loanSystem->numberOfLoans; i++) {
        free(loanSystem->loans[i]);
    }

    free(loanSystem->loans);

    free(loanSystem);

    printf("Loan system memory freed successfully.\n");
}
void printLoanByAccNum(LoanSystem* loanSystem) {
    char accountNumber[BANK_ID_ACC_SIZE];
    printf("Enter account number: ");
    scanf("%s", accountNumber);
    Loan* loan = findLoanByAccount(loanSystem, accountNumber);
    if (loan == NULL) {
        printf("Error: Loan for account number %s not found.\n", accountNumber);
    }
    else {
        printLoan(loan);
    }
}














