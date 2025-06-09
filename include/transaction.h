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


typedef struct {
    char timestamp[20];
    char sender[100];
    char receiver[100];
    double amount;
    char status[10]; // "Success" or "Failed"
    char reason[50]; // Reason for failure or empty if success
    double sender_balance_before;
    double sender_balance_after;
    double receiver_balance_before;
    double receiver_balance_after;
} TransactionRecord;

double deposit();
double withdraw(User *user);
double amount_to_transfer(User *user);
int transfer(User *user, double amount);

void transfer_simulation();

#endif 
