#include "unity.h"
#include "mock_account.h"
#include "mock_customer.h"
#include "mock_transaction.h"
#include "types.h"
#include "transaction_helper.h"
#include <stdio.h>

void setUp(void) {}
void tearDown(void) {
    remove("test/mock_input.txt");
    remove("../customers.csv");
    freopen("CON", "r", stdin);
    // freopen("/dev/tty", "r", stdin); // Linux, Mac
}

// ======= UNIT TEST =======
// ======= 00 mock dependecies
// ======= 01 dependecies
// ======= 02 deposit()
// ======= 03 withdraw() // MISSING
// ======= 04 transfer()

// =========================
// ======= 00 mock dependecies
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
}

// =========================
// ======= 01 dependencies:
//         clear_buffer()
//         parse_amount()
//         validate_amount_for_transaction()
//         validate_transaction()
//         amount_to_deposit()
// =========================

// ======= clear_buffer()
void test_clear_buffer(void) {
    TEST_ASSERT_EQUAL(0, clear_buffer());
    // TEST_IGNORE();
}

// ======= parse_amount()
void test_parse_amount_VALID(void) {
    TEST_ASSERT_EQUAL_DOUBLE(15.15, parse_amount("15.15"));
}

void test_parse_amount_INVALID_STRING(void) {
    TEST_ASSERT_EQUAL_DOUBLE(0.0, parse_amount("qwerty"));
}

void test_parse_amount_INVALID_STRING_TRAIL(void) {
    TEST_ASSERT_EQUAL_DOUBLE(0.0, parse_amount("123qwerty"));
}

void test_parse_amount_INVALID_STRING_LEAD(void) {
    TEST_ASSERT_EQUAL_DOUBLE(0.0, parse_amount("as123"));
}

// ======= validate_amount_for_transaction()
void test_validate_amount_for_transaction_VALID(void) {
    mock_stdin("15.15");
    double result = validate_amount_for_transaction("deposit");
    TEST_ASSERT_EQUAL_DOUBLE(15.15, result);
}

