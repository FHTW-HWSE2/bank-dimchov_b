#ifndef USER_H
#define USER_H

typedef enum {
    STANDARD = 0,     
    INITIAL_BALANCE,  
    OVERDRAFT_LIMIT   
} Accounttype;

typedef struct {
    char name[200]; 
    char SSN[20]; 
    Accounttype account; 
    double balance;
    char bank;
} User;

void report(User *user);
void save_account_to_csv(User *user, char *filename);
int update_balance_in_csv(User *user, double new_amount);
int read_customer_data(User *user, const char *name, const char *ssn);
int parse_customer_line(char *line, char *name, char *ssn, int *account, double *balance);

#endif