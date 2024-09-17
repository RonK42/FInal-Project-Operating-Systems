#define _CRT_SECURE_NO_WARNINGS
#include "bankbranch.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "functions.h"

static const char* sortOptStr[eNofSortOpt] = { "None", "ID", "Name", "Position" };

BankBranch* createBankBranch(int branchId, const char* address) {
    BankBranch* branch = (BankBranch*)malloc(sizeof(BankBranch));
    if (!branch) {
        printf("ERROR");
        return NULL;
    }
    branch->branchId = branchId;
    strcpy(branch->address, address);
    branch->clerks = NULL;
    branch->numberOfClerks = 0;
    branch->DB = createDBFromTextFile("clientsData.txt");
    return branch;
}

void addClerk(BankBranch* branch, Clerk* newClerk) {
    Clerk** temp = (Clerk**)realloc(branch->clerks, sizeof(Clerk*) * (branch->numberOfClerks + 1));
    if (temp != NULL) {
        branch->clerks = temp;
        branch->clerks[branch->numberOfClerks++] = newClerk;
    }
    else {
        printf("Error: Memory allocation failed while adding a new clerk.\n");
    }
}


void deposit(BankBranch* branch, const char* accountNumber, double amount) {
    BankAccount* account = findAccount(branch->DB, accountNumber);
    if (account != NULL) {
        account->balance += amount;
        printf("Deposit completed. New balance: %.2f\n", account->balance);
    }
    else {
        printf("Account not found.\n");
    }
}

void withdraw(BankBranch* branch, const char* accountNumber, double amount) {
    BankAccount* account = findAccount(branch->DB, accountNumber);
    if (account != NULL) {
        if (account->balance >= amount) {
            account->balance -= amount;
            printf("Withdrawal completed. New balance: %.2f\n", account->balance);
        }
        else {
            printf("Insufficient funds.\n");
        }
    }
    else {
        printf("Account not found.\n");
    }
}
Clerk* findClerkById(BankBranch* branch, int clerkId) {
    for (int i = 0; i < branch->numberOfClerks; i++) {
        if (branch->clerks[i]->clerkId == clerkId) {
            return branch->clerks[i];
        }
    }
    return NULL;
}
void saveBankBranchToFile(const BankBranch* branch, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    // Save the branch ID and address
    fprintf(file, "Branch ID: %d\n", branch->branchId);
    fprintf(file, "Address: %s\n", branch->address);

    // Save the number of clerks
    fprintf(file, "Number of Clerks: %d\n", branch->numberOfClerks);

    // Save each clerk's information
    for (int i = 0; i < branch->numberOfClerks; i++) {
        Clerk* clerk = branch->clerks[i];
        fprintf(file, "Clerk ID: %d, Name: %s, Position: %d\n",
            clerk->clerkId, clerk->name, clerk->positionLevel);
    }

    fclose(file);
    printf("Bank branch saved successfully to %s\n", filename);
}
BankBranch* loadBankBranchFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file for reading.\n");
        return NULL;
    }

    BankBranch* branch = (BankBranch*)malloc(sizeof(BankBranch));
    if (!branch) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    // Set the ClientDB to NULL as we are not loading it from the file
    branch->DB = NULL;

    // Read branch ID and address
    fscanf(file, "Branch ID: %d\n", &branch->branchId);
    fscanf(file, "Address: %[^\n]\n", branch->address);

    // Read the number of clerks
    fscanf(file, "Number of Clerks: %d\n", &branch->numberOfClerks);

    // Allocate memory for clerks
    branch->clerks = (Clerk**)malloc(sizeof(Clerk*) * branch->numberOfClerks);
    if (!branch->clerks) {
        printf("Error: Memory allocation failed.\n");
        free(branch);
        fclose(file);
        return NULL;
    }

    char buffer[256]; // Buffer to read each line
    for (int i = 0; i < branch->numberOfClerks; i++) {
        branch->clerks[i] = (Clerk*)malloc(sizeof(Clerk));
        if (!branch->clerks[i]) {
            printf("Error: Memory allocation failed.\n");
            for (int j = 0; j < i; j++) {
                free(branch->clerks[j]->name);
                free(branch->clerks[j]);
            }
            free(branch->clerks);
            free(branch);
            fclose(file);
            return NULL;
        }

        // Allocate memory for the name field
        branch->clerks[i]->name = (char*)malloc(50 * sizeof(char)); // Assuming a max length of 50 for names
        if (!branch->clerks[i]->name) {
            printf("Error: Memory allocation for clerk name failed.\n");
            for (int j = 0; j <= i; j++) {
                free(branch->clerks[j]->name);
                free(branch->clerks[j]);
            }
            free(branch->clerks);
            free(branch);
            fclose(file);
            return NULL;
        }

        // Read a line and parse it
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            if (sscanf(buffer, "Clerk ID: %d, Name: %[^,], Position: %d",
                &branch->clerks[i]->clerkId, branch->clerks[i]->name,
                (int*)&branch->clerks[i]->positionLevel) != 3) {
                printf("Error: Failed to read clerk data.\n");
                for (int j = 0; j <= i; j++) {
                    free(branch->clerks[j]->name);
                    free(branch->clerks[j]);
                }
                free(branch->clerks);
                free(branch);
                fclose(file);
                return NULL;
            }
        }
        else {
            printf("Error: Unexpected end of file or reading error.\n");
            for (int j = 0; j <= i; j++) {
                free(branch->clerks[j]->name);
                free(branch->clerks[j]);
            }
            free(branch->clerks);
            free(branch);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    printf("Bank branch loaded successfully from %s\n", filename);
    return branch;
}



