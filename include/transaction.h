#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "customer.h"
#include <string.h>

typedef struct {
    char date[20];
    char type[20];
    char account_name[200];
    double amount;
} Transaction;

double deposit();
double withdraw(User *user);
double amount_to_transfer(User *user);
int transfer(User *user, double amount);
void simulate_7_days(User *users, int user_count); //MS
void log_transaction(const Transaction *tx); //MS
void print_last_10_transactions(); //MS

#endif 