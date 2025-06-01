#include <ctype.h>

#include "../include/account.h"
#include "../include/customer.h"
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>

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

    double amount = parse_amount(buffer);
    return amount;
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

// ===== DEPOSIT
double amount_to_deposit() {
    double amount = validate_amount_for_transaction("deposit");

    if(amount < 0) {
        printf("Invalid amount. Please enter only positive numbers.\n");
        return 0.0;
    }
    else if (amount == 0.0) {
        printf("Error: $%.2lf money was deposited.\n", amount);
        return 0.0;
    } else {
        printf("Successfully deposited $%.2lf\n", amount);
        return amount;
    }
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