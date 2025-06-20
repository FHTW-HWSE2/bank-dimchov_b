#ifndef TRANSACTIONS_HELPER_H
#define TRANSACTIONS_HELPER_H

#include "types.h"

int clear_buffer();
double parse_amount(const char *input);
double validate_amount_for_transaction(const char *action);
double validate_transaction(User *user, const char *action);

// double validate_amount_to_deposit(const char *input);
double amount_to_deposit();
double validate_amount_to_deposit(const char *input);
int clear_buffer();

double amount_to_withdraw(User *user);
int withdraw_from_user(User *user, double amount);

int validate_recipient(User *user, double amount);
double amount_to_transfer(User *user);

#endif //TRANSACTIONS_HELPER_H
