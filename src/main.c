#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../include/transaction.h"
#include "../include/customer.h"
#include "../include/account.h"

void print_help() {
    printf("\n*** WELCOME TO THE BANK ***");
    printf("\nSelect an option: ");
    printf("\n 1.   Create an account");
    printf("\n 2.   Delete account");
    printf("\n 3.   Deposit money");
    printf("\n 4.   Withdraw money");
    printf("\n 5.   Transfer money");
    printf("\n 6.   Report");
    printf("\n 7.   Exit");
}

int main(int argc, char *argv[]) {
    int choice = 0; 
    User user;

    srand(time(NULL));

do {
    print_help();
    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    printf("\n");

    switch(choice){
        case 1:
            create_account(&user);
            break;
        case 2:
            if (login(&user)) {
                delete_account(&user);
            }
            break; 
        case 3:
            if (login(&user)) {
                double amount = deposit();
                update_balance_in_csv(&user, amount);
                check_customer_balance(&user);
            }
            break;
        case 4:
            if (login(&user)) {
                double amount = withdraw(&user);
                update_balance_in_csv(&user, amount);
                check_customer_balance(&user);
            }
            break;
    /*  case 5:
            if (login(&user)) {
                double amount = transfer(&user);
                update_balance_in_csv(&user, amount);
                check_customer_balance(&user);
            }
            break; */
        case 6:
            report(&user);
            break;
        case 7: 
            printf("Exiting... See you later!\n");
            break;
        default:
            printf("Invalid choice! Please select 1-7\n");
    }

} while(choice != 7);

    return 0;
}
