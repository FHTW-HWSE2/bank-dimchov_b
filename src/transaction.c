#include "../include/transaction.h"
#include <stdio.h>

double deposit() {
    double amount = 0.0;
    printf("\nEnter amount to deposit: $");
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

void report(double balance) {
    printf("\nYour current balance is: $%.2lf", balance);
}

double withdraw(double balance) {
    double amount = 0.0;
    printf("\nEnter amount to withdraw: $");
    scanf("%lf", &amount);
    if(amount < 0) {
        printf("Invalid amount!\n");
        return 0.0;
    }
    else if(amount > balance) {
        printf("Not enough money bro! Your balance is $%.2lf", balance);
        return 0.0;
    }
    else {
        printf("Successfully withdrew $%.2lf", amount);
        return amount;
    }
}