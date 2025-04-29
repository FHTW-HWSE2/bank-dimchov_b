#include "../include/account.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

int generate_ssn(User *user) {
    int ssn = rand() % 90000000 + 10000000;
    snprintf(user->SSN, sizeof(user->SSN), "%d", ssn);

    printf("Your SSN is: %s\n", user->SSN);
    printf("Remember your SSN. It is required for authentication.\n");

    return 0;
}

int chooseAccountType(User *user) {
    int choice;

    printf("\nPlease choose an accounttype:\n");
    printf("0 - standard account\n");
    printf("1 - account with initial balance (max. 100k)\n");
    printf("2 - account with overdraft limit 1000$\n");
    printf("Your choice: ");
    scanf("%d", &choice);
    
    if(choice >=0 && choice <=2) {
        user->account = (Account_type)choice;
        }
    else {
        printf("Invalid entry.\n");
    }
    return 0;
}

int create_account(User *user) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Please enter your name: ");
    fgets(user->name, sizeof(user->name), stdin);

    size_t length = strlen(user->name);
    if (length > 0 && user->name[length - 1] == '\n') {
        user->name[length - 1] = '\0';
    }
    generate_ssn(user);
    chooseAccountType(user);

    if (user->account == INITIAL_BALANCE) {
        printf("What is the initial balance you want to put on your account?:\n ");
        scanf("%lf", &user->balance);
        if (user->balance > 100000) {
            printf("Where did you get all that money? Very sus! Go somewhere else!\n");
            return 1;
        }
    }

    save_account_to_csv(user, "../customers.csv");

    return 0;
}

int login(User *user) {
    char name[200];
    char ssn[9];
    printf("Please login to your account.\n");
    printf("Enter accountname: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Please enter your name: ");
    fgets(name, sizeof(name), stdin);

    size_t length = strlen(name);
    if (length > 0 && name[length - 1] == '\n') {
        name[length - 1] = '\0';
    }

    printf("Enter SSN: ");
    scanf("%s", ssn);

    if (read_customer_data(user, name, ssn)) {
        printf("Successfully logged in.\n");
    } else {
        printf("Account does not exist.\n");
        return 0;
    }
    return 1;
}


int delete_account(User *user) {
    FILE *input = fopen("../customers.csv", "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!input || !temp) {
        perror("Error opening file.");
        if (input) fclose(input);
        if (temp) fclose(temp);
        return 0;
    }

    if (user->balance != 0) {
        if (user->balance < 0) {
            printf("Your balance is negative. You cannot delete the account.\n");
            return 0;
        } else {
            printf("You still have money in your account. Please withdraw it before deletion.\n");
            return 0;
        }
    }
    else {
        char line[256];
        char name[100], ssn[20];
        int account;
        bool found = 0;
        double balance;

        while (fgets(line, sizeof(line), input)) {
            if (!parse_customer_line(line, name, ssn, &account, &balance)) {
                continue;
            }
        
            if(strcmp(name, user->name) == 0 && strcmp(ssn, user->SSN) == 0) {
                found = true; 
                continue;
            }

            fprintf(temp, "%s,%s,%d,%.2lf\n", name, ssn, account, balance);
        }

        fclose(input);
        fclose(temp);

        remove("../customers.csv");
        rename("temp.csv", "../customers.csv");

        if (found) {
            printf("Account for %s (SSN: %s) successfully deleted.\n", name, ssn);
        } else {
            printf("Account not found.\n");
        }
        
    return found;
    }
}

int check_customer_balance(User *user) {
    printf("Your current balance is: $%.2lf\n", user->balance);
    return user->balance;
}