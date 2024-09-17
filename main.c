#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ClientDB.h"
#include "BankBranch.h"
#include "Client.h"
#include "LoanSystem.h"

int main() {
    printf("Hello dear administrator.\n");
    printf("Please choose how you want the system to load:\n");
    printf("1. Load from Txt file\n");
    printf("2. Load from Binary file\n");

    int choice;
    scanf("%d", &choice);

    BankBranch* branch = NULL;
    LoanSystem* loanSystem = NULL;

    switch (choice) {
    case 1:
        branch = loadBankBranchFromFile("bankBranchData.txt");
        branch->DB = createDBFromTextFile("clientsData.txt");
        loanSystem = loadLoanSystemFromFile("loansData.txt");
        break;
    case 2:
        branch = loadBankBranchFromFileBin("bankBranchData.bin");
        branch->DB = createClientDB();
        readClientDataBinary(branch->DB, "clientData.bin");
        loanSystem = loadLoanSystemFromBinaryFile("loansData.bin");
        break;
    default:
        printf("Invalid choice.\n");
        return 1;
    }
    while (1) { 
        printf("\n");
        printf("The system has been loaded successfully.\n");
        printf("Please choose what you want to do next:\n");
        printf("1. Print data (Print all current data in the system)\n");
        printf("2. Add/Edit Clients/Clerks\n");
        printf("3. Add Loans to clients\n");
        printf("4. Sort clerks based on the selected criteria\n");
        printf("5. Find a clerk using the current sorting order\n");
        printf("6. Promote all clerks to the next level if possible\n");
        printf("7. Promote or demote a specific clerk based on their ID\n");
        printf("8. Save all data.\n");
        printf("9. Save and exit\n");

        int choice2;
        scanf("%d", &choice2);

        switch (choice2) {
        case 1:
			printf("What do you want to print?\n");
			printf("1. Bank Branch\n");
			printf("2. Loan By Account Num\n");
			int choice4;
			scanf("%d", &choice4);
			switch (choice4) {
			case 1:
                printBankBranch(branch);
                break;
			case 2:
			printLoanByAccNum(loanSystem);
			}
			break;
        case 2:
            printf("Please choose what you want to do next:\n");
            printf("1. Add/Edit/Remove Client\n");
            printf("2. Add Clerk\n");
            int choice3;
            scanf("%d", &choice3);

            switch (choice3) {
            case 1:
                printf("Choice 1.\n");
                updateClientDB(branch->DB);
                break;
            case 2:
                printf("Choice 2.\n");
                Clerk* clerk = createClerk();
                addClerk(branch, clerk);
                break;
            default:
                printf("Invalid choice.\n");
                break;
            }
            break;

        case 3:
            addLoan(loanSystem, branch->DB);
            break;

        case 4:
            sortClerks(branch);
            break;

        case 5:
            findClerk(branch);
            break;

        case 6:
            updateClerks(branch);
            break;

        case 7:
            manageClerksInBankSystem(branch);
            break;

        case 8:
            saveBankBranchToFile(branch, "bankBranchData.txt");
            saveBankBranchToFileBin(branch, "bankBranchData.bin");
            writeClientData(branch->DB, "clientsData.txt");
            writeClientDataBinary(branch->DB, "clientData.bin");
            saveLoanSystemToFile(loanSystem, "loansData.txt");
            saveLoanSystemToBinaryFile(loanSystem, "loansData.bin");
            break;

        case 9:
            saveBankBranchToFile(branch, "bankBranchData.txt");
            saveBankBranchToFileBin(branch, "bankBranchData.bin");
            writeClientData(branch->DB, "clientsData.txt");
            writeClientDataBinary(branch->DB, "clientData.bin");
            saveLoanSystemToFile(loanSystem, "loansData.txt");
            saveLoanSystemToBinaryFile(loanSystem, "loansData.bin");
            freeLoanSystem(loanSystem);
            freeBankBranch(branch);
            printf("Exiting the program.\n");
            return 0;  // Exit the program

        default:
            printf("Invalid choice.\n");
            break;
        }
    }
    return 0;
}