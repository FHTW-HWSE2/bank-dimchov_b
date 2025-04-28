#ifdef TEST

#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

int scanf (const char *format, ...) {
  va_list args; // variadic, holds info by va_start/copy/arg/end
  va_start(args, format);
  int *out = va_arg(args, int *);
  *out = 2;
  va_end(args);
  return *out;
}

void test_get_valid_choice(void) {
    TEST_ASSERT_EQUAL_INT(2, val_user_input);
}

#endif