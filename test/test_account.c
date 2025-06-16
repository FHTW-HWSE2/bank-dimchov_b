#include <unity.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>
//#include "customer.h"
#include "mock_customer.h"
#include "mock_main.h"
#include "mock_transaction.h"
#include "account.h"

void setUp(void) {
    parse_customer_line_IgnoreAndReturn(1);
    read_customer_data_IgnoreAndReturn(1);
    save_account_to_csv_Ignore();
    csv_header_Ignore();
}
void tearDown(void) {}
/*
    remove("test/mock_input.txt");
    remove("../customers.csv");
    freopen("CON", "r", stdin);
    // freopen("/dev/tty", "r", stdin); // Linux, Mac
}*/
// =========================
// ======= 00 mock dependecies
// =========================

void mock_stdin(const char *input) {
    FILE *file = fopen("test/mock_input.txt", "w");
    fprintf(file, "%s\n", input);
    fclose(file);

    freopen("test/mock_input.txt", "r", stdin);
}

void mock_customers_csv() {
    FILE *file = fopen("../customers.csv", "w");
    fprintf(file, "Testily Toastily,238598764,0,100.0,1\n");
    fprintf(file, "User Two,87654321,1,200.0,1\n");
    fprintf(file, "User Three,77654221,0,800.0,1\n");
    fclose(file);
}


// =========================
// ======= generate_ssn(User *user)
// =========================
void test_generate_ssn_SUCCESS(void) {
    User test_user = {
        .name = "Testily Toastily",
        //.SSN = "238598764",
        //.account = STANDARD,
        //.balance = 100.00,
        //.account_number = 1
    };

    //srand(90); // set fixed seed for predictability
    generate_ssn(&test_user);

    int result = strlen(test_user.SSN);
    TEST_ASSERT_EQUAL_INT(8,result);

    // Check if it only contains digits
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE_MESSAGE(test_user.SSN[i] >= '0' && test_user.SSN[i] <= '9', "SSN should contain only digits");
    }
}

// =========================
// ======= chooseAccountType(User *user)
// =========================

void test_choose_account_type_STANDARD(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        //.account = STANDARD,
        //.balance = 100.00,
        //.account_number = 1
    };

    mock_stdin("0");

    int result = chooseAccountType(&test_user);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(STANDARD, test_user.account);
}

void test_choose_account_type_INITIAL_BALNCE(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        //.account = STANDARD,
        //.balance = 100.00,
        //.account_number = 1
    };

    mock_stdin("1\n100.00");
    int result = chooseAccountType(&test_user);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(INITIAL_BALANCE, test_user.account);
    TEST_ASSERT_EQUAL_DOUBLE(100.00, test_user.balance);
}

void test_choose_account_type_OVERDRAFT_LIMIT(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        //.account = STANDARD,
        //.balance = 100.00,
        //.account_number = 1
    };

    mock_stdin("2\n");

    int result = chooseAccountType(&test_user);
    TEST_ASSERT_EQUAL_INT(2, result);
    TEST_ASSERT_EQUAL_INT(OVERDRAFT_LIMIT, test_user.account);
}

void test_choose_account_type_INVALID_INPUT(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        //.account = STANDARD,
        //.balance = 100.00,
        //.account_number = 1
    };

    mock_stdin("q\n");

    int result = chooseAccountType(&test_user);
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_choose_account_type_BEYOND_ALLOWED_INIT_BALANCE(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        //.account = STANDARD,
        //.balance = 100.00,
        //.account_number = 1
    };

    mock_stdin("1\n100000.01");

    int result = chooseAccountType(&test_user);
    TEST_ASSERT_EQUAL_INT(1, result);
}

