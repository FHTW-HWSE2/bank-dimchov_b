#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../include/customer.h"
#include "../include/account.h"
#include "../include/transaction.h"

// Function to display the menu options
void print_help() {
    printf("\n*** WELCOME TO THE BANK ***");
    printf("\nSelect an option: ");
    printf("\n 1.   Create an account");
    printf("\n 2.   Delete account");
    printf("\n 3.   Deposit money");
    printf("\n 4.   Withdraw money");
    printf("\n 5.   Transfer money");
    printf("\n 6.   Report");
    printf("\n 7.   Simulate 7 days");
    printf("\n 8.   Exit");
    printf("\n");
}
/*
// Placeholder function for deposit
double deposit() {
    double amount;
    printf("Enter the amount to deposit (whole cents only): ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Deposit amount must be positive.\n");
        return 0;
    }

    return amount;
}

// Placeholder function for withdrawal
double withdraw(User *user) {
    double amount;
    printf("Enter the amount to withdraw: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("Withdrawal amount must be positive.\n");
        return 0;
    }

    if (user->balance - amount < -1000 && user->account == OVERDRAFT_LIMIT) {
        printf("Withdrawal exceeds overdraft limit.\n");
        return 0;
    } else if (user->balance - amount < 0 && user->account != OVERDRAFT_LIMIT) {
        printf("Insufficient funds.\n");
        return 0;
    }

    return -amount; // Return negative amount for withdrawal
}
*/
// Main function
int main(int argc, char *argv[]) {
    int choice = 0;
    User users[100]; // Array to store multiple users
    int user_count = 0; // Counter for the number of users

    srand(time(NULL)); // Seed the random number generator

    do {
        print_help();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
            case 1: // Create an account
                create_account(&users[user_count++]);
                break;

            case 2: // Delete an account
                if (login(&users[0])) {
                    delete_account(&users[0]);
                }
                break;

            case 3: // Deposit money
                if (login(&users[0])) {
                    double amount = deposit();
                    if (amount > 0) {
                        update_balance_in_csv(&users[0], amount);
                        check_customer_balance(&users[0]);
                    }
                }
                break;

            case 4: // Withdraw money
                if (login(&users[0])) {
                    double amount = withdraw(&users[0]);
                    if (amount < 0) { // Ensure it's a valid withdrawal
                        update_balance_in_csv(&users[0], amount);
                        check_customer_balance(&users[0]);
                    }
                }
                break;

            case 5: // Transfer money
                if (login(&users[0])) {
                    check_customer_balance(&users[0]);
                    double amount = amount_to_transfer(&users[0]);
                    if (amount < 0) {
                        if (transfer(&users[0], amount)) {
                            update_balance_in_csv(&users[0], amount);
                        }
                    }
                }
                break;

            case 6: // Generate a report
                report(&users[0]);
                break;

            case 7: // Simulate 7 days
                simulate_7_days(users, user_count);
                break;

            case 8: // Exit
                printf("Exiting... See you later!\n");
                break;

            default: // Invalid choice
                printf("Invalid choice! Please select a number between 1 and 8.\n");
        }

    } while (choice != 8);

    return 0;
}