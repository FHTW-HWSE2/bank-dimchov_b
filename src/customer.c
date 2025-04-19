#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/customer.h"


void save_account_to_csv(User *user, char *filename) {
    FILE *file = fopen(filename, "a");

    if(file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%s,%s,%d,%lf\n", user->name, user->SSN, user->account, user->balance); 
    fclose(file);
}

int parse_customer_line(char *line, char *name, char *ssn, int *account, double *balance) {
    char *token = strtok(line, ",");
    if (!token) return 0;
    strcpy(name, token);

    token = strtok(NULL, ",");
    if (!token) return 0;
    strcpy(ssn, token);

    token = strtok(NULL, ",");
    if (!token) return 0;
    *account = atoi(token);

    token = strtok(NULL, ",");
    if (!token) return 0;
    *balance = atof(token);

    return 1;
}

int read_customer_data(User *user, const char *name, const char *ssn) {
    char line[256];
    char file_name[200], file_ssn[9];
    int file_account;
    double file_balance;

    FILE *file = fopen("../customers.csv", "r");
    if (!file) {
        perror("Could not open customers file");
        return 0;
    }
    while (fgets(line, sizeof(line), file)) {
      
        if (!parse_customer_line(line, file_name, file_ssn, &file_account, &file_balance)) {
            continue;  
        }

            if (strcmp(file_name, name) == 0 && strcmp(file_ssn, ssn) == 0) {
                strcpy(user->name, file_name);
                strcpy(user->SSN, file_ssn);

                switch (file_account) {
                    case 0:
                        user->account = STANDARD;
                        break;
                    case 1:
                        user->account = INITIAL_BALANCE;
                        break;
                    case 2:
                        user->account = OVERDRAFT_LIMIT;
                        break;
                    default:
                        user->account = STANDARD; 
                        break;
                }

                user->balance = file_balance;

                fclose(file);
                return 1; 
            }
    }
    fclose(file);
    return 0; 
}

int update_balance_in_csv(User *user, double new_amount) {
    FILE *input = fopen("../customers.csv", "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!input || !temp) {
        perror("Error opening file.");
        if (input) fclose(input);
        if (temp) fclose(temp);
        return 0;
    }

    char line[256];
    char name[100], ssn[20];
    int account;
    double balance;

    while (fgets(line, sizeof(line), input)) {
        if (!parse_customer_line(line, name, ssn, &account, &balance)) {
            continue;
        }

        if (strcmp(name, user->name) == 0 && strcmp(ssn, user->SSN) == 0) {
            balance += new_amount;
            user->balance = balance; 
        }

        fprintf(temp, "%s,%s,%d,%.2lf\n", name, ssn, account, balance);
    }

    fclose(input);
    fclose(temp);

    remove("../customers.csv");
    rename("temp.csv", "../customers.csv");
    
    return 1;
}

int count_total_accounts() {
    FILE *file = fopen("../customers.csv", "r");
    
    if (!file) {
        perror("Error opening file.");
        return 0;
    }

    char line[256];
    int count;

    while (fgets(line, sizeof(line), file)) {
            count++;
    }
    fclose(file);
    return count;
}

int last_ten_transactions() {
    return 1;
}

void report(User *user) {
    printf("=== Customer Report ===\n");
    int total = count_total_accounts();
    printf("Total accounts in system: %d\n", total);
}