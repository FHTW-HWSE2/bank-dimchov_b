#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "types.h"
#include <string.h>

int deposit(User *users);
// double amount_to_deposit();
// double withdraw(User *user);
int withdraw(User *user);
//double amount_to_transfer(User *user);
void log_transaction(const char *sender_name, const char *recipient_name, const char *recipient_ssn, double amount);
int transfer(User *user);
// int transfer_(User *user, double amount);
// void simulate_7_days(User *users, int user_count);
int transfer_simulation();

#endif 