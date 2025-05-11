#include "../include/transaction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

double withdraw(User *user) {
    double amount = validate_transaction(user, "withdraw");
    if (amount == 0.0) return 0.0;

    printf("Successfully withdrew $%.2lf\n", -amount);
    return amount;
}

double amount_to_transfer(User *user) {
    printf("For transfer first verify the amount you want to withdraw:\n");
    return validate_transaction(user, "transfer");
}

void log_transaction(const char *sender_name, const char *recipient_name, const char *recipient_ssn, double amount) {
    FILE *log = fopen("../log.csv", "a");
    if (!log) {
        perror("Failed to open transaction log");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    fprintf(log, "%s,%s,%s,%s,%.2f\n", timestamp, sender_name, recipient_name, recipient_ssn, amount);

    fclose(log);
}

int transfer(User *user, double amount) {

    //To whom?
    char recipient_name[100];
    char recipient_ssn[10];
    printf("Please enter the name of the account you want to send money to: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    fgets(recipient_name, sizeof(recipient_name), stdin);

    size_t length = strlen(recipient_name);
    if (length > 0 && recipient_name[length - 1] == '\n') {
        recipient_name[length - 1] = '\0';
    }

    printf("Please enter the SSN of the account you want to send money to: ");
    scanf("%s", recipient_ssn);

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

        if (strcmp(name, recipient_name) == 0 && strcmp(ssn, recipient_ssn) == 0) {
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
        log_transaction(user->name, recipient_name, recipient_ssn, amount);
    }

    fclose(input);
    fclose(temp);

    remove("../customers.csv");
    rename("temp.csv", "../customers.csv");

    return 1;

}