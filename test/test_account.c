#include <unity.h>
#include <types.h>
#include "customer.h"
#include "mock_main.h"
#include "account.h"

void setUp(void) {}
void tearDown(void) {}

void test_create_account_SUCCESS(void) {

    User user = {
        .name = "Max Mustermann",
        .SSN = "123456789",
        .account = 1,
        .balance = 100.0,
        .account_number = 1
    };
    User user2 = {
        .name = "Moritz Mustermann",
        .SSN = "223456789",
        .account = 1,
        .balance = 200.0,
        .account_number = 1
    };

    FILE *file = fopen("../customers.csv", "w");
    fprintf(file, "Max Mustermann,123456789,1,100.00,1");
    fprintf(file, "Moritz Mustermann,223456789,1,200.00,1");
    fclose(file);

    get_int_value();
    login(&user);
    chooseAccountType(&user2);
    check_number_of_accounts(user2.name, user2.SSN);
    save_account_to_csv(&user, "../customers.csv");

    int result = create_account(&user);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_get_int_value_SUCCESS() {
    int result = get_int_value();
    TEST_ASSERT_EQUAL_INT(1, result);
}