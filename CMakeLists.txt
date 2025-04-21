cmake_minimum_required(VERSION 3.15...3.31)

# Define the project with name, version, description, and language
project(bank VERSION 1.0
                  DESCRIPTION "bank project"
                  LANGUAGES C)

# Define the executable to be built from source files
add_executable(bank src/main.c src/account.c src/transaction.c src/customer.c
)

# Include header files
target_include_directories(bank PRIVATE include)

# Compiler Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unused-parameter")
