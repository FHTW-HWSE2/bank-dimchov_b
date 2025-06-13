#ifndef ACCOUNT_HELPER_H
#define ACCOUNT_HELPER_H

int get_int_value(void);
int login(User *user);
int chooseAccountType(User *user);
int check_number_of_accounts(const char *name, const char *SSN);
void save_account_to_csv(User *user, char *filename);

#endif // ACCOUNT_HELPER_H