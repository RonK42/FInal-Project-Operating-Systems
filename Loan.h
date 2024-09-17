#ifndef LOAN_H
#define LOAN_H

#include "date.h"



typedef enum {
    LOW_RATE = 3,
    MEDIUM_RATE = 5,
    HIGH_RATE = 7
} InterestRate;


typedef struct {
    int loanId;
    char accountNumber[9];
    double principalAmount;
    InterestRate interestRate;
} Loan;
Loan* createLoan(char* accountNumber);
printLoan(Loan* loan);

#endif

