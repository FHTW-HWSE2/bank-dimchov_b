#ifdef TEST

#include "unity.h"
//#include "main.h"
#include "mock_main.h"

void setUp(void) {}
void tearDown(void) {}

void test_get_valid_choice_valid_choice(void) {
  int expected_value = 2;
  //TEST_ASSERT_EQUAL_INT(get_valid_choice(), 2);
  get_input_ExpectAndReturn(expected_value, 2);

  //int mock_choice = get_valid_choice();
  //clear_stdin_ExpectAndReturn(0);
  TEST_ASSERT_EQUAL_INT(expected_value, get_valid_choice());
}

void test_get_valid_choice_invalid_choice(void) {
  int invalid_choice = -1;
  int valid_choice = 8;

  get_input_ExpectAndReturn(invalid_choice, -1);
  clear_stdin_ExpectAndReturn(0);
  get_input_ExpectAndReturn(valid_choice, 1);

  int mock_choice = get_valid_choice();
  TEST_ASSERT_EQUAL_INT(get_valid_choice, mock_choice);
}

#endif