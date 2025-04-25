#include "../include/transaction.h"
#include <stdio.h>
#include <stdlib.h>

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
    else if(user->account == INITIAL_BALANCE && amount > user->balance) {
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

double amount_to_transfer(User *user) {
    printf("For transfer first verify the amount you want to withdraw: \n");

    double amount = 0.0;
    printf("Enter amount to transfer: $");
    scanf("%lf", &amount);
    if(amount < 0) {
        printf("Invalid amount!\n");
        return 0.0;
    }
    else if(user->account == STANDARD && amount > user->balance) {
        printf("Not enough money bro!\n");
        return 0.0;
    }
    else if(user->account == INITIAL_BALANCE && amount > user->balance) {
        printf("Not enough money bro!");
        return 0.0;
    }
    else if(user->account == OVERDRAFT_LIMIT && (user->balance - amount) < -1000) {
        printf("Overdraft limit exceeded! You can not go below -$1000.\n");
        return 0.0;
    }
    else {
        return -amount;
    }

}

int transfer(User *user, double amount) {

    //To whom?
    char recipient_name[100];
    printf("Please enter the name of the account you want to send money to: ");
    scanf("%s", recipient_name);

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

    int found = 0;
    while (fgets(line, sizeof(line), input)) {
        if (!parse_customer_line(line, name, ssn, &account, &balance)) {
            continue;
        }

        if (strcmp(name, recipient_name) == 0) {
            balance -= amount;
            user->balance = balance;
            found = 1;
        }

        fprintf(temp, "%s,%s,%d,%.2lf\n", name, ssn, account, balance);
    }
    if (!found) {
        printf("Account does not exist!\n");
        return EXIT_FAILURE;
    }
    else {
        printf("Money has been transferred.\n");
    }

    fclose(input);
    fclose(temp);

    remove("../customers.csv");
    rename("temp.csv", "../customers.csv");

    return 1;

}