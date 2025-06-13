#include <unity.h>
#include <types.h>
#include <stdio.h>
#include "customer.h"
#include "mock_main.h"

void setUp(void) {}
void tearDown(void) {
    remove("customers.csv");
    remove("temp.csv");
}

// ======= UNIT TEST =======
// ======= 01 save_account_to_csv()
// ======= 02 parse_customer_line()
// ======= 03 read_customer_data()
// ======= 04 update_balance_in_csv()
// ======= 05 report()
// ======= 06 execute_hardcoded_transactions()
// ======= 07 generate_transactions_report()

void mock_stdin(const char *input) {
    FILE *file = fopen("test/mock_input.txt", "w");
    fprintf(file, "%s\n", input);
    fclose(file);

    freopen("test/mock_input.txt", "r", stdin);
}

// =========================
// ======= 01 save_account_to_csv()
// =========================
void test_save_account_to_csv_SUCCESS(void) {
    TEST_IGNORE();
}

// =========================
// ======= 02 parse_customer_line()
// =========================
void test_parse_customer_line_SUCCESS(void) {
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

    int result = parse_customer_line(mock_csv_line, mock_name, mock_ssn, &mock_account_type, &mock_balance, &mock_account_number);
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_parse_customer_line_INCOMPLETE_CSV_LINE(void) {
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

    // account type is missing | value between ssn and balance
    char mock_incomplete_csv_line[256] = "Testily Toastily,238598764,100.0,1";

    int result = parse_customer_line(mock_incomplete_csv_line,mock_name, mock_ssn, &mock_account_type, &mock_balance, &mock_account_number);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_parse_customer_line_UNEXPECTED_EXTRA_COMMA_AT_THE_END (void) {
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

    // extra comma at the end
    char mock_extra_comma_end_in_csv_line[256] = "Testily Toastily,238598764,0,100.0,1,";
    int result = parse_customer_line(mock_extra_comma_end_in_csv_line,mock_name, mock_ssn, &mock_account_type, &mock_balance, &mock_account_number);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_parse_customer_line_UNEXPECTED_EXTRA_COMMA_IN_THE_MIDDLE (void) {
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

    // extra comma in the middle
    char mock_extra_comma_middle_in_csv_line[256] = "Testily Toastily,,238598764,0,100.0,1";
    int result = parse_customer_line(mock_extra_comma_middle_in_csv_line,mock_name, mock_ssn, &mock_account_type, &mock_balance, &mock_account_number);
    TEST_ASSERT_EQUAL_INT(0, result);
}

// =========================
// ======= 01 read_customer_data()
// =========================

void test_read_customer_data_SUCCESS(void) {
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

    FILE *file = fopen("../customers.csv", "w");
    fprintf(file, "Testily Toastily,238598764,0,100.0,1");
    fclose(file);

    char mock_csv_line[256] = "Testily Toastily,238598764,0,100.0,1";
    parse_customer_line(mock_csv_line,mock_name, mock_ssn, &mock_account_type, &mock_balance, &mock_account_number);
//    parse_customer_line_ExpectAndReturn(mock_csv_line, mock_name, mock_ssn, &mock_account_type, &mock_balance, &mock_account_number,0);

    int result = read_customer_data(&test_user, mock_name, mock_ssn, &mock_account_number);
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_read_customer_data_WRONG_NAME(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00,
        .account_number = 1
    };
    char *mock_name = "Tittily Toastily";
    char *mock_ssn = test_user.SSN;
    int mock_account_type = test_user.account;
    double mock_balance = test_user.balance;
    int mock_account_number = test_user.account_number;

    FILE *file = fopen("../customers.csv", "w");
    fprintf(file, "Testily Toastily,238598764,0,100.0,1");
    fclose(file);

    char mock_csv_line[256] = "Testily Toastily,238598764,0,100.0,1";

    int result = read_customer_data(&test_user, mock_name, mock_ssn, &mock_account_number);
    TEST_ASSERT_EQUAL_INT(0, result);
}




void setup_csv_with_user() {
    FILE *file = fopen("../customers.csv", "w");
    fprintf(file, "Max Mustermann,123456789,1,100.00,1\n");
    fclose(file);
}

void test_update_balance_in_csv_SUCCESS (void) {
    
    User user = {
        .name = "Max Mustermann",
        .SSN = "123456789",
        .account = 1,
        .balance = 100.0,
        .account_number = 1
    };
    setup_csv_with_user();
    int result = update_balance_in_csv(&user, 70.00);
    TEST_ASSERT_EQUAL_INT(1, result);
}

