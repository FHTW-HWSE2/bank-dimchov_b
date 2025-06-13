#include <unity.h>
#include <types.h>
#include "mock_customer.h"
#include "mock_main.h"
#include "account.h"
#include "transaction_helper.h"

void setUp(void) {}
void tearDown(void) {}
/*
void test_create_account_SUCCES (void) {
    User user = {
        .name = "Alpaka King",
        .SSN = "17272792"
    };

    get_int_value_IgnoreAndReturn(1);
    login_IgnoreAndReturn(1);
    chooseAccountType_IgnoreAndReturn(0);
    check_number_of_accounts_IgnoreAndReturn(1);
    save_account_to_csv_Ignore();

    int result = create_account(&user);

    TEST_ASSERT_EQUAL_INT(0, result);
}
*/

void test_login_SUCCESS(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00,
        .account_number = 1
    };
    char *mock_name = "Testily Toastily";
//    mock_stdin("Testily Toastily");
//    strcpy(mock_name, "Testily Toastily");
//    mock_stdin("238598764");
    char *mock_ssn = "238598764";


    check_number_of_accounts("Testily Toastily", test_user.SSN);
    int mock_number_of_account = get_int_value();
    read_customer_data(&test_user, mock_name, mock_ssn, &mock_number_of_account);

    int result = login(&test_user);
    TEST_ASSERT_EQUAL_INT(1, result);
  }

void test_login_WRONG_SSN(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00,
        .account_number = 1
    };
    char *mock_name = "Testily Toastily";

    char *mock_ssn = "101505909";


    check_number_of_accounts("Testily Toastily", test_user.SSN);
    int mock_number_of_account = get_int_value();
    read_customer_data(&test_user, mock_name, mock_ssn, &mock_number_of_account);

    int result = login(&test_user);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_check_number_of_accounts_SUCCESS(void) {
    FILE *file = fopen("../customers.csv", "w");
    fprintf(file, "Testily Toastily,238598764,0,100.0,1");
    fprintf(file, "Testily Toastily,238598764,0,100.0,2");
    fprintf(file, "Testily Toastily,238598764,0,100.0,3");
    fclose(file);

    FILE *fileread = fopen("../customers.csv", "r");
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00,
        .account_number = 1
    };
    char *mock_name = test_user.name;
    char *mock_ssn = test_user.SSN;
    int mock_account_type = test_user.account;
    double mock_balance = test_user.balance;
    int mock_account_number = test_user.account_number;

    char mock_csv_line[256] = "Testily Toastily,238598764,0,100.0,1";
    parse_customer_line(mock_csv_line,mock_name, mock_ssn, &mock_account_type, &mock_balance, &mock_account_number);

    int result = check_number_of_accounts(test_user.name, test_user.SSN);
    TEST_ASSERT_EQUAL_INT(3, result);

    fclose(fileread);
}
