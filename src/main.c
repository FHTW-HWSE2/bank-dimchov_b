#include <stdio.h>
#include "../include/transaction.h"
// #include "../include/account.h"

void print_help() {
    printf("\n*** WELCOME TO THE BANK ***");
    printf("\nSelect an option: ");
    printf("\n 1.   Create an account");
    printf("\n 2.   Delete account");
    printf("\n 3.   Deposit money");
    printf("\n 4.   Withdraw moeny");
    printf("\n 5.   Transfer money");
    printf("\n 6.   Check balance");
    printf("\n 7.   Exit");
}

int main(int argc, char *argv[]) {

    int choice = 0; 
    double balance = 0;

do {
    print_help();
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch(choice){
    /*  case 1:
            create_account();
            break;
        case 2:
            delete_account();
            break; */
        case 3:
            balance += deposit();
            break;
        case 4:
            balance -= withdraw(balance);
            break;
    /*  case 5:
            transfer(balance);
            break; */
        case 6:
            report(balance);
            break;
        case 7: 
            printf("\nExiting... See you later!\n");
            break;
        default:
            printf("\nInvalid choice! Please select 1-7\n");
    }

} while(choice != 7);

    return 0;
}
