#include "../include/transaction.h"
#include "../include/account.h"
#include "../include/customer.h"
#include "../include/types.h"
#include <ctype.h>
#include "../include/account.h"
#include "../include/customer.h"
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== TRANSACTION
int clear_buffer(){
    // EXCLUDE from unit test
    #ifndef TEST
        while (getchar() != '\n' &&  getchar() != EOF);
    #endif
        return 0;
}

double parse_amount(const char *input) {
    char *trail_ptr;
    // string to double
    // strtod(const char *input, char **ptr_to_first_char_after_num)
    double amount = strtod(input, &trail_ptr);

    if (trail_ptr == input) {
        printf("Invalid input. Please enter only numbers.\n");
        return 0.0;
    }

    if (amount < 0.0) {
        printf("Negative numbers are not allowed. Please enter only positive numbers.\n");
        return 0.0;
    }

    // Check for further non-whitespace trailing characters
    while (*trail_ptr != '\0') {
        if (!isspace((unsigned char)*trail_ptr)) {
            printf("Invalid characters found. Please enter only numbers.\n");
            return 0.0;
        }
        trail_ptr++;
    }

    return amount;
}

double validate_amount_for_transaction(const char *action) {
    char buffer[50];
    clear_buffer();
    printf("Enter amount to %s: $", action);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Failed to read input. Please enter only numbers.\n");
        return 0.0;
    }


// ===== DEPOSIT
    double amount = parse_amount(buffer);
    return amount;
}

// ===== DEPOSIT
double validate_amount_to_deposit(const char *input) {
    double amount;
    // scanf("%lf", &amount);
    if (sscanf(input, "%lf", &amount) != 1) {
      printf("Invalid input. Please enter only numbers.\n");
      return 0.0;
    }

    if(amount < 0) {
        printf("Invalid amount. Please enter only positive numbers.\n");
        return 0.0;
    }
    else {
        printf("Successfully deposited $%.2lf\n", amount);
        return amount;
    }
}

double amount_to_deposit() {
    char buffer[50];
    clear_buffer();
    printf("Enter amount to deposit: $");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Failed to read input. Please enter only numbers.\n");
        return 0.0;
    }
    return validate_amount_to_deposit(buffer);
}

double validate_transaction(User *user, const char *action) {
    // double amount = 0.0;
    // clear_buffer();
    // printf("Enter amount to %s: $", action);
    // scanf("%lf", &amount);

    double amount = validate_amount_for_transaction(action);

    if (amount < 0) {
        printf("Invalid amount!\n");
        return 0.0;
    }

    if ((user->account == STANDARD || user->account == INITIAL_BALANCE) && amount > user->balance) {
        printf("Not enough money bro!\n");
        return 0.0;
    }

    if (user->account == OVERDRAFT_LIMIT && (user->balance - amount) < -1000) {
        printf("Overdraft limit exceeded! You can not go below -$1000.\n");
        return 0.0;
    }

    return -amount;
}

// ===== WITHDRAW
double amount_to_withdraw(User *user) {
    double amount = validate_transaction(user, "withdraw");
    if (amount == 0.0) return 0.0;

    printf("Successfully withdrew $%.2lf\n", -amount);
    return amount;
}

int withdraw_from_user(User *user, double amount) {
    if (amount < 0) {
        update_balance_in_csv(user, amount);
        check_customer_balance(user);
        return 1;
    }
    return 0;
}

// ===== TRANSFER


int validate_recipient(User *user, double amount) {
    //To whom?
    char recipient_name[100];
    char recipient_ssn[10];

    printf("Please enter the name of the account you want to send money to: ");

    if (fgets(recipient_name, sizeof(recipient_name), stdin) == NULL) {
        printf("Failed to read input. Please try again.\n");
        return 0;
    }

    size_t length = strlen(recipient_name);
    if (length > 0 && recipient_name[length - 1] == '\n') {
        recipient_name[length - 1] = '\0';
    }

    printf("Please enter the SSN of the account you want to send money to: ");
    scanf("%s", recipient_ssn);

    printf("Please enter account number of receiving account: ");
    int number_of_account = (get_int_value());

    FILE *input = fopen("../customers.csv", "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!input || !temp) {
        perror("Error opening file.");
        if (input) fclose(input);
        if (temp) fclose(temp);
        return 0;
    }

    char line[256];
    char name[100], ssn[20];
    int account;
    double balance;
    int account_number;

    int found = 0;
    while (fgets(line, sizeof(line), input)) {
        if (!parse_customer_line(line, name, ssn, &account, &balance, &account_number)) {
            continue;
        }

        if (strcmp(name, recipient_name) == 0 && strcmp(ssn, recipient_ssn) == 0 && number_of_account == account_number) {
            balance -= amount;
            user->balance = balance;
            found = 1;
        }

        fprintf(temp, "%s,%s,%d,%.2lf,%d\n", name, ssn, account, balance, account_number);
    }
    if (!found) {
        printf("Account does not exist!\n");
        return EXIT_FAILURE;
    }
    else {
        printf("Money has been transferred.\n");
        log_transaction(user->name, recipient_name, recipient_ssn, amount);
    }

    fclose(input);
    fclose(temp);

    remove("../customers.csv");
    rename("temp.csv", "../customers.csv");

    return 1;

}

double amount_to_transfer(User *user) {
    printf("For transfer first verify the amount you want to withdraw:\n");
    return validate_transaction(user, "transfer");
}