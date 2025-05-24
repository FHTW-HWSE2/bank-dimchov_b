#include "../include/account.h"
#include "../include/customer.h"
#include "../include/types.h"
#include <stdio.h>


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

int clear_buffer(){
  while (getchar() != '\n' &&  getchar() != EOF);
  return 0;
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

// ===== WITHDRAW
double validate_transaction(User *user, const char *action) {
    double amount = 0.0;
    printf("Enter amount to %s: $", action);
    scanf("%lf", &amount);

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