#include "../include/transaction.h"
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

    int found = 0;
    while (fgets(line, sizeof(line), input)) {
        if (!parse_customer_line(line, name, ssn, &account, &balance)) {
            continue;
        }

        if (strcmp(name, recipient_name) == 0 && strcmp(ssn, recipient_ssn) == 0) {
            balance -= amount;
            user->balance = balance;
            found = 1;
        }

        fprintf(temp, "%s,%s,%d,%.2lf\n", name, ssn, account, balance);
    }
    if (!found) {
        printf("Account does not exist!\n");
        return EXIT_FAILURE;
    }
    else {
        printf("Money has been transferred.\n");
    }

    fclose(input);
    fclose(temp);

    remove("../customers.csv");
    rename("temp.csv", "../customers.csv");

    return 1;

}
/**
 * Logs a single transaction to the persistent transaction history file.
 *
 * @param tx Pointer to the Transaction struct containing the transaction details.
 *
 * This function appends the transaction data to "transactions.dat" in binary mode.
 * It assumes the file is writable and handles closing the file after writing.
 */
void log_transaction(const Transaction *tx) {
    FILE *fp = fopen("transactions.dat", "ab"); // Open file in append-binary mode
    if (fp) {
        fwrite(tx, sizeof(Transaction), 1, fp);  // Write one transaction record
        fclose(fp);                              // Close the file to save changes
    }
}

/**
* Prints the last 10 recorded transactions from the transaction history file.
*
* This function reads all transactions from "transactions.dat", stores them in memory,
* and then prints the 10 most recent ones (or fewer if less than 10 exist).
* Each transaction includes the date, type, account name, and amount.
*/
void print_last_10_transactions() {
    Transaction buffer[1000]; // Temporary storage for transactions
    int count = 0;

    FILE *fp = fopen("transactions.dat", "rb"); // Open file in read-binary mode
    if (!fp) {
        printf("No transaction history found.\n");
        return;
    }

    // Read transactions into buffer
    while (fread(&buffer[count], sizeof(Transaction), 1, fp)) {
        count++;
        if (count >= 1000) break;
    }
    fclose(fp);

    // Determine where to start printing the last 10 entries
    int start = (count > 10) ? count - 10 : 0;
    printf("\n--- Last %d Transactions ---\n", (count < 10) ? count : 10);
    for (int i = start; i < count; i++) {
        printf("Date: %s | Type: %s | Account: %s | Amount: %.2f\n",
               buffer[i].date,
               buffer[i].type,
               buffer[i].account_name,
               buffer[i].amount);
    }
}