void saveBankBranchToFileBin(const BankBranch* branch, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    // Save the branch ID
    fwrite(&branch->branchId, sizeof(int), 1, file);

    // Save the address
    fwrite(branch->address, sizeof(char), 150, file);

    // Save the number of clerks
    fwrite(&branch->numberOfClerks, sizeof(int), 1, file);

    // Save each clerk's information
    for (int i = 0; i < branch->numberOfClerks; i++) {
        Clerk* clerk = branch->clerks[i];
        fwrite(&clerk->clerkId, sizeof(int), 1, file);
        fwrite(clerk->name, sizeof(char), 50, file);  // Assuming name is 50 chars max
        fwrite(&clerk->positionLevel, sizeof(int), 1, file);
    }

    fclose(file);
    printf("Bank branch saved successfully to %s\n", filename);
}
BankBranch* loadBankBranchFromFileBin(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error: Could not open file for reading.\n");
        return NULL;
    }

    BankBranch* branch = (BankBranch*)malloc(sizeof(BankBranch));
    if (!branch) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    // Set the ClientDB to NULL as we are not loading it from the file
    branch->DB = NULL;

    // Read branch ID
    fread(&branch->branchId, sizeof(int), 1, file);

    // Read the address (assuming the fixed size of 150 bytes for the address)
    fread(branch->address, sizeof(char), 150, file);

    // Read the number of clerks
    fread(&branch->numberOfClerks, sizeof(int), 1, file);

    // Allocate memory for clerks
    branch->clerks = (Clerk**)malloc(sizeof(Clerk*) * branch->numberOfClerks);
    if (!branch->clerks) {
        printf("Error: Memory allocation failed.\n");
        free(branch);
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < branch->numberOfClerks; i++) {
        branch->clerks[i] = (Clerk*)malloc(sizeof(Clerk));
        if (!branch->clerks[i]) {
            printf("Error: Memory allocation failed.\n");
            for (int j = 0; j < i; j++) {
                free(branch->clerks[j]->name);
                free(branch->clerks[j]);
            }
            free(branch->clerks);
            free(branch);
            fclose(file);
            return NULL;
        }

        // Read the clerk ID
        fread(&branch->clerks[i]->clerkId, sizeof(int), 1, file);

        // Allocate memory for the name field and read the name (assuming max 50 characters)
        branch->clerks[i]->name = (char*)malloc(50 * sizeof(char));
        if (!branch->clerks[i]->name) {
            printf("Error: Memory allocation for clerk name failed.\n");
            for (int j = 0; j <= i; j++) {
                free(branch->clerks[j]->name);
                free(branch->clerks[j]);
            }
            free(branch->clerks);
            free(branch);
            fclose(file);
            return NULL;
        }
        fread(branch->clerks[i]->name, sizeof(char), 50, file);

        // Read the clerk's position level
        fread(&branch->clerks[i]->positionLevel, sizeof(int), 1, file);
    }

    fclose(file);
    printf("Bank branch loaded successfully from %s\n", filename);
    return branch;
}


