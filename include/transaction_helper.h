#ifndef TRANSACTIONS_HELPER_H
#define TRANSACTIONS_HELPER_H

double amount_to_deposit();
double validate_amount_to_deposit(const char *input);
int clear_buffer();

double validate_transaction(User *user, const char *action);
double amount_to_withdraw(User *user);
int withdraw_from_user(User *user, double amount);

#endif //TRANSACTIONS_HELPER_H
