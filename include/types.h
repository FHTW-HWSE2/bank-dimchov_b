#ifndef TYPES_H
#define TYPES_H

typedef enum {
    STANDARD = 0,
    INITIAL_BALANCE,
    OVERDRAFT_LIMIT
} Account_type;

typedef struct {
    char name[200];
    char SSN[20];
    Account_type account;
    double balance;
    char bank;
    int account_number;
} User;

typedef struct {
    char date[20];
    char type[20];
    char account_name[200];
    double amount;
} Transaction;

#endif //TYPES_H
