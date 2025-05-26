#include "unity.h"
#include "mock_account.h"
#include "mock_customer.h"
#include "types.h"
#include "transaction_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setup(void) {}
void teardown(void) {
    remove("test/mock_input.txt");
    freopen("CON", "r", stdin);
    // freopen("/dev/tty", "r", stdin); // Linux, Mac
}

// ======= UNIT TEST =======
// ======= 00 mock dependecies
// ======= 01 dependecies
// ======= 02 deposit()
// ======= 03 withdraw()

// =========================
// ======= 00 mock dependecies
void mock_stdin(const char *input) {
    FILE *file = fopen("test/mock_input.txt", "w");
    fprintf(file, "%s\n", input);
    fclose(file);

    // rewind(file);
    freopen("test/mock_input.txt", "r", stdin);
}

// =========================
// ======= 01 dependencies:
//         clear_buffer()
//         parse_amount()
//         validate_amount_for_transaction()
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

}

// =========================
// ======= 02 deposit:
//
// =========================
