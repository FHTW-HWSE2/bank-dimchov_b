#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <time.h>

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
} User;

void report(User *user);
void save_account_to_csv(User *user, char *filename);
int update_balance_in_csv(User *user, double new_amount);
int read_customer_data(User *user, const char *name, const char *ssn);
int parse_customer_line(char *line, char *name, char *ssn, int *account, double *balance);
void simulate_7_days(User *users, int user_count);
void execute_hardcoded_transactions(User *users, int user_count);
void generate_transaction_report();  // Add this declaration

#endif