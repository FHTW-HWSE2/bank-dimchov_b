#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "customer.h"

void simulate_7_days(User *users, int user_count);
double deposit();
double withdraw(User *user);
double transfer(User *user);

#endif 