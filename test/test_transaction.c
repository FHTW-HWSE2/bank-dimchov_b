#include "unity.h"
#include "mock_account.h"
#include "mock_customer.h"
#include "mock_transaction_helper.h"
#include "types.h"
#include "transaction.h"

void setUp(void) {}
void tearDown(void) {}

// ======= UNIT TEST =======
// ======= 01 deposit()
// ======= 02 withdraw()


// =========================
// ======= 01 deposit()
// =========================
void test_deposit_SUCCESS(void) {
  User test_user = {
      .name = "Testily Toastily",
      .SSN = "238598764",
      .account = 0,
      .balance = 100.00,
      .account_number = 0
  };

  // mock dependencies
  amount_to_deposit_ExpectAndReturn(55.55);
  update_balance_in_csv_ExpectAndReturn(&test_user, 55.55, 1);
  // check_customer_balance_ExpectAndReturn(&test_user, 155.55);
  check_customer_balance_ExpectAndReturn(&test_user, test_user.balance);

  int result = deposit(&test_user);

  // execute unit test
  // epected: 1 for success
  TEST_ASSERT_EQUAL_INT(1, result);
}

void test_deposit_INVALID(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = 0,
        .balance = 100.00
    };

    // mock invalid deposit
    amount_to_deposit_ExpectAndReturn(-10.00);

    int result = deposit(&test_user);
    // epected: 0 for failure
    TEST_ASSERT_EQUAL_INT(0, result);
}

// =========================
// ======= 02 withdraw()
// =========================
void test_withdraw_SUCCESS(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = 0,
        .balance = 100.00
    };

    // mock valid amount to withdraw
    amount_to_withdraw_ExpectAndReturn(&test_user, 10.10);

    // mock valid withdrawal
    withdraw_from_user_ExpectAndReturn(&test_user, 10.10, 1);

    int result = withdraw(&test_user);
    TEST_ASSERT_EQUAL_INT(1, result);
}