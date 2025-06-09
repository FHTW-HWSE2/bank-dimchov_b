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
int transfer_internal(User *sender, const char *recipient_name, const char *recipient_ssn, int recipient_account_number, double amount) {
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

    int found = 0;

    // Skip and copy header
    if (fgets(line, sizeof(line), input)) {
        fprintf(temp, "%s", line);
    }

    while (fgets(line, sizeof(line), input)) {
        if (sscanf(line, "%99[^,],%19[^,],%d,%lf,%d", name, ssn, &account, &balance, &account_number) != 5) {
            continue;
        }

        if (strcmp(name, recipient_name) == 0 && strcmp(ssn, recipient_ssn) == 0 && recipient_account_number == account_number) {
            // Check if sender has enough balance before transferring
            if (sender->balance < amount) {
                printf("Transfer failed: Insufficient balance.\n");
                fclose(input);
                fclose(temp);
                remove("temp.csv"); // Clean up
                return 0; // Not enough balance
            }

            // Proceed with transfer
            sender->balance -= amount;
            balance += amount;
            found = 1;
        }

        // Write to temp file (updated balance if it's the recipient)
        fprintf(temp, "%s,%s,%d,%.2lf,%d\n", name, ssn, account, balance, account_number);
    }

    fclose(input);
    fclose(temp);

    if (!found) {
        printf("Account does not exist!\n");
        remove("temp.csv");
        return 0;
    } else {
        // Replace old customers.csv with updated temp.csv
        remove("../customers.csv");
        rename("temp.csv", "../customers.csv");

        // Log the transaction
        log_transaction(sender->name, recipient_name, recipient_ssn, amount);
        printf("transaction completed successfully.\n");

        return 1;
    }
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

    // Create real User structs (in real case: load from file or initialize properly)
    User users[3] = {
        {"Alice Smith", "123456789", STANDARD, 1000.0, 'A', 1111},
        {"Bob Johnson", "987654321", STANDARD, 1500.0, 'B', 2222},
        {"John Doe", "555555555", STANDARD, 800.0, 'C', 3333}
    };

    // 👉 Before anything: write users into customers.csv
    FILE *file = fopen("../customers.csv", "w");  // "w" to overwrite for clean start
    if (file != NULL) {
        // Write header if needed
        fprintf(file, "Name,SSN,AccountType,Balance,AccountNumber\n");
        for (int i = 0; i < 3; i++) {
            fprintf(file, "%s,%s,%d,%.2lf,%d\n",
                    users[i].name, users[i].SSN, users[i].account, users[i].balance, users[i].account_number);
        }
        fclose(file);
    } else {
        printf("Could not open customers.csv to write initial users.\n");
        return;
    }

    srand(time(NULL));
    int transaction_count = 0;

    for (int day = 0; day < DAYS; day++) {
        for (int t = 0; t < TRANSACTIONS_PER_DAY; t++) {
            int num_users = 3;
            int sender_index = rand() % num_users;
            int receiver_index;

            do {
                receiver_index = rand() % num_users;
            } while (receiver_index == sender_index);

            double amount = rand() % 500 + 200;  // Random 200-700

            time_t rawtime;
            struct tm *timeinfo;
            time(&rawtime);
            rawtime += day * 86400;
            timeinfo = localtime(&rawtime);

            char timestamp[20];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

            // Save balances BEFORE transaction
            transactions[transaction_count].sender_balance_before = users[sender_index].balance;
            transactions[transaction_count].receiver_balance_before = users[receiver_index].balance;

            // Perform the transfer using group function
            int result = transfer_internal(&users[sender_index], users[receiver_index].name, users[receiver_index].SSN, users[receiver_index].account_number, amount);

            // Save transaction data
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

            // Save balances AFTER transaction
            transactions[transaction_count].sender_balance_after = users[sender_index].balance;
            transactions[transaction_count].receiver_balance_after = users[receiver_index].balance;

            transaction_count++;
        }
    }

    // Print the report
    printf("%-20s %-15s %-15s %-10s %-10s %-25s %-20s %-20s %-20s %-20s\n",
           "Timestamp", "Sender", "Receiver", "Amount", "Status", "Reason",
           "Sender Bal Before", "Sender Bal After", "Receiver Bal Before", "Receiver Bal After");

    for (int i = 0; i < TOTAL_TRANSACTIONS; i++) {
        printf("%-20s %-15s %-15s %-10.2f %-10s %-25s %-20.2f %-20.2f %-20.2f %-20.2f\n",
               transactions[i].timestamp,
               transactions[i].sender,
               transactions[i].receiver,
               transactions[i].amount,
               transactions[i].status,
               transactions[i].reason,
               transactions[i].sender_balance_before,
               transactions[i].sender_balance_after,
               transactions[i].receiver_balance_before,
               transactions[i].receiver_balance_after);
    }

    // Write to CSV

    // WARNING: This simulation clears the customers.csv file!
    // Do NOT use this in production environments.
    FILE *csv = fopen("csv_simulation.csv", "w");
    if (csv == NULL) {
        perror("Failed to open csv_simulation.csv");
        return;
    }

    fprintf(csv, "Timestamp,Sender,Receiver,Amount,Status,Reason,SenderBalanceBefore,SenderBalanceAfter,ReceiverBalanceBefore,ReceiverBalanceAfter\n");
    for (int i = 0; i < TOTAL_TRANSACTIONS; i++) {
        fprintf(csv, "%s,%s,%s,%.2f,%s,%s,%.2f,%.2f,%.2f,%.2f\n",
                transactions[i].timestamp,
                transactions[i].sender,
                transactions[i].receiver,
                transactions[i].amount,
                transactions[i].status,
                transactions[i].reason,
                transactions[i].sender_balance_before,
                transactions[i].sender_balance_after,
                transactions[i].receiver_balance_before,
                transactions[i].receiver_balance_after);
    }

    fclose(csv);
}
    /** timestamp for each transaction - with time
    // print report in the console for the whole processes (transactions)
    //and show it as a table in the terminal and write it in extra csv (csv_simulation.csv) and it should save it.
    //the report should have a timestamp and sender name to receiver name and the amount. check whether the transaction was sucessful. check the
    //whether they have money.
    //3 transactions per day and each transaction should look different sending direction.
    //even 3 person is even better!!

    **/

