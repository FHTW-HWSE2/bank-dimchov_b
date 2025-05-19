#include "../include/account.h"
#include "../include/main.h"
#include "../include/customer.h"
#include "../include/types.h"
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
    double amount;

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
        return 1;
    }

    if (user->account == INITIAL_BALANCE) {
        printf("What is the initial balance you want to put on your account?: ");
        scanf("%lf", &amount);
        if (amount <= 100000.00) {
            user->balance = amount;
        }
        else {
            printf("Where did you get all that money? Very sus! Go somewhere else!\n");
            return 1;
        }
    }
    return 0;
}

int create_account(User *user) {
    printf("Do you already have one or more accounts in our bank?");
    printf("\n 0 - No");
    printf("\n 1 - Yes\n");
    int new_customer = get_int_value();
    if (new_customer == 1) {
        if (login(user) == 1) {
            printf("Great! We will create a new account for you.\n");

            //New account for same client:
            User *user2 = malloc(sizeof(User));
            if (user2 == NULL) {
                perror("Error allocating memory for user2");
                return 1;
            }
            strcpy(user2->name, user->name);
            strcpy(user2->SSN, user->SSN);

            int account_type = chooseAccountType(user2);
            if (account_type == 1) {
                return 1;
            }
            user2->account_number = check_number_of_accounts(user2->name, user2->SSN) + 1;
            save_account_to_csv(user2, "../customers.csv");
        }
    }
    else if (new_customer == 0) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("Please enter your name: ");
        fgets(user->name, sizeof(user->name), stdin);

        size_t length = strlen(user->name);
        if (length > 0 && user->name[length - 1] == '\n') {
            user->name[length - 1] = '\0';
        }
        generate_ssn(user);
        int account_type = chooseAccountType(user);
        if (account_type == 1) {
            return 1;
        }
        user->account_number = 1;
        save_account_to_csv(user, "../customers.csv");
    }
    else {
        printf("Invalid entry.\n");
    }
    return 0;
}

int login(User *user) {
    char name[200];
    char ssn[9];
    int number_of_account = 1;

    printf("Please login to your account.\n");
    printf("Enter accountname: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    fgets(name, sizeof(name), stdin);

    size_t length = strlen(name);
    if (length > 0 && name[length - 1] == '\n') {
        name[length - 1] = '\0';
    }

    printf("Enter SSN: ");
    scanf("%s", ssn);

    if (check_number_of_accounts(name, ssn) > 1) {
        printf("You have more than one account in our bank.\n"
               "Please enter account number of the account you want access to:\n");
        number_of_account = get_int_value();
    }
    if (read_customer_data(user, name, ssn, &number_of_account)) {
        printf("Successfully logged in.\n");
    } else {
        printf("Account does not exist.\n");
        return 0;
    }
    return 1;
}

int get_int_value() {
    char input[2];
    int value = 1;
    int d;
    while ((d = getchar()) != '\n' && d != EOF);
    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (sscanf(input, "%d", &value) != 1) {
            printf("Invalid input.\n");
            return -1;
        }
    }
    return value;
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
    csv_header(temp);

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
        int account_number;

        fgets(line, sizeof(line), input);

        while (fgets(line, sizeof(line), input)) {
            if (!parse_customer_line(line, name, ssn, &account, &balance, &account_number)) {
                continue;
            }
        
            if(strcmp(name, user->name) == 0 && strcmp(ssn, user->SSN) == 0 && account_number == user->account_number) {
                found = true; 
                continue;
            }

            fprintf(temp, "%s,%s,%d,%.2lf,%d\n", name, ssn, account, balance, account_number);
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

int check_number_of_accounts(const char *name, const char *ssn) {
    char line[256];
    char file_name[200], file_ssn[20];
    int file_account;
    double file_balance;
    int file_account_number;
    int count = 0;

    FILE *file = fopen("../customers.csv", "r");
    if (!file) {
        perror("Could not open customers file");
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        if (!parse_customer_line(line, file_name, file_ssn, &file_account, &file_balance, &file_account_number)) {
            continue;
        }
        if (strcmp(file_name, name) == 0 && strcmp(file_ssn, ssn) == 0) {
            count++;
        }
    }
    fclose(file);
    if (count == 0) {
        return EXIT_FAILURE;
    }
    return count;
}