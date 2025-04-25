#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "customer.h"
#include <string.h>


void simulate_7_days(User *users, int user_count);
double deposit();
double withdraw(User *user);
int transfer(User *user, double amount);
double amount_to_transfer(User *user);
#endif 