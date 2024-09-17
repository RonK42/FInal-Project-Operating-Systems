#define _CRT_SECURE_NO_WARNINGS
#include "loan.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "functions.h"

Loan* createLoan(char* accountNumber) {
    Loan* loan = (Loan*)malloc(sizeof(Loan));
    if (!loan)
    {
        printf("ERROR");
        return NULL;
    }
	printf("Enter loan ID: ");
	scanf("%d", &loan->loanId); 
    strcpy(loan->accountNumber, accountNumber);
    printf("Enter principal amount: ");
    scanf("%lf", &loan->principalAmount);
    printf("Enter interest rate (3, 5, 7): ");
    scanf("%d", &loan->interestRate);
    return loan;
}
printLoan(Loan* loan) {
	printf("Loan ID: %d\n", loan->loanId);
	printf("Account number: %s\n", loan->accountNumber);
	printf("Principal amount: %.2f\n", loan->principalAmount);
	printf("Interest rate: %d\n", loan->interestRate);
}