eSortOption showSortMenu() {
    int opt;
    printf("Base on what field do you want to sort?\n");
    do {
        for (int i = 1; i < eNofSortOpt; i++)
            printf("Enter %d for %s\n", i, sortOptStr[i]);
        scanf("%d", &opt);
    } while (opt <= eNone || opt >= eNofSortOpt);

    return (eSortOption)opt;
}


void sortClerks(BankBranch* branch) {
    branch->clerkSortOpt = showSortMenu();

    int(*compare)(const void* clerk1, const void* clerk2) = NULL;

    switch (branch->clerkSortOpt) {
    case eClerkID:
        compare = compareClerkByID;
        break;
    case eClerkName:
        compare = compareClerkByName;
        break;
    case eClerkPosition:
        compare = compareClerkByPosition;
        break;
    }

    if (compare != NULL) {
        qsort(branch->clerks, branch->numberOfClerks, sizeof(Clerk*), compare);
        printf("Clerks sorted successfully.\n");
    }
    else {
        printf("Invalid sort option.\n");
    }
}

void findClerk(const BankBranch* branch) {
    eSortOption sortOption = branch->clerkSortOpt;
    int(*compare)(const void*, const void*) = NULL;
    Clerk c = { 0 };
    Clerk* pClerk = &c;

    switch (sortOption) {
    case eClerkID:
        printf("Enter Clerk ID: ");
        scanf("%d", &c.clerkId);
        compare = compareClerkByID;
        break;

    case eClerkName:
        printf("Enter Clerk Name: ");
        scanf("%s", c.name);
        compare = compareClerkByName;
        break;

    case eClerkPosition:
        printf("Enter Clerk Position (number): ");
        scanf("%d", (int*)&c.positionLevel);
        compare = compareClerkByPosition;
        break;
    }

    if (compare != NULL) {
        Clerk** pF = bsearch(&pClerk, branch->clerks, branch->numberOfClerks, sizeof(Clerk*), compare);
        if (pF == NULL)
            printf("Clerk was not found\n");
        else {
            printf("Clerk found:\n");
            printf("ID: %d, Name: %s, Position: %d\n", (*pF)->clerkId, (*pF)->name, (*pF)->positionLevel);
        }
    }
    else {
        printf("The search cannot be performed, invalid sort option selected\n");
    }
}

void printBankBranch(const BankBranch* branch) {
    printf("Bank Branch ID: %d\n", branch->branchId);
    printf("Address: %s\n", branch->address);

    printf("\n-------- Has %d clerks\n", branch->numberOfClerks);
    printClerksArr(branch->clerks, branch->numberOfClerks);

    if (branch->DB != NULL) {
        printf("\n-------- Has %d clients\n", branch->DB->clientCount);
        printClientDB(branch->DB);
    }
}



void printClerksArr(Clerk** clerks, int size) {
    generalArrayFunction(clerks, size, sizeof(Clerk*), printClerk);
}

void freeClerksArr(Clerk** clerks, int size) {
    generalArrayFunction(clerks, size, sizeof(Clerk*), freeClerk);
    free(clerks);
}

void freeBankBranch(BankBranch* branch) {
    if (branch == NULL) return;

    freeClerksArr(branch->clerks, branch->numberOfClerks);

  //  if (branch->DB != NULL) {
		////freeClientDB(branch->DB);  Need to repair 
  //      free(branch->DB);
  //  }

    free(branch);
}

void promoteAllClerks(Clerk** clerks, int size) {
    generalArrayFunction(clerks, size, sizeof(Clerk*), promoteClerk);
}

void updateClerks(BankBranch* branch) {
    promoteAllClerks(branch->clerks, branch->numberOfClerks);
    printf("All clerks have been promoted.\n");
}
void manageClerksInBankSystem(BankBranch* branch) {
    int clerkId, action;

    // Receiving input from the user
    printf("Enter Clerk ID: ");
    scanf("%d", &clerkId);

    printf("Enter Action (1 for Level Up, 2 for Level Down): ");
    scanf("%d", &action);

    Clerk* clerk = findClerkById(branch, clerkId);
    if (clerk == NULL) {
        printf("Clerk with ID %d not found in branch %d.\n", clerkId, branch->branchId);
        return;
    }

    switch (action) {
    case 1:  // Level up
        promoteClerk(clerk);
        break;
    case 2:  // Level down
        demoteClerk(clerk);
        break;
    default:
        printf("Invalid action.\n");
    }
}


