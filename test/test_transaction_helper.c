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
// ======= 01 deposit()
// ======= 02 withdraw()

// =========================
// ======= 01 deposit()
// ======= core logic:
//         validate_amount_to_deposit(const char *input)
// ======= other dependencies:
//         clear_buffer()
//         amount_to_deposit()
// =========================

// ======= validate_amount_to_deposit(const char *input)
void test_validate_amount_to_deposit_VALID(void) {
    double result = validate_amount_to_deposit("15.15");
    TEST_ASSERT_EQUAL_DOUBLE(15.15, result);
    // TEST_ASSERT_DOUBLE_WITHIN(0.0001, 15.15, result);
}

void test_validate_amount_to_deposit_NEGATIVE(void) {
    double result = validate_amount_to_deposit("-15.15");
    TEST_ASSERT_EQUAL_DOUBLE(0.0, result);
}

void test_validate_amount_to_deposit_INVALID_STRING(void) {
    double result = validate_amount_to_deposit("not_a_number");
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
  TEST_ASSERT_EQUAL_DOUBLE(15.15, result);
  //TEST_ASSERT_DOUBLE_WITHIN(0.0001, 15.15, result);
}

// ======= clear_buffer()
void test_clear_buffer(void) {
  TEST_ASSERT_EQUAL(0, clear_buffer());
  // TEST_IGNORE();
}

// ======= amount_to_deposit()
void mock_stdin(const char *input) {
    FILE *file = fopen("test/mock_input.txt", "w+");
    fprintf(file, "%s\n", input);
    rewind(file);
    freopen("test/mock_input.txt", "r", stdin);
}

void test_amount_to_deposit_VALID(void) {
    clear_buffer();
  mock_stdin("15.15");
  double result = amount_to_deposit();
  TEST_ASSERT_EQUAL_DOUBLE(15.15, result);
  //TEST_IGNORE();
}

// =========================
// ======= 02 withdraw()
// ======= core logic:
//         withdraw_from_user()
//         validate_amount_to_deposit()
// =========================
