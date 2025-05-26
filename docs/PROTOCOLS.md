# 24/03/25:
## Planned tasks for upcoming classes:
- Plan the project and add main structure
- Create a CMake file
- Add necessary tools like Unity and CMock

# 10/4/25:
Issues:
- pulling from Github didnt work due to the included build directory
-> solution: add the build directory to .gitignore

Extra: 
- Created a local alias to simplify the build process

# 14/04/25:
## Planned tasks for sprint until 28/04/25:
- Function that allows user to create & delete bank account with name and SSN of the client
- Save account data in a csv file
- Function that allows for transferring money between accounts
- Simulate 7days 

# 26/04/25:
## Planned tasks for sprint until 05/05/25:
- allow name of account owner to have first and last name
- save last ten transactions in a file
- testing

# 29/4/25:
Issues:
multiple users had the same name, which caused transactions to be applied to all matching accounts
-> solution: ask for SSN during transfer to uniquely identify the recipient

cmock did not work as expected
-> we will try using Unity for testing instead

- instead of adding a local alias, we will generate a script for building

# 05/05/25:
- Suggested workaround to CMock:
Hardcode simulated values (User, Input, Balance)  in a separate c-File and use the hardcoded mocks for testing instead of CMock.
- Second bank instance:
The project is not going to be adapted to save a second bank instance in the data of an account. The requirement will instead be developed as an isolated case to test if accounts of different bank instances is possible or not (Result: it should not be possible)


Pending: 
- Testing (Clariza)
- One client can hold multiple bank accounts (Marie)
- Total amount of money held by the bank (Dilara)

# 11/05/25
- CMock actually works! =D
  - Mocked files need to be in a separate h-file in order to be mocked by CMock!
    - scanf in and of itself is not mock-able with CMock and or test-able with Unity
      - a workaround would be to refactor the functions to accept input sources or anything but scanf
        - `fgets` and `sscanf` are test-able alternatives that do not even need to be mocked. Yay!
          - example `amount_to_deposit()` in `transaction_helper.c` and tested in `test_transaction_helper.c`
  - the header-file of the functions that are being tested need to be included
    - "header.h" is enough
    - no relative path needed
### UNIT TEST
#### Tested
- deposit()
- validate_amount_to_deposit()
  - refactored deposit() dependencies into smaller test-able functions
- 
#### Further changes for unit testing
- added PROTOCOL_UNIT_TEST.md to docs folder
- re-structured option 3: deposit
  - renamed deposit() to amount_to_deposit()
  - moved everything under option 3 in main.c into a new func deposit()
  - moved dependencies for deposit() into its own .c .h file for easier testing
    - transaction_helper.c (added to CMake)
    - transaction_helper.h
  - re-structured dependency: amount_to_deposit for easer testing
    - extracted and re-factored in order to use sscanf and fgets instead of scanf:
      - added func 
        - validate_amount_to_deposit
        - clear_buffer

## Request
- refactor main()
  - make main() more readable
    - move main functions into corresponding source file
      - example deposit()
  - refactor dependencies / functions into separate functions
    - if unsure: ask ChatGPT how to refactor the function for easier testing
      - some refactoring might make the code more portable and secure
  - extract, refactor and replace any scanf
    - suggestion: try sscanf and fgets

# 18/05/25
- Clariza
  - Add gcovr through Ceedling
  - generate gcovr html 
    - `gcovr --html --html-details -o test/coverage/index.html test/build`
  - Comment out user_count in main.c because it is unused

# 26/05/2025
  - Clariza
    - [ ] Transfering money between two bank institutes is not possible
    - [ ] Actually simulate 7 days of transactions
    - Refactored transfer()
      - Rename original transfer() to validate_recipient()

# 01/06/25
- Clariza
  - Further unit testing
    - deposit()
    - transfer()
      - refactored and moved functions
        - core logic is in transaction_helper.c

# 06/06/2025
- Clariza
  - Online meeting with Mohammed
    - Discussed the "7 day simulation of transaction and report" requirement
    - Resolved misunderstandings
    - Tried helping to understand the transfer_internal() function and what else needed to be done in addition
      - Agreed upon some details of the requirement
        - Timestamps for each transaction
          - Time/Date needs to be simulated
        - Create 3 users for the sim
        - Simulate at least 3 transaction per day
        - Each transaction should be different (different sender, recipient or amount)
        - All simulated transactions shall be recorded in a separate .csv
          - transactions are displayed at the end of thee simulation
          - Report shall be displayed in a table format
            - Timestamp | Sender | Recipient | Amount
            
# 26/05/2025
  - Clariza
    - [ ] Transfering money between two bank institutes is not possible
    - [ ] Actually simulate 7 days of transactions