#ifndef TRANSACTIONS_HELPER_H
#define TRANSACTIONS_HELPER_H

#include "types.h"

double validate_amount_to_deposit(const char *input);
int clear_buffer();
double amount_to_deposit();

double validate_transaction(User *user, const char *action);
double amount_to_withdraw(User *user);
int withdraw_from_user(User *user, double amount);

#endif //TRANSACTIONS_HELPER_H
