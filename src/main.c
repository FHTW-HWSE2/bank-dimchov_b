#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../include/customer.h"
#include "../include/account.h"
#include "../include/transaction.h"
#include "../include/main.h"

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

int get_input(int *choice) {
    return scanf("%d", choice);
}

int clear_stdin() {
    while (getchar() != '\n');
    return 0;
}

int get_valid_choice() {
    int choice = 0;
    while (1) {
        printf("\nEnter your choice: ");
        if (get_input(&choice) != 1) {
            clear_stdin();
            printf("Invalid input! Please enter a valid number.\n");
        } else if (choice < 1 || choice > 8) {
            printf("Invalid choice! Please select a number between 1 and 8.\n");
        } else {
            break;
        }
    }
    return choice;
}

int main(int argc, char *argv[]) {
    int choice = 0;
    User users;
    int user_count = 0; 

    srand(time(NULL)); 

    do {
        print_help();
        choice = get_valid_choice();

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
                printf("Exiting... Bye-byeee! Don't spend all your money at once! („• ֊ •„)੭\n");
                break;

            default: 
                printf("Invalid choice! Please select a number between 1 and 8.\n");
        }

    } while (choice != 8);

    return 0;
}