# list of unit tests

## transactions.h
### ===== int deposit(User *users); =====
- valid input | PASSED
- invalid input | PASSED

edited in project.yml
```
# Configuration options specific to Unity.
:unity:
:defines: [] <---- added brackets
#  - UNITY_EXCLUDE_FLOAT <---- changed into a comment
```
Changed in to a comment in order to allow floating points during testing.

### ===== double withdraw(User *user) =====
#### ====== ===== double validate_amount_to_deposit(const char *input) ===== =====
- valid | FAILED <----------------------------------------------- !!!
  - valid | PASSED - 18/05/25 Add all funcs to header not just the once being shared with other C-files
- negative | PASSED
- invalud string | PASSED
- empty input | PASSED

- Console error message
```"Unity Double Precision Disabled"```
- Solution: edited project.yml
```
# Configuration options specific to Unity. 
:unity:
  :defines:
    - UNITY_INCLUDE_FLOAT
    - UNITY_INCLUDE_DOUBLE
```
