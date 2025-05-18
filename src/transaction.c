#include "../include/transaction_helper.h"
#include "../include/transaction.h"
#include "../include/account.h"
#include "../include/customer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int deposit(User *users) {
    double amount = amount_to_deposit();
    if (amount > 0) {
        update_balance_in_csv(users, amount);
        check_customer_balance(users);
        return 1;
    }
    return 0;
}



int withdraw(User *user) {
    double amount = amount_to_withdraw(user);
    if (amount < 0) {
        update_balance_in_csv(user, amount);
        check_customer_balance(user);
    }
    return 0;
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

int transfer_internal(User *user, const char *recipient_name, const char *recipient_ssn, int number_of_account, double amount) {
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

    printf("Please enter account number of receiving account:\n");
    int number_of_account = (get_int_value());

    return transfer_internal(user, recipient_name, recipient_ssn, number_of_account, amount);
}

int transfer_simulation() {
    User user = {
        .name = "Bubble Bee",
        .balance = 5000.0
    };

    const char *recipient_name = "Chicken Nugget";
    const char *recipient_ssn = "123456789";
    int account_number = 1;
    double amount = 250.0;

    printf("%s transferred $%.2f to %s\n", user.name, amount, recipient_name);
    double daily_amounts[7] = {250.0, 300.0, 150.0, 400.0, 500.0, 100.0, 200.0};

    printf("Starting 7 day transfer simulation:\n");
    for (int day = 1; day <= 7; day++) {
        double amount = daily_amounts[day - 1];

        printf("\nDay %d: %s transfers $%.2f to %s\n",
               day, user.name, amount, recipient_name);

        int result = transfer_internal(&user, recipient_name, recipient_ssn, account_number, amount);

        if (!result) {
            printf("Transfer failed on day %d\n", day);
            return 0;
        }
        printf("Balance from %s after day %d transfer: $%.2f\n", user.name, day, user.balance);
    }
    printf("\n7-day simulation completed. Final balance: $%.2f\n", user.balance);

    return 1;

}