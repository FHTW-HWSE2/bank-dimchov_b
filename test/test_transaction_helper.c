#include "unity.h"
#include "types.h"
#include "transaction_helper.h"

void setup(void) {}
void teardown(void) {}

// ======= UNIT TEST =======
// ======= 01 deposit()
// ======= 02 withdraw()

// =========================
// ======= 01 deposit()
// ======= core logic: validate_amount_to_deposit()
// =========================
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

// =========================
// ======= 02 withdraw()
// ======= core logic:
//         withdraw_from_user()
//         validate_amount_to_deposit()
// =========================
