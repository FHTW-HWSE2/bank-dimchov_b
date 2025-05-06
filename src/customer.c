#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/customer.h"
#include "../include/transaction.h"

// Global variable to store the current simulated date
time_t simulated_date;

Transaction transactions[100];
int transaction_count = 0;

void save_account_to_csv(User *user, char *filename) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%s,%s,%d,%.2lf\n", user->name, user->SSN, user->account, user->balance);
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
    char file_name[200], file_ssn[20];
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
            user->account = (Account_type)file_account;
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
    char name[200], ssn[20];
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

    FILE *file = fopen("../customers.csv", "r");
    if (!file) {
        perror("Could not open customers file");
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        if (!parse_customer_line(line, file_name, file_ssn, &file_account, &file_balance)) {
            
            continue;
        }
        total_balance += file_balance;
    }

    fclose(file);
    return total_balance;
}

void report(User *user) {
    
    printf("\n===== CUSTOMER REPORT (^‿^) =====\n");
    
    int total = count_total_accounts();
    printf("Total accounts in system: %d\n", total);

    double total_balance = total_money_in_bank();
    printf("Total money held in bank: %.2lf$\n", total_balance);
}

// Function to simulate advancing the date by 7 days
void simulate_7_days(User *users, int user_count) {
    // Initialize the simulated date to the current date if not already set
    if (simulated_date == 0) {
        simulated_date = time(NULL);
    }

    // Advance the date by 7 days
    simulated_date += 7 * 24 * 60 * 60;

    printf("Simulated date advanced by 7 days.\n");

    // Execute hardcoded transactions
    execute_hardcoded_transactions(users, user_count);

    // Generate a report of all transactions
    generate_transaction_report();
}

// Function to execute hardcoded transactions
void execute_hardcoded_transactions(User *users, int user_count) {
    // Example hardcoded transactions
    for (int i = 0; i < user_count; i++) {
        if (users[i].account == STANDARD) {
            // Deposit $100 into standard accounts
            users[i].balance += 100;

            // Log the transaction
            Transaction t;
            strftime(t.date, sizeof(t.date), "%Y-%m-%d", localtime(&simulated_date));
            strcpy(t.type, "Deposit");
            strcpy(t.account_name, users[i].name);
            t.amount = 100;
            transactions[transaction_count++] = t;
        } else if (users[i].account == OVERDRAFT_LIMIT) {
            // Withdraw $50 from overdraft accounts
            users[i].balance -= 50;

            // Log the transaction
            Transaction t;
            strftime(t.date, sizeof(t.date), "%Y-%m-%d", localtime(&simulated_date));
            strcpy(t.type, "Withdrawal");
            strcpy(t.account_name, users[i].name);
            t.amount = -50;
            transactions[transaction_count++] = t;
        }
    }

    printf("Hardcoded transactions executed.\n");
}

// Function to generate a report of all transactions
void generate_transaction_report() {
    printf("\n=== Transaction Report ===\n");
    printf("Date       | Type       | Account Name       | Amount\n");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < transaction_count; i++) {
        printf("%-10s | %-10s | %-18s | $%.2lf\n",
               transactions[i].date,
               transactions[i].type,
               transactions[i].account_name,
               transactions[i].amount);
    }

    printf("-----------------------------------------------------\n");
}