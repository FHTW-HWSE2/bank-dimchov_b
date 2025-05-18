#include <stdio.h>

double validate_amount_to_deposit(const char *input) {
    double amount;
    // scanf("%lf", &amount);
    if (sscanf(input, "%lf", &amount) != 1) {
      printf("Invalid input. Please enter only numbers.\n");
      return 0.0;
    }

    if(amount < 0) {
        printf("Invalid amount. Please enter only positive numbers.\n");
        return 0.0;
    }
    else {
        printf("Successfully deposited $%.2lf\n", amount);
        return amount;
    }
}

int clear_buffer(){
    while (getchar() != '\n' &&  getchar() != EOF);
    return 0;
}

double amount_to_deposit() {
    char buffer[50];
    clear_buffer();
    printf("Enter amount to deposit: $");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Failed to read input. Please enter only numbers.\n");
        return 0.0;
    }
    return validate_amount_to_deposit(buffer);
}