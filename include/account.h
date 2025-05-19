#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "types.h"

int get_int_value();
int login(User *user);
int create_account(User *user);
int delete_account(User *user);
int check_customer_balance(User *user);
int check_number_of_accounts(const char *name, const char *ssn);

#endif