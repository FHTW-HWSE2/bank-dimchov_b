#ifdef TEST

#include "unity.h"
#include "mock_main.h"

void setUp(void) {}
void tearDown(void) {}

void test_get_valid_choice_valid_choice(void) {
  int mock_choice = 2;

  get_input_ExpectAndReturn(&mock_choice, 1);
  get_input_IgnoreArg_choice();

  mock_choice = 2;

  TEST_ASSERT_EQUAL_INT(2, get_valid_choice());
}

#endif