void test_validate_amount_for_transaction_VALID_WITH_WHITESPACE(void) {
    mock_stdin(" 15.15  ");
    double result = validate_amount_for_transaction("withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(15.15, result);
}

void test_validate_amount_for_transaction_INVALID_NEGATIVE_NUMBER(void) {
    mock_stdin("-15.15");
    double result = validate_amount_for_transaction("withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_amount_for_transaction_INVALID_STRING(void) {
    mock_stdin("qwerty");
    double result = validate_amount_for_transaction("withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_amount_for_transaction_INVALID_STRING_TRAIL(void) {
    mock_stdin("15.15qwerty");
    double result = validate_amount_for_transaction("deposit");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_amount_for_transaction_INVALID_STRING_TRAIL_WITH_WHITESPACE(void) {
    mock_stdin("15.15qwerty    ");
    double result = validate_amount_for_transaction("withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_amount_for_transaction_INVALID_STRING_LEAD(void) {
    mock_stdin("a15.15qwerty");
    double result = validate_amount_for_transaction("withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_amount_for_transaction_INVALID_STRING_WITH_WHITESPACE(void) {
    mock_stdin(" 15.15 qwerty");
    double result = validate_amount_for_transaction("withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

// ======= validate_transaction()
void test_validate_transaction_VALID(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };

    mock_stdin("15.15");
    double result = validate_transaction(&test_user,"withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(-15.15, result);
}

void test_validate_transaction_INVALID_NEGATIVE_NUM(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };

    mock_stdin("-15.15");
    double result = validate_transaction(&test_user,"withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_transaction_INVALID_INSUFFICIENT_BALANCE_STANDARD(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };

    mock_stdin("150.15");
    double result = validate_transaction(&test_user,"withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_transaction_INVALID_INSUFFICIENT_BALANCE_INITIAL_BALANCE(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = INITIAL_BALANCE,
        .balance = 100.00
    };

    mock_stdin("150.15");
    double result = validate_transaction(&test_user,"withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}


void test_validate_transaction_INVALID_OVERDRAFT_LIMIT_EXCEEDED(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = OVERDRAFT_LIMIT,
        .balance = 100.00
    };

    mock_stdin("1500.15");
    double result = validate_transaction(&test_user,"withdraw");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

// =========================
// ======= 02 deposit:
//            amount_to_deposit()
// =========================
void test_amount_to_deposit_VALID(void) {
    mock_stdin("15.15");
    double result = amount_to_deposit();
    TEST_ASSERT_EQUAL_DOUBLE(15.15, result);
}

void test_amount_to_deposit_VALID_WHITESPACE(void) {
    mock_stdin("    15.15    ");
    double result = amount_to_deposit();
    TEST_ASSERT_EQUAL_DOUBLE(15.15, result);
}

void test_amount_to_deposit_INVALID_NEGATIVE_NUM(void) {
    mock_stdin("-15.15");
    double result = amount_to_deposit();
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_amount_to_deposit_INVALID_STRING(void) {
    mock_stdin("qwerty");
    double result = amount_to_deposit();
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_amount_to_deposit_EMPTY_INPUT(void) {
  double result = validate_amount_to_deposit("");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_amount_to_deposit_MIXED_DATATYPES_LEADING_NUM(void) {
  double result = validate_amount_to_deposit("15.15AbCdEfg");
  TEST_ASSERT_EQUAL_DOUBLE(15.15, result);
  // TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_amount_to_deposit_MIXED_DATATYPES_LEADING_CHAR(void) {
    double result = validate_amount_to_deposit("a15.15AbCdEfg");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_amount_to_deposit_WHITESPACE(void) {
  double result = validate_amount_to_deposit("    15.15 ");
  // TEST_ASSERT_EQUAL_DOUBLE(15.15, result);
  TEST_ASSERT_DOUBLE_WITHIN(0.0001, 15.15, result);
}

// ======= clear_buffer()
void test_clear_buffer(void) {
    TEST_ASSERT_EQUAL(0, clear_buffer());
    // TEST_IGNORE();
}

// =========================
// ======= 03 withdraw:
//            amount_to_withdraw()
//            withdraw_from_user()
// =========================

// ======= amount_to_withdraw()
void test_amount_to_withdraw_SUCCESS(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };

    mock_stdin("15.15");
    double result = amount_to_withdraw(&test_user);
    TEST_ASSERT_EQUAL_DOUBLE(-15.15, result);
}

void test_amount_to_withdraw_FAILURE(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };

    mock_stdin("0.0");
    double result = amount_to_withdraw(&test_user);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

// ======= withdraw_from_user()
void test_withdraw_from_user_SUCCESS(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };

    update_balance_in_csv_ExpectAndReturn(&test_user, -15.15, 0);
    check_customer_balance_ExpectAndReturn(&test_user, test_user.balance);
    int result = withdraw_from_user(&test_user, -15.15);
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_withdraw_from_user_FAILURE(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };

    // amount_to_widthdraw failed and returned 0.0
    int result = withdraw_from_user(&test_user, 0.0);
    TEST_ASSERT_EQUAL_INT(0, result);
}

// =========================
// ======= 04 transfer:
//            validate_recipient()
//            amount_to_transfer()
// =========================

// ======= validate_recipient()
void test_validate_recipient_SUCCESS() {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };

    double transfered_amount = 20.00;

    mock_stdin("User Two\n87654321\n");
    get_int_value_ExpectAndReturn(1);

    mock_customers_csv();

    char line[256] = "User Two,87654321,1,200.0,1\n";
    char *recipient_name = "User Two";
    char *recipient_ssn = "87654321";
    int recipient_account = 1;
    double recipient_balance = 200.00;
    int recipient_account_number = 1;
    // "User Two,87654321,1,200.0,1\n");
    //parse_customer_line(char *line, char *name, char *ssn, int *account, double *balance, int *account_number)
    parse_customer_line_ExpectAndReturn(line, recipient_name, recipient_ssn, &recipient_account, &recipient_balance, &recipient_account_number,1);

    log_transaction_Expect("Testily Toastily", "User Two", "87654321", transfered_amount);

    int result = validate_recipient(&test_user, transfered_amount);
    TEST_ASSERT_EQUAL_INT(1, result);
}

// ======= amount_to_transfer()
void test_amount_to_transfer_VALID(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };

    mock_stdin("15.15");
    double result = amount_to_transfer(&test_user);
    TEST_ASSERT_EQUAL_DOUBLE(-15.15, result);
}

void test_amount_to_transfer_INVALI_INSUFFICIENT_BALANCE(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00
    };


    double result = amount_to_transfer(&test_user);
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}