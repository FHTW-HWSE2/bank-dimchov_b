#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/customer.h"
#include "../include/main.h"

void save_account_to_csv(User *user, char *filename) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%s,%s,%d,%.2lf,%d\n", user->name, user->SSN, user->account, user->balance, user->account_number);
    fclose(file);
}

int parse_customer_line(char *line, char *name, char *ssn, int *account, double *balance, int *account_number) {
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

    token = strtok(NULL, ",");
    if (!token) return 0;
    *account_number = atoi(token);

    return 1;
}

int read_customer_data(User *user, const char *name, const char *ssn, const int *account_number) {
    char line[256];
    char file_name[200], file_ssn[20];
    int file_account;
    double file_balance;
    int file_account_number;

    FILE *file = fopen("../customers.csv", "r");
    if (!file) {
        perror("Could not open customers file");
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        if (!parse_customer_line(line, file_name, file_ssn, &file_account, &file_balance, &file_account_number)) {
            continue;
        }
        if (strcmp(file_name, name) == 0 && strcmp(file_ssn, ssn) == 0 && file_account_number == *account_number) {
            strcpy(user->name, file_name);
            strcpy(user->SSN, file_ssn);
            user->account = (Account_type)file_account;
            user->balance = file_balance;
            user->account_number = file_account_number;

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
    csv_header(temp);

    char line[256];
    char name[200], ssn[20];
    int account;
    double balance;
    int account_number;

    fgets(line, sizeof(line), input);

    while (fgets(line, sizeof(line), input)) {
        if (!parse_customer_line(line, name, ssn, &account, &balance, &account_number)) {
            continue;
        }

        if (strcmp(name, user->name) == 0 && strcmp(ssn, user->SSN) == 0 && account_number == user->account_number) {
            balance += new_amount;
            user->balance = balance;
        }

        fprintf(temp, "%s,%s,%d,%.2lf,%d\n", name, ssn, account, balance, account_number);
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
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        count++;
    }

    fclose(file);
    return count;
}

double total_money_in_bank() {
    char line[256];
    char file_name[200], file_ssn[20];
    int file_account;
    double file_balance;
    double total_balance = 0.0;
    int account_number;

    FILE *file = fopen("../customers.csv", "r");
    if (!file) {
        perror("Could not open customers file");
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        if (!parse_customer_line(line, file_name, file_ssn, &file_account, &file_balance, &account_number)) {
            
            continue;
        }
        total_balance += file_balance;
    }

    fclose(file);
    return total_balance;
}

void write_last_10_transactions() {
    FILE *file = fopen("../log.csv", "r");
    if (!file) {
        perror("Could not open log file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long position = ftell(file);   // Get the current position of the file pointer (this is the size of the file in bytes)
    int line_count = 0;

    while(position > 0 && line_count <= 10) {
        fseek(file, --position, SEEK_SET);  // Decrement the position by 1 byte and move the file pointer to the new position
        char ch = fgetc(file);

        if (ch == '\n') {
            line_count++;  
        }
    }
}

void print_last_10_transaction() {
    FILE *file = fopen("../log.csv", "r");
    if (!file) {
        perror("Could not open file");
        return;
    }

    char line[256];
    int line_count = 0;

    while (fgets(line, sizeof(line), file) && line_count < 10) {
        printf("%s", line);
        line_count++;
    }

    fclose(file);
}

void report(User *user) {
    
    printf("\n===== CUSTOMER REPORT (^‿^) =====\n");
    
    int total = count_total_accounts();
    printf("Total accounts in system: %d\n", total);

    double total_balance = total_money_in_bank();
    printf("\nTotal money held in bank: %.2lf$\n", total_balance);

    printf("\n~~~ Last 10 transactions ~~~\n");
    print_last_10_transaction();
}