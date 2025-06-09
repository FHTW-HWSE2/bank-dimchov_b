[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/RxH5GUXD)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=18374949&assignment_repo_type=AssignmentRepo)


# Simple Console-Based Banking Application

## Overview:
This is a simple console-based banking application. When the program is running, a clear menu will be printed using the `print_help()` function. This function displays all available options so the user can interact with the system easily.

### Menu Options:
When executed, the following options will appear:

```
*** WELCOME TO THE BANK ***
Select an option:
 1.   Create an account
 2.   Delete account
 3.   Deposit money
 4.   Withdraw money
 5.   Transfer money
 6.   Report
 7.   Simulate 7 days
 8.   Exit
```

Each number corresponds to a specific action you can perform in the banking system. This menu will help guide the user through the available features every time it's printed.

## Project Build

### Prerequisites:
Make sure that `cmake` and `make` are installed on your system.

### Building the Project:

1. **Clone the project**:
   Clone the repository to your local machine:

   ```bash
   git clone git@github.com:FHTW-HWSE2/bank-dimchov_b.git
   cd bank-dimchov_b
   ```

2. **Make the build script executable**:
   Make the `build.sh` script executable:

   ```bash
   chmod +x build.sh
   ```

3. **Run the build script**:
   Now, you can run the script to build the project and execute tests:

   ```bash
   ./build.sh
   ```

### Notes:
- The script assumes that `cmake` and `make` are already installed on your system.
- If you need to modify the build process or make specific configurations, you can edit the `build.sh` script.


# ⚠️ Important Note About `transfer_simulation()`

The function `transfer_simulation()` is intended for **simulation purposes** during project testing and development.

## 📚 How it Works
- **Every time** the simulation runs, it will:
  - **Clear** (overwrite) the file `customers.csv`.
  - Insert a fresh set of **hardcoded users**:
    - Alice Smith
    - Bob Johnson
    - John Doe
- This ensures a **clean test environment** and avoids old or conflicting data from previous runs.
- Simulated transactions are performed between these predefined users over a period of **7 days**, with **3 random transactions per day**.
- After the simulation, a detailed report is:
  - Printed in the terminal.
  - Saved as `csv_simulation.csv`, including:
    - Timestamps
    - Sender/Receiver names
    - Amounts
    - Transaction status
    - Failure reasons
    - Balances before and after transactions.

## ⚠️ Warning
> **This simulation will permanently delete and recreate `customers.csv`** every time it runs.  
> **Do NOT use this behavior in a real production environment.**  
> In real systems, customer data must be preserved and handled carefully.


