#include "../include/transaction.h"
#include "account.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double deposit() {
    double amount;
    printf("Enter amount to deposit: $");
    scanf("%lf", &amount);

    if(amount < 0) {
        printf("Invalid amount\n");
        return 0.0;
    }
    else {
        printf("Successfully deposited $%.2lf\n", amount);
        return amount;
    }
}

double validate_transaction(User *user, const char *action) {
    double amount = 0.0;
    printf("Enter amount to %s: $", action);
    scanf("%lf", &amount);

    if (amount < 0) {
        printf("Invalid amount!\n");
        return 0.0;
    }

    if ((user->account == STANDARD || user->account == INITIAL_BALANCE) && amount > user->balance) {
        printf("Not enough money bro!\n");
        return 0.0;
    }

    if (user->account == OVERDRAFT_LIMIT && (user->balance - amount) < -1000) {
        printf("Overdraft limit exceeded! You can not go below -$1000.\n");
        return 0.0;
    }

    return -amount;
}

double withdraw(User *user) {
    double amount = validate_transaction(user, "withdraw");
    if (amount == 0.0) return 0.0;

    printf("Successfully withdrew $%.2lf\n", -amount);
    return amount;
}

double amount_to_transfer(User *user) {
    printf("For transfer first verify the amount you want to withdraw:\n");
    return validate_transaction(user, "transfer");
}

