#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdlib.h>
#include "../include/customer.h"
#include "../include/account.h"
#include "../include/transaction.h"
#include "../include/main.h"

// Print the available menu options
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

// Wrapper for reading user input
int get_input(int *choice) {
    return scanf("%d", choice);  // Do NOT flush stdin here!
}

// Clear leftover characters from stdin
int clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);  // Make sure to check for EOF
    return 0;
}
/*
 * Updated main.c to improve user input handling.
 *
 * Previous implementation using scanf("%d", ...) caused the program to behave
 * incorrectly when users pressed Enter without entering a number — resulting in
 * an apparent freeze or invisible loop.
 *
 * Changes:
 * - I Replaced scanf() with fgets() + sscanf() in get_valid_choice() for safer, line-based input.
 * - Handles empty input (just Enter), non-numeric values (e.g., "abc"), and out-of-range numbers gracefully.
 * - Improved feedback to the user for invalid input without requiring stdin flushing hacks.
 *
 * These changes make the program more robust, especially in real-world usage where input errors are common.
 * By MS
 */

int get_valid_choice() {
    char input[100];   // Buffer to read full line including newline
    int choice = 0;

    while (1) {
        printf("\nEnter your choice: ");

        if (!fgets(input, sizeof(input), stdin)) {
            // If input is NULL (e.g., Ctrl+D), handle as exit
            printf("Input error.\n");
            continue;
        }

        // Remove trailing newline if present
        input[strcspn(input, "\n")] = 0;

        // Check if it's empty input
        if (strlen(input) == 0) {
            printf("No input provided. Please enter a number between 1 and 8.\n");
            continue;
        }

        // Try to parse the number
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input! Please enter a valid number.\n");
            continue;
        }

        if (choice < 1 || choice > 8) {
            printf("Invalid choice! Please select a number between 1 and 8.\n");
            continue;
        }

        break;  // Valid choice
    }

    return choice;
}

// Main program loop
int main(int argc, char *argv[]) {
    int choice = 0;
    User users;
    int user_count = 0;

    srand(time(NULL));

    do {
        print_help();
        choice = get_valid_choice();  // Get a valid menu option from user

        switch (choice) {
            case 1:
                create_account(&users);
                break;

            case 2:
                if (login(&users)) {
                    delete_account(&users);
                }
                break;

            case 3:
                if (login(&users)) {
                    double amount = deposit();
                    if (amount > 0) {
                        update_balance_in_csv(&users, amount);
                        check_customer_balance(&users);
                    }
                }
                break;

            case 4:
                if (login(&users)) {
                    double amount = withdraw(&users);
                    if (amount < 0) {
                        update_balance_in_csv(&users, amount);
                        check_customer_balance(&users);
                    }
                }
                break;

            case 5:
                if (login(&users)) {
                    check_customer_balance(&users);
                    double amount = amount_to_transfer(&users);
                    if (amount < 0) {
                        if (transfer(&users, amount)) {
                            update_balance_in_csv(&users, amount);
                        }
                    }
                }
                break;

            case 6:
                report(&users);
                break;

            case 7:
                simulate_7_days(&users, user_count);
                break;

            case 8:
                // Exit condition
                printf("Exiting... Bye-byeee! Don't spend all your money at once! („• ֊ •„)੭\n");
                break;

            default:
                printf("Invalid choice! Please select a number between 1 and 8.\n");
        }

    } while (choice != 8);  // Exit when user selects option 8

    return 0;
}