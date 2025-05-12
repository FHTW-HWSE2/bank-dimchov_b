#ifdef TEST
#include "unity.h"
#include "transaction_helper.h"

void setup(void) {}
void teardown(void) {}

// ======= UNIT TEST =======
// ======= 01 amount_to_deposit()

// =========================
// ======= 01 amount_to_deposit()
// =========================
void test_validate_amount_to_deposit_VALID(void) {
    double result = validate_amount_to_deposit("15.15");
    TEST_ASSERT_EQUAL_DOUBLE(15.15, result);
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

#endif