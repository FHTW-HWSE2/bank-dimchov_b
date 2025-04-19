#include "../include/transaction.h"
#include <stdio.h>

double deposit() {
    double amount;
    printf("Enter amount to deposit: $");
    scanf("%lf", &amount);

    if(amount < 0) {
        printf("Invalid amount\n");
        return 0.0;
    }
    else {
        printf("Successfully deposited $%.2lf\n", amount);
        return amount;
    }
}

double withdraw(User *user) {
    double amount = 0.0;
    printf("Enter amount to withdraw: $");
    scanf("%lf", &amount);
    if(amount < 0) {
        printf("Invalid amount!\n");
        return 0.0;
    }
    else if(user->account == STANDARD && amount > user->balance) {
        printf("Not enough money bro!");
        return 0.0;
    }
    else if(user->account == OVERDRAFT_LIMIT && (user->balance - amount) < -1000) {
        printf("Overdraft limit exceeded! You can not go below -$1000.\n");
        return 0.0;
    }
    else {
        printf("Successfully withdrew $%.2lf\n", amount);
        return -amount;
    }
}

