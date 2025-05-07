#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "customer.h"

int login(User *user);
int create_account(User *user);
int delete_account(User *user);
int check_customer_balance(User *user) ;

#endif