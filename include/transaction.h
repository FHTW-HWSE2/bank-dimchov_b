#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "customer.h"
#include <string.h>

int deposit(User *users);
// double amount_to_deposit();
// double withdraw(User *user);
int withdraw(User *user);
double amount_to_transfer(User *user);
int transfer(User *user, double amount);
// void simulate_7_days(User *users, int user_count);

#endif 