// =========================
// ======= create_account(User *user);
// =========================
void test_create_account_NEW_CUSTOMER_SUCCESS(void) {
    User user;
    mock_stdin("0");
    get_int_value();
    mock_stdin("Max Mustermann");
    generate_ssn(&user);
    chooseAccountType(&user);
    save_account_to_csv_Ignore();
    int result = create_account(&user);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_STRING("Max Mustermann", user.name);
    TEST_ASSERT_EQUAL_STRING("1234556789", user.SSN);
    TEST_ASSERT_EQUAL_INT(1, user.account_number);

    FILE *file = fopen("../customers.csv", "r");
    TEST_ASSERT_NOT_NULL(file);
    /*
    char buffer[256];
    int found = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, "Max Mustermann")) {
            found = 1;
            break;
        }
    }*/
    fclose(file);
    // TEST_ASSERT_TRUE_MESSAGE(found, "Customer was not saved to CSV");
}

void test_create_account_INITIAL_BALANCE(void) {
    User user = {
        .name = "Max Mustermann"
    };
    mock_stdin("1\n1515.15");
    chooseAccountType(&user);
    int result =  create_account(&user);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(INITIAL_BALANCE, user.account);
    TEST_ASSERT_EQUAL_DOUBLE(1515.15, user.balance);
}

// =========================
// ======= get_int_value();
// =========================

void test_get_int_value_SUCCESS() {
    int result = get_int_value();
    TEST_ASSERT_EQUAL_INT(1, result);
}
*/


// =========================
// ======= login(User *user);
// =========================
void test_login_SUCCESS(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00,
        .account_number = 1
    };
    char *mock_name = "Testily Toastily";
//    mock_stdin("Testily Toastily");
//    strcpy(mock_name, "Testily Toastily");
//    mock_stdin("238598764");
    char *mock_ssn = "238598764";


    check_number_of_accounts("Testily Toastily", test_user.SSN);
    int mock_number_of_account = get_int_value();
    read_customer_data(&test_user, mock_name, mock_ssn, &mock_number_of_account);

    int result = login(&test_user);
    TEST_ASSERT_EQUAL_INT(1, result);
  }

void test_login_WRONG_SSN(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00,
        .account_number = 1
    };
    char *mock_name = "Testily Toastily";

    char *mock_ssn = "101505909";


    check_number_of_accounts("Testily Toastily", test_user.SSN);
    int mock_number_of_account = get_int_value();
    read_customer_data(&test_user, mock_name, mock_ssn, &mock_number_of_account);

    int result = login(&test_user);
    TEST_ASSERT_EQUAL_INT(0, result);
}

// =========================
// ======= delete_account(User *user)
// =========================

void test_delete_account_NEGATIVE_BALANCE(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = -100.00,
        .account_number = 1
    };

    int result = delete_account(&test_user);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_delete_account_POSITIVE_BALANCE(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00,
        .account_number = 1
    };

    int result = delete_account(&test_user);
    TEST_ASSERT_EQUAL_INT(0, result);
}



void test_delete_account_FAILED_TO_OPEN_FILE(void) {
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00,
        .account_number = 1
    };
    FILE *file = fopen("../customers.csv", "r");
    int result = delete_account(&test_user);
    TEST_ASSERT_EQUAL_INT(0, result);
}

// =========================
// ======= check_number_of_accounts(const char *name, const char *ssn)
// =========================

void test_check_number_of_accounts_SUCCESS(void) {
    FILE *file = fopen("../customers.csv", "w");
    fprintf(file, "Testily Toastily,238598764,0,100.0,1");
    fprintf(file, "Testily Toastily,238598764,0,100.0,2");
    fprintf(file, "Testily Toastily,238598764,0,100.0,3");
    fclose(file);

    FILE *fileread = fopen("../customers.csv", "r");
    User test_user = {
        .name = "Testily Toastily",
        .SSN = "238598764",
        .account = STANDARD,
        .balance = 100.00,
        .account_number = 1
    };
    char *mock_name = test_user.name;
    char *mock_ssn = test_user.SSN;
    int mock_account_type = test_user.account;
    double mock_balance = test_user.balance;
    int mock_account_number = test_user.account_number;

    char mock_csv_line[256] = "Testily Toastily,238598764,0,100.0,1";
    parse_customer_line(mock_csv_line,mock_name, mock_ssn, &mock_account_type, &mock_balance, &mock_account_number);

    int result = check_number_of_accounts(test_user.name, test_user.SSN);
    TEST_ASSERT_EQUAL_INT(3, result);

    fclose(fileread);
}
