#include <unity.h>
#include <types.h>
#include "mock_customer.h"
#include "mock_main.h"
#include "account.h"

void setUp(void) {}
void tearDown(void) {}

void test_create_account_SUCCES (void) {
    User user = {
        .name = "Alpaka King",
        .SSN = "17272792"
    };

    get_int_value_IgnoreAndReturn(1);
    login_IgnoreAndReturn(1);
    chooseAccountType_IgnoreAndReturn(0);
    check_number_of_accounts_IgnoreAndReturn(1);
    save_account_to_csv_Ignore();

    int result = create_account(&user);

    TEST_ASSERT_EQUAL_INT(0, result);
}