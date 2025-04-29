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

