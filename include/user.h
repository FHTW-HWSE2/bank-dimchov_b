#ifndef USER_H
#define USER_H

typedef enum {
    STANDARD = 0,     
    INITIAL_BALANCE,  
    OVERDRAFT_LIMIT   
} Account_type;

typedef struct {
    char name; 
    int SSN; 
    Account_type *account; 
    
    char bank;
} User;

#endif