void log_transaction(const char *sender_name, const char *recipient_name, const char *recipient_ssn, double amount) {
    FILE *log = fopen("../log.csv", "a");
    if (!log) {
        perror("Failed to open transaction log");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    fprintf(log, "%s,%s,%s,%s,%.2f\n", timestamp, sender_name, recipient_name, recipient_ssn, amount);

    fclose(log);
}


/**
 * @brief Transfers money from sender to receiver if possible.
 *
 * @param sender Pointer to sender User struct
 * @param recipient_name Receiver's name
 * @param recipient_ssn Receiver's SSN
 * @param recipient_account_number Receiver's account number
 * @param amount Amount to transfer
 * @return 1 if success, 0 if fail (e.g., account not found or insufficient balance)
 */
int transfer_internal(User *user, const char *recipient_name, const char *recipient_ssn, int number_of_account, double amount) {
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
    int account_number;

    int sender_found = 0;
    int recipient_found = 0;

    // Copy header
    if (fgets(line, sizeof(line), input)) {
        fprintf(temp, "%s", line);
    }

    while (fgets(line, sizeof(line), input)) {
        if (!parse_customer_line(line, name, ssn, &account, &balance, &account_number)) {
            continue;
        }

        // If recipient found -> add amount to balance
        if (strcmp(name, recipient_name) == 0 && strcmp(ssn, recipient_ssn) == 0 && account_number == number_of_account) {
            balance += amount;
            recipient_found = 1;
        }

        // If sender found -> subtract amount from balance
        if (strcmp(name, user->name) == 0 && strcmp(ssn, user->SSN) == 0 && account_number == user->account_number) {
            if (user->balance < amount) {
                printf("Transfer failed: Insufficient balance.\n");
                fclose(input);
                fclose(temp);
                remove("temp.csv");
                return 0;
            }
            balance -= amount;
            user->balance = balance;  // Update sender balance
            sender_found = 1;
        }

        // Write updated or unchanged line back
        fprintf(temp, "%s,%s,%d,%.2lf,%d\n", name, ssn, account, balance, account_number);
    }

    fclose(input);
    fclose(temp);

    if (!recipient_found || !sender_found) {
        printf("Account does not exist!\n");
        remove("temp.csv");
        return 0;
    }

    remove("../customers.csv");
    rename("temp.csv", "../customers.csv");

    printf("Money has been transferred.\n");
    return 1;
}

int transfer(User *user, double amount) {

    //To whom?
    char recipient_name[100];
    char recipient_ssn[10];
    printf("Please enter the name of the account you want to send money to: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    fgets(recipient_name, sizeof(recipient_name), stdin);

    size_t length = strlen(recipient_name);
    if (length > 0 && recipient_name[length - 1] == '\n') {
        recipient_name[length - 1] = '\0';
    }

    printf("Please enter the SSN of the account you want to send money to: ");
    scanf("%s", recipient_ssn);

    printf("Please enter account number of receiving account:\n");
    int number_of_account = (get_int_value());

    return transfer_internal(user, recipient_name, recipient_ssn, number_of_account, amount);
}


/**
 * @brief Simulates banking transactions over 7 days and generates a detailed report.
 *
 * This function simulates a total of 21 transactions (3 per day for 7 days) between predefined users.
 * For each transaction:
 * - Randomly selects a sender and a receiver (ensuring they are different).
 * - Generates a random transaction amount.
 * - Checks if the sender has sufficient balance to complete the transaction.
 * - If the sender has enough balance:
 *   - Deducts the amount from the sender's balance.
 *   - Adds the amount to the receiver's balance.
 *   - Marks the transaction as "Success".
 * - If not enough balance:
 *   - Transaction is marked as "Failed" with reason "Insufficient balance".
 *
 * For each transaction, the following information is recorded:
 * - Timestamp of the transaction.
 * - Sender name.
 * - Receiver name.
 * - Transaction amount.
 * - Status (Success/Failed).
 * - Reason (if failed).
 * - Sender's balance before and after the transaction.
 * - Receiver's balance before and after the transaction.
 *
 * After the simulation:
 * - Prints a table of all transactions to the console.
 * - Saves the transaction report to a CSV file named `csv_simulation.csv`.
 *
 * @note This version uses dummy user data. For real usage, will replace the hardcoded users
 * and balances with actual system user data.
 *
 * @file transfer_simulation.c
 * @author
 * @date 2025
 */


void transfer_simulation() {
    const int DAYS = 7;
    const int TRANSACTIONS_PER_DAY = 3;
    const int TOTAL_TRANSACTIONS = DAYS * TRANSACTIONS_PER_DAY;

    TransactionRecord transactions[TOTAL_TRANSACTIONS];

    // First: create initial CSV with hardcoded users
    FILE *file = fopen("../customers.csv", "w");
    if (!file) {
        perror("Error opening customers.csv");
        return;
    }
    fprintf(file, "Name,SSN,AccountType,Balance,AccountNumber\n");
    fprintf(file, "Alice Smith,123456789,0,1000.00,1111\n");
    fprintf(file, "Bob Johnson,987654321,0,1500.00,2222\n");
    fprintf(file, "John Doe,555555555,0,800.00,3333\n");
    fclose(file);

    // Create user array for mapping names and account info (used for lookups)
    User users[3] = {
        {"Alice Smith", "123456789", STANDARD, 0.0, 'A', 1111},
        {"Bob Johnson", "987654321", STANDARD, 0.0, 'B', 2222},
        {"John Doe", "555555555", STANDARD, 0.0, 'C', 3333}
    };

    srand(time(NULL));
    int transaction_count = 0;

    for (int day = 0; day < DAYS; day++) {
        for (int t = 0; t < TRANSACTIONS_PER_DAY; t++) {

            int sender_index = rand() % 3;
            int receiver_index;

            do {
                receiver_index = rand() % 3;
            } while (receiver_index == sender_index);

            double amount = rand() % 500 + 200;

            // Read current sender balance from CSV:
            FILE *csv = fopen("../customers.csv", "r");
            if (!csv) {
                perror("Error opening customers.csv");
                return;
            }

            char line[256];
            char name[100], ssn[20];
            int account_type;
            double balance;
            int acc_number;

            // Skip header
            fgets(line, sizeof(line), csv);

            while (fgets(line, sizeof(line), csv)) {
                if (!parse_customer_line(line, name, ssn, &account_type, &balance, &acc_number)) continue;

                if (strcmp(name, users[sender_index].name) == 0 &&
                    strcmp(ssn, users[sender_index].SSN) == 0 &&
                    acc_number == users[sender_index].account_number) {

                    users[sender_index].balance = balance;
                    break;
                }
            }
            fclose(csv);

            // Build timestamp
            time_t rawtime = time(NULL) + (day * 86400);
            struct tm *timeinfo = localtime(&rawtime);
            char timestamp[20];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

            // Build sender struct for transfer_internal()
            User sender;
            strcpy(sender.name, users[sender_index].name);
            strcpy(sender.SSN, users[sender_index].SSN);
            sender.account = users[sender_index].account;
            sender.balance = users[sender_index].balance;  // use real balance from CSV
            sender.bank = users[sender_index].bank;
            sender.account_number = users[sender_index].account_number;

            // Perform transfer using transfer_internal()
            int result = transfer_internal(&sender,
                                            users[receiver_index].name,
                                            users[receiver_index].SSN,
                                            users[receiver_index].account_number,
                                            amount);

            // Store result for reporting
            strcpy(transactions[transaction_count].timestamp, timestamp);
            strcpy(transactions[transaction_count].sender, users[sender_index].name);
            strcpy(transactions[transaction_count].receiver, users[receiver_index].name);
            transactions[transaction_count].amount = amount;

            if (result == 1) {
                strcpy(transactions[transaction_count].status, "Success");
                strcpy(transactions[transaction_count].reason, "");
            } else {
                strcpy(transactions[transaction_count].status, "Failed");
                strcpy(transactions[transaction_count].reason, "Insufficient balance");
            }

            transaction_count++;
        }
    }

    // Print simulation report
    printf("%-20s %-15s %-15s %-10s %-10s %-25s\n",
           "Timestamp", "Sender", "Receiver", "Amount", "Status", "Reason");

    for (int i = 0; i < TOTAL_TRANSACTIONS; i++) {
        printf("%-20s %-15s %-15s %-10.2f %-10s %-25s\n",
               transactions[i].timestamp,
               transactions[i].sender,
               transactions[i].receiver,
               transactions[i].amount,
               transactions[i].status,
               transactions[i].reason);
    }

    // Write simulation report to CSV
    FILE *report_csv = fopen("csv_simulation.csv", "w");
    if (!report_csv) {
        perror("Failed to open csv_simulation.csv");
        return;
    }

    fprintf(report_csv, "Timestamp,Sender,Receiver,Amount,Status,Reason\n");
    for (int i = 0; i < TOTAL_TRANSACTIONS; i++) {
        fprintf(report_csv, "%s,%s,%s,%.2f,%s,%s\n",
                transactions[i].timestamp,
                transactions[i].sender,
                transactions[i].receiver,
                transactions[i].amount,
                transactions[i].status,
                transactions[i].reason);
    }
    fclose(report_csv);
}
    /** timestamp for each transaction - with time
    // print report in the console for the whole processes (transactions)
    //and show it as a table in the terminal and write it in extra csv (csv_simulation.csv) and it should save it.
    //the report should have a timestamp and sender name to receiver name and the amount. check whether the transaction was sucessful. check the
    //whether they have money.
    //3 transactions per day and each transaction should look different sending direction.
    //even 3 person is even better

    **/

