//#include "../include/transaction.h"
#include "../include/transaction_helper.h"
#include "../include/customer.h"
#include "../include/account.h"
#include "../include/types.h"
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
    return withdraw_from_user(user, amount);
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

// char *validate_recipient_name(const char *user) {}
// char *validate_recipient_ssn(const char *user) {}

int transfer(User *users) {
    check_customer_balance(users);
    double amount = amount_to_transfer(users);
    if (amount < 0) {
        if (validate_recipient(users, amount)) {
            update_balance_in_csv(users, amount);
            return 0;
        }
    }
    return 1;
}