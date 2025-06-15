#include <stdio.h>
#include <unity.h>
#include <types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "customer.h"
#include "mock_main.h"
// #include "main.h"
#include "mock_transaction.h"
//#include "mock_customer.h"

void setUp(void) {}
void teardown(void) {
    remove("test/mock_input.txt");
    remove("../customers.csv");
    remove("customers.csv");
    remove("temp.csv");
    freopen("CON", "r", stdin);
    // freopen("/dev/tty", "r", stdin); // Linux, Mac
}

// ======= UNIT TEST =======
// ======= 01 save_account_to_csv()
// ======= 02 parse_customer_line()
// ======= 03 read_customer_data()
// ======= 04 update_balance_in_csv()
// ======= 05 count_total_accounts()
// ======= 06 report()
// ======= 07 total_money_in_bank()

void mock_stdin(const char *input) {
    FILE *file = fopen("test/mock_input.txt", "w");
    fprintf(file, "%s\n", input);
    fclose(file);

    freopen("test/mock_input.txt", "r", stdin);
}

// =========================
// ======= 00 mock dependecies
// =========================
void mock_stdin(const char *input) {
    FILE *file = fopen("test/mock_input.txt", "w");
    fprintf(file, "%s\n", input);
    fclose(file);

    freopen("test/mock_input.txt", "r", stdin);
}

void mock_customers_csv() {
    FILE *file = fopen("../customers.csv", "w");
    fprintf(file, "Testily Toastily,238598764,0,100.0,1\n");
    fprintf(file, "User Two,87654321,1,200.0,1\n");
    fprintf(file, "User Three,77654221,0,800.0,1\n");
    fprintf(file, "User Four,67654321,1,200.0,1\n");
    fprintf(file, "User Five,57654321,1,1100.0,1\n");
    fprintf(file, "User Six,47654321,0,2030.0,1\n");
    fclose(file);
    // sum balance 4430.00
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
// ======= 03 read_customer_data()
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


// =========================
// ======= 04 update_balance_in_csv()
// =========================

void setup_csv_with_user() {
    FILE *file = fopen("../customers.csv", "w");
    fprintf(file, "Max Mustermann,123456789,1,100.00,1\n");
    fclose(file);
}

void test_count_total_accounts_SUCCESS(void) {
    mock_customers_csv();
    TEST_ASSERT_EQUAL_INT(6, count_total_accounts());
}

void test_update_balance_in_csv_SUCCESS(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };
    FILE *temp = fopen("mock_input.csv", "w");

    mock_customers_csv();
    csv_header_Expect(temp);
    int result = update_balance_in_csv(&test_user, 15.15);
    TEST_ASSERT_EQUAL_INT(1, result);
}

// =========================
// ======= 05 count_total_accounts()
// =========================

void test_count_total_accounts_SUCCESS(void) {
    mock_customers_csv();
    TEST_ASSERT_EQUAL_INT(6, count_total_accounts());
}



// =========================
// ======= 06 report()
// =========================

void test_report_CORRECT_OUTPUT_SUCCESS(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = OVERDRAFT_LIMIT,
        .balance = 100.00
    };

    char buffer[1024] = {0};

    // Redirect stdout
    fflush(stdout);
    int original_stdout_fd = dup(fileno(stdout));
    FILE *temp_stdout = tmpfile();
    dup2(fileno(temp_stdout), fileno(stdout));

    report(&test_user);

    fflush(stdout);
    fseek(temp_stdout, 0, SEEK_SET);
    fread(buffer, 1, sizeof(buffer) - 1, temp_stdout);

    // Restore stdout
    dup2(original_stdout_fd, fileno(stdout));
    close(original_stdout_fd);
    fclose(temp_stdout);

    TEST_ASSERT_NOT_NULL(strstr(buffer, "===== CUSTOMER REPORT (^‿^) ====="));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "Total accounts in system: 6"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "Total money held in bank: 4430.00$"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "~~~ Last 10 transactions ~~~"));
}

// =========================
// ======= 07 total_money_in_bank()
// =========================
void test_total_money_in_bank_SUCCESS(void) {
    FILE *file = fopen("../customers.csv", "w");
    //fprintf(file, "Name,SSN,Account,Balance,AccountNumber\n");
    fprintf(file, "Testily Toastily,238598764,0,100.0,1\n");
    fprintf(file, "User Two,87654321,1,200.0,1\n");
    fprintf(file, "User Three,77654221,0,800.0,1\n");
    fprintf(file, "User Four,67654321,1,200.0,1\n");
    fprintf(file, "User Five,57654321,1,1100.0,1\n");
    fprintf(file, "User Six,47654321,0,2030.0,1\n");
    fclose(file);
    // sum balance 4430.00

    double result = total_money_in_bank();
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 4430.00, result);
}

void test_total_money_in_bank_EMPTY_FILE(void) {
    FILE *file = fopen("../customers.csv", "w");
    fclose(file);

    double result = total_money_in_bank();
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 00.00, result);
}

void test_total_money_in_bank_INVALID_LINES(void) {
    FILE *file = fopen("../customers.csv", "w");
    fprintf(file, "Testily Toastily,238598764,0,100.0,1\n");
    fprintf(file, "First invalid line\n");
    fprintf(file, "User Two,87654321,1,200.0,1\n");
    fprintf(file, "Second invalid line\n");
    fprintf(file, "User Three,77654221,0,300.0,1\n");
    fclose(file);

    double result = total_money_in_bank();
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 600.00, result);
}