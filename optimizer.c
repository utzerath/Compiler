    #include "optimizer.h"
    #include <stdbool.h>
    #include <ctype.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdint.h>

// Data structure to map function names to their return variables
typedef struct FunctionReturn {
    char* functionName;
    char* returnVar;
    struct FunctionReturn* next;
} FunctionReturn;

FunctionReturn* functionReturns = NULL;

void addFunctionReturn(const char* functionName, const char* returnVar) {
    if (functionName == NULL || returnVar == NULL) {
        fprintf(stderr, "Error: functionName or returnVar is NULL in addFunctionReturn.\n");
        return;
    }

    FunctionReturn* newEntry = malloc(sizeof(FunctionReturn));
    if (newEntry == NULL) {
        perror("Failed to allocate memory for FunctionReturn");
        exit(EXIT_FAILURE);
    }
    addToFreeList(newEntry);  // Track the FunctionReturn struct

    newEntry->functionName = strdup(functionName);
    if (newEntry->functionName == NULL) {
        perror("Failed to duplicate functionName");
        exit(EXIT_FAILURE);
    }
    addToFreeList(newEntry->functionName);

    newEntry->returnVar = strdup(returnVar);
    if (newEntry->returnVar == NULL) {
        perror("Failed to duplicate returnVar");
        exit(EXIT_FAILURE);
    }
    addToFreeList(newEntry->returnVar);

    newEntry->next = functionReturns;
    functionReturns = newEntry;

    printf("Added function return mapping: %s returns %s\n", functionName, returnVar);
}




// Enhanced FreeListNode with reference counting
typedef struct FreeListNode {
    void* ptr;
    int refCount;                // Reference count for the pointer
    struct FreeListNode* next;
} FreeListNode;

FreeListNode* freeList = NULL;

// Function to add pointers to the free list with reference counting
void addToFreeList(void* ptr) {
    if (ptr == NULL) return;
    printf("Adding pointer to freeList: %p\n", ptr);  // Debug line

    // Check if the pointer is already in the freeList
    FreeListNode* current = freeList;
    while (current != NULL) {
        if (current->ptr == ptr) {
            current->refCount += 1;
            printf("Pointer %p already in freeList. Incremented refCount to %d.\n", ptr, current->refCount);
            return;
        }
        current = current->next;
    }

    // Pointer not found in freeList, add as new node
    FreeListNode* node = malloc(sizeof(FreeListNode));
    if (node == NULL) {
        perror("Failed to allocate memory for FreeListNode");
        exit(EXIT_FAILURE);
    }
    node->ptr = ptr;
    node->refCount = 1;           // Initialize refCount to 1
    node->next = freeList;
    freeList = node;
}

// Function to decrement reference count and free if refCount reaches zero
void removeFromFreeList(void* ptr) {
    if (ptr == NULL) return;
    printf("Removing pointer from freeList: %p\n", ptr);  // Debug line

    FreeListNode* current = freeList;
    FreeListNode* prev = NULL;

    while (current != NULL) {
        if (current->ptr == ptr) {
            current->refCount -= 1;
            printf("Decremented refCount of pointer %p to %d.\n", ptr, current->refCount);
            if (current->refCount <= 0) {
                // Remove node from freeList
                if (prev == NULL) {
                    freeList = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current->ptr);     // Free the actual memory
                free(current);          // Free the FreeListNode
                printf("Freed pointer %p and removed from freeList.\n", ptr);
            }
            return;
        }
        prev = current;
        current = current->next;
    }

    // Pointer not found in freeList
    fprintf(stderr, "Warning: Attempted to remove pointer %p which is not in freeList.\n", ptr);
}

// Function to free all pointers in the freeList (used during program termination)
void freeAllFreeList() {
    FreeListNode* current = freeList;
    while (current != NULL) {
        FreeListNode* temp = current;
        current = current->next;
        if (temp->ptr != NULL) {
            free(temp->ptr);
            printf("Freed pointer %p from freeList during cleanup.\n", temp->ptr);
        }
        free(temp);
    }
    freeList = NULL;
}

// Updated freeTACNode to use removeFromFreeList instead of directly freeing
void freeTACNode(TAC* node) {
    if (node->result) {
        removeFromFreeList(node->result);
    }
    if (node->arg1) {
        removeFromFreeList(node->arg1);
    }
    if (node->op) {
        removeFromFreeList(node->op);
    }
    if (node->arg2) {
        removeFromFreeList(node->arg2);
    }
    removeFromFreeList(node);
}


bool isUsedLaterFinal(TAC* head, const char* varName) { 
    if (varName == NULL) {
        printf("isUsedLaterFinal: varName is NULL\n");
        return false;
    }

    printf("isUsedLaterFinal: Checking for varName='%s'\n", varName);

    while (head != NULL) {
        // Check for variable redefinition (to avoid considering it "used" after redefinition)
        if (head->op && strcmp(head->op, "=") == 0 && head->result && strcmp(head->result, varName) == 0) {
            return false;  // Variable redefined before any use
        }

        // Check if `varName` is used in either `arg1` or `arg2`, excluding 'return' operations
        if ((head->arg1 && strcmp(head->arg1, varName) == 0) ||
            (head->arg2 && strcmp(head->arg2, varName) == 0)) {

            if (head->op && strcmp(head->op, "return") == 0) {
                // Ignore uses in 'return' statements
                // Continue to the next instruction
            } else {
                return true;  // Variable is used in another operation before redefinition
            }
        }

        // Check if `varName` appears in a "write" operation as `arg1`
        if (head->op && strcmp(head->op, "write") == 0 && head->arg1 && strcmp(head->arg1, varName) == 0) {
            return true;  // Variable is used in a "write" operation
        }

        head = head->next;
    }
    return false;
}

void deadCodeEliminationFinal(TAC** head, bool freeAll) {
    TAC* current = *head;
    TAC* prev = NULL;

    while (current != NULL) {
        printf("Processing TAC in deadCodeEliminationFinal: op='%s', result='%s'\n",
            current->op ? current->op : "NULL",
            current->result ? current->result : "NULL");

        bool shouldRemove = false;

        // Remove 'return' statements
        if (current->op != NULL && strcmp(current->op, "return") == 0) {
            printf("Dead Code Elimination: Removing 'return' statement\n");
            shouldRemove = true;
        }
        // Remove assignments where result is not used later
        else if (current->op != NULL && strcmp(current->op, "=") == 0 && current->result != NULL) {
            // Only remove if the result variable is not used later in the TAC list
            if (!isUsedLaterFinal(current->next, current->result)) {
                printf("Dead Code Elimination: Removing dead assignment to '%s'\n", current->result);
                shouldRemove = true;
            }
        }

        if (shouldRemove) {
            // Adjust pointers to remove the current node
            TAC* temp = current;
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            current = current->next;
            freeTACNode(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }

    // If freeAll is true, free any remaining TAC nodes
    if (freeAll) {
        freeTACList(*head);
        *head = NULL;
    }
}





bool deadCodeElimination(TAC** head, bool freeAll) {
    bool changed = false;
    TAC* current = *head;
    TAC* prev = NULL;

    while (current != NULL) {
        printf("Processing TAC in deadCodeElimination: op='%s', result='%s'\n",
            current->op ? current->op : "NULL",
            current->result ? current->result : "NULL");

        // Only consider removing assignments if they truly have no effect
    if (current->op != NULL && strcmp(current->op, "=") == 0 && current->result != NULL) {
        // Skip array assignments
        if (strstr(current->result, "[") != NULL) {
            // This is an array assignment, do not remove
            printf("Skipping array assignment to '%s'\n", current->result);
        } else {
            // Only remove if the result variable is not used later in the TAC list
            if (!isUsedLater(current->next, current->result)) {
                printf("Dead Code Elimination: Removing dead assignment to '%s'\n", current->result);
                changed = true;

                // Adjust pointers to remove the current node
                if (prev == NULL) {
                    *head = current->next;
                } else {
                    prev->next = current->next;
                }

                TAC* nextNode = current->next;
                freeTACNode(current);
                current = nextNode;
                continue;
            }
        }
    }
        prev = current;
        current = current->next;
    }

    // If freeAll is true, free any remaining TAC nodes
    if (freeAll) {
        freeTACList(*head);
        *head = NULL;
    }

    return changed;
}



bool isUsedLater(TAC* head, const char* varName) {
    if (varName == NULL) {
        printf("isUsedLater: varName is NULL\n");
        return false;
    }

    printf("isUsedLater: Checking for varName='%s'\n", varName);

    // **If the variable is a parameter, we consider it as used**
    if (strncmp(varName, "param", 5) == 0) {
        // It's a parameter variable, so we consider it as used
        return true;
    }
    while (head != NULL) {
        // Check for variable redefinition (to avoid considering it "used" after redefinition)
        if (head->op && strcmp(head->op, "=") == 0 && head->result && strcmp(head->result, varName) == 0) {
            return false;  // Variable redefined before any use
        }

        // Check if `varName` is used in either `arg1` or `arg2` in any meaningful operation
        if ((head->arg1 && strcmp(head->arg1, varName) == 0) ||
            (head->arg2 && strcmp(head->arg2, varName) == 0)) {
            return true;  // Variable is used in another operation before redefinition
        }

        // Check if `varName` appears in a "write" operation as `arg1`
        if (head->op && strcmp(head->op, "write") == 0 && head->arg1 && strcmp(head->arg1, varName) == 0) {
            return true;  // Variable is used in a "write" operation
        }

        head = head->next;
    }
    return false;
}



void safeStrReplace(char** target, const char* source) {
    if (*target && *target != source) {
        removeFromFreeList(*target);
    }
    if (source) {
        char* trimmed = trimWhitespace(source);
        *target = trimmed;
        if (*target == NULL) {
            perror("Failed to allocate memory for string duplication");
            exit(EXIT_FAILURE);
        }
        addToFreeList(*target);
    } else {
        *target = NULL;
    }
}

bool isNumericConstant(const char* str) {
    return isConstant(str) || isFloatConstant(str);
}



// Utility function to check if a string is a valid float constant
bool isFloatConstant(const char* str) {
    if (str == NULL || *str == '\0') {
        return false;
    }

    // Skip leading whitespace
    while (isspace((unsigned char)*str)) {
        ++str;
    }

    char* endptr;
    double val = strtod(str, &endptr);

    // If strtod didn't parse anything, return false
    if (str == endptr) {
        return false;
    }

    // Skip any trailing whitespace
    while (isspace((unsigned char)*endptr)) {
        ++endptr;
    }

    // If we reached the end, it's a valid float constant
    return *endptr == '\0';
}

bool isConstant(const char* str) {
    if (str == NULL || *str == '\0') {
        return false;
    }

    // Skip leading whitespace
    while (isspace((unsigned char)*str)) {
        ++str;
    }

    // Check for negative sign
    if (*str == '-') {
        ++str;
    }

    // There should be at least one digit
    if (!isdigit((unsigned char)*str)) {
        return false;
    }

    // Check the rest of the characters
    while (*str && !isspace((unsigned char)*str)) {
        if (!isdigit((unsigned char)*str)) {
            return false;
        }
        ++str;
    }

    // Skip trailing whitespace
    while (isspace((unsigned char)*str)) {
        ++str;
    }

    // If we reached the end, it's a valid constant
    return *str == '\0';
}

// Utility function to check if a string is a boolean constant ("true" or "false")
bool isBooleanConstant(const char* str) {
    if (str == NULL) return false;
    return strcmp(str, "true") == 0 || strcmp(str, "false") == 0;
}




// Utility function to parse array access expressions like "arr[0]"
bool parseArrayAccess(const char* expr, char** arrayName, char** index) {
    if (expr == NULL) return false;

    // Trim whitespace from the expression
    char* trimmedExpr = trimWhitespace(expr);
    if (trimmedExpr == NULL) return false;

    // Find the '[' character
    char* leftBracket = strchr(trimmedExpr, '[');
    if (leftBracket == NULL) {
        free(trimmedExpr);
        return false;
    }

    // Find the ']' character
    char* rightBracket = strchr(leftBracket, ']');
    if (rightBracket == NULL) {
        free(trimmedExpr);
        return false;
    }

    // Extract the array name
    size_t arrayNameLen = leftBracket - trimmedExpr;
    *arrayName = (char*)malloc(arrayNameLen + 1);
    if (*arrayName == NULL) {
        perror("Failed to allocate memory for array name");
        exit(EXIT_FAILURE);
    }
    strncpy(*arrayName, trimmedExpr, arrayNameLen);
    (*arrayName)[arrayNameLen] = '\0';

    // Extract the index expression
    size_t indexLen = rightBracket - leftBracket - 1;
    *index = (char*)malloc(indexLen + 1);
    if (*index == NULL) {
        perror("Failed to allocate memory for index");
        exit(EXIT_FAILURE);
    }
    strncpy(*index, leftBracket + 1, indexLen);
    (*index)[indexLen] = '\0';

    free(trimmedExpr);

    // Trim whitespace from array name and index
    char* temp = trimWhitespace(*arrayName);
    free(*arrayName);
    *arrayName = temp;

    temp = trimWhitespace(*index);
    free(*index);
    *index = temp;

    return true;
}


bool constantFolding(TAC** head) {
    TAC* current = *head;
    bool changed = false;

    while (current != NULL) {
        // Debug: Print the exact op string with delimiters
        if (current->op != NULL) {
            printf("Current op: '%s'\n", current->op);
        } else {
            printf("Current op: NULL\n");
        }

        // Skip 'write' operations entirely
        if (current->op != NULL && strcmp(current->op, "write") == 0) {
            printf("Skipping 'write' operation: result='%s'\n", current->result ? current->result : "NULL");
            current = current->next;
            continue;
        }

        printf("Folding TAC: result='%s', arg1='%s', op='%s', arg2='%s'\n",
            current->result ? current->result : "NULL",
            current->arg1 ? current->arg1 : "NULL",
            current->op ? current->op : "NULL",
            (current->arg2 && (uintptr_t)current->arg2 != 0xbebebebebebebebeULL) ? current->arg2 : "UNINITIALIZED");

        // Handle 'array_access' operations
        if (current->op != NULL && strcmp(current->op, "array_access") == 0) {
            printf("Handling 'array_access' operation.\n");
            if (current->arg2 != NULL) {
                // Replace arg1 with arg2
                safeStrReplace(&current->arg1, current->arg2);
                // Set op and arg2 to NULL
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
                printf("Array access folded: %s = %s\n", current->result, current->arg1);
            } else {
                printf("arg2 is NULL, cannot fold 'array_access' operation.\n");
            }
            current = current->next;
            continue;
        }

        // Handle operations where both arguments are numeric constants
        if (current->arg1 && current->arg2 &&
            (uintptr_t)current->arg1 != 0xbebebebebebebebeULL &&
            (uintptr_t)current->arg2 != 0xbebebebebebebebeULL &&
            isNumericConstant(current->arg1) && isNumericConstant(current->arg2)) {

            // Determine if we are dealing with integer or float constants
            bool isIntOperation = isConstant(current->arg1) && isConstant(current->arg2);
            bool isFloatOperation = isFloatConstant(current->arg1) || isFloatConstant(current->arg2);

            if (isIntOperation) {
                if (strcmp(current->op, "+") == 0) {
                    int result = atoi(current->arg1) + atoi(current->arg2);
                    foldOperation(current, result);
                    changed = true;
                }
                else if (strcmp(current->op, "-") == 0) {
                    int result = atoi(current->arg1) - atoi(current->arg2);
                    foldOperation(current, result);
                    changed = true;
                }
                else if (strcmp(current->op, "*") == 0) {
                    int result = atoi(current->arg1) * atoi(current->arg2);
                    foldOperation(current, result);
                    changed = true;
                }
                else if (strcmp(current->op, "/") == 0) {
                    int right = atoi(current->arg2);
                    if (right != 0) {
                        int result = atoi(current->arg1) / right;
                        foldOperation(current, result);
                        changed = true;
                    } else {
                        fprintf(stderr, "Error: Division by zero encountered in constant folding.\n");
                    }
                }
            } else if (isFloatOperation) {
                float left = atof(current->arg1);
                float right = atof(current->arg2);
                float result = 0;

                if (strcmp(current->op, "+") == 0 || strcmp(current->op, "add.s") == 0) {
                    result = left + right;
                }
                else if (strcmp(current->op, "-") == 0 || strcmp(current->op, "sub.s") == 0) {
                    result = left - right;
                }
                else if (strcmp(current->op, "*") == 0 || strcmp(current->op, "mul.s") == 0) {
                    result = left * right;
                }
                else if (strcmp(current->op, "/") == 0 || strcmp(current->op, "div.s") == 0) {
                    if (right != 0.0f) {
                        result = left / right;
                    } else {
                        fprintf(stderr, "Error: Division by zero encountered in constant folding (floats).\n");
                        current = current->next;
                        continue;
                    }
                }

                foldFloatOperation(current, result);
                changed = true;
            }
        }

        // Identity operation folding for integers, if arg2 is valid
        if (current->arg2 && (uintptr_t)current->arg2 != 0xbebebebebebebebeULL && isNumericConstant(current->arg2)) {
            if (strcmp(current->op, "*") == 0 && strcmp(current->arg2, "1") == 0) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
            else if (strcmp(current->op, "+") == 0 && strcmp(current->arg2, "0") == 0) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
            else if (strcmp(current->op, "-") == 0 && strcmp(current->arg2, "0") == 0) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
            else if (strcmp(current->op, "/") == 0 && strcmp(current->arg2, "1") == 0) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
        }

        // Identity operation folding for floating-point numbers
        if (current->arg2 && (uintptr_t)current->arg2 != 0xbebebebebebebebeULL && isFloatConstant(current->arg2)) {
            float arg2Val = atof(current->arg2);
            if ((strcmp(current->op, "mul.s") == 0 || strcmp(current->op, "*") == 0) && arg2Val == 1.0f) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
            else if ((strcmp(current->op, "add.s") == 0 || strcmp(current->op, "+") == 0) && arg2Val == 0.0f) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
            else if ((strcmp(current->op, "sub.s") == 0 || strcmp(current->op, "-") == 0) && arg2Val == 0.0f) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
            else if ((strcmp(current->op, "div.s") == 0 || strcmp(current->op, "/") == 0) && arg2Val == 1.0f) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
        }

        current = current->next;
    }

    return changed;
}




void foldOperation(TAC* current, int result) {
    char resultStr[20];
    sprintf(resultStr, "%d", result);
    safeStrReplace(&current->arg1, resultStr);  // Replace with result string
    safeStrReplace(&current->op, "=");          // Properly replace 'op'
    safeStrReplace(&current->arg2, NULL);       // Clear arg2
}


void foldFloatOperation(TAC* current, float result) {
    char resultStr[30];
    snprintf(resultStr, sizeof(resultStr), "%.6f", result);
    safeStrReplace(&current->arg1, resultStr);
    safeStrReplace(&current->op, "=");
    safeStrReplace(&current->arg2, NULL);
}




char* trimWhitespace(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    // Skip leading whitespace
    while (isspace((unsigned char)*str)) {
        ++str;
    }

    // If all spaces, return empty string
    if (*str == '\0') {
        char* emptyStr = strdup("");
        if (emptyStr == NULL) {
            perror("Failed to allocate memory for empty string");
            exit(EXIT_FAILURE);
        }
        return emptyStr;
    }

    // Find the end of the string
    const char* end = str + strlen(str) - 1;

    // Skip trailing whitespace
    while (end > str && isspace((unsigned char)*end)) {
        --end;
    }

    // Length of trimmed string
    size_t len = end - str + 1;

    // Allocate new string
    char* trimmed = (char*)malloc(len + 1);
    if (trimmed == NULL) {
        perror("Failed to allocate memory for trimmed string");
        exit(EXIT_FAILURE);
    }
    strncpy(trimmed, str, len);
    trimmed[len] = '\0';
    return trimmed;
}




bool constantPropagation(TAC** head) {
    bool changed = false;
    bool localChanged;

    // Extend VarValue to include array indices
    typedef struct VarValue {
        char* varName;
        char* index;  // NULL for scalar variables, non-NULL for array elements
        char* value;
        struct VarValue* next;
    } VarValue;

    VarValue* varTable = NULL;

    do {
        localChanged = false;
        TAC* current = *head;

        while (current != NULL) {

        // Handle 'return' instructions
        if (current->op != NULL && strcmp(current->op, "return") == 0) {
            // The function name is in arg2, and the return variable is in arg1
            printf("Processing 'return' instruction:\n");
            printf("  op: '%s'\n", current->op);
            printf("  arg1 (returnVar): '%s'\n", current->arg1 ? current->arg1 : "NULL");
            printf("  arg2 (functionName): '%s'\n", current->arg2 ? current->arg2 : "NULL");
            printf("  result: '%s'\n", current->result ? current->result : "NULL");

            if (current->arg1 == NULL || current->arg2 == NULL) {
                fprintf(stderr, "Error: 'return' instruction has NULL arg1 or arg2.\n");
                current = current->next;
                continue;
            }

            // Add the function return mapping
            addFunctionReturn(current->arg2, current->arg1);
            current = current->next;
            continue;
        }

        // Inside your constantPropagation function
        if (current->op != NULL && strcmp(current->op, "=") == 0 && current->arg1 != NULL) {
            // Handle array access in arg1
            char* arrayName = NULL;
            char* indexStr = NULL;
            if (parseArrayAccess(current->arg1, &arrayName, &indexStr)) {
                // Check if we have a known value for this array element
                VarValue* var = varTable;
                while (var != NULL) {
                    if (strcmp(var->varName, arrayName) == 0 && strcmp(var->index, indexStr) == 0) {
                        // Replace the array access with the known value
                        printf("Replacing array access '%s' with value '%s'\n", current->arg1, var->value);
                        safeStrReplace(&current->arg1, var->value);
                        changed = true;
                        break;
                    }
                    var = var->next;
                }
                free(arrayName);
                free(indexStr);
            }
            // Continue with the rest of the constant propagation logic
        }


        
        // Skip 'write' operations entirely
        if (current->op != NULL && strcmp(current->op, "write") == 0) {
            current = current->next;
            continue;
        }

        // Handle assignments to array elements
        if (current->op != NULL && strcmp(current->op, "[]=") == 0) {
            // current->result: array name
            // current->arg1: index
            //current->arg2: value

            // Attempt to replace arg2 if it's a variable with a known constant value
            if (current->arg2 != NULL && !isNumericConstant(current->arg2)) {
                VarValue* var = varTable;
                while (var != NULL) {
                    if (var->index == NULL && strcmp(current->arg2, var->varName) == 0) {
                        printf("Propagating constant: Replacing '%s' with '%s' in arg2\n", current->arg2, var->value);
                        safeStrReplace(&current->arg2, var->value);
                        changed = true;
                        break;
                    }
                    var = var->next;
                }
            }

            // Now, check if arg2 is a constant (after possible substitution)
            if (isNumericConstant(current->arg2) && isNumericConstant(current->arg1)) {
                // Map array element to constant value
                VarValue* newVar = malloc(sizeof(VarValue));
                if (newVar == NULL) {
                    perror("Failed to allocate memory for VarValue");
                    exit(EXIT_FAILURE);
                }
                addToFreeList(newVar);  // Track the VarValue struct

                newVar->varName = strdup(current->result);
                if (newVar->varName == NULL) {
                    perror("Failed to allocate memory for varName");
                    exit(EXIT_FAILURE);
                }
                addToFreeList(newVar->varName);

                newVar->index = strdup(current->arg1);  // Store index
                if (newVar->index == NULL) {
                    perror("Failed to allocate memory for index");
                    exit(EXIT_FAILURE);
                }
                addToFreeList(newVar->index);

                newVar->value = strdup(current->arg2);
                if (newVar->value == NULL) {
                    perror("Failed to allocate memory for value");
                    exit(EXIT_FAILURE);
                }
                addToFreeList(newVar->value);

                newVar->next = varTable;
                varTable = newVar;
                printf("Added mapping: %s[%s] = %s\n", newVar->varName, newVar->index, newVar->value);
                changed = true;
            } else {
                // Invalidate any existing mapping for this array element
                VarValue** indirect = &varTable;
                while (*indirect) {
                    if ((*indirect)->index != NULL &&
                        strcmp((*indirect)->varName, current->result) == 0 &&
                        strcmp((*indirect)->index, current->arg1) == 0) {
                        VarValue* temp = *indirect;
                        *indirect = (*indirect)->next;
                        removeFromFreeList(temp->varName);
                        removeFromFreeList(temp->index);
                        removeFromFreeList(temp->value);
                        removeFromFreeList(temp);
                        printf("Removed mapping for array element: %s[%s]\n", current->result, current->arg1);
                        break;
                    }
                    indirect = &(*indirect)->next;
                }
            }
            current = current->next;
            continue;
        }

        // Handle array accesses in operations
        // Replace array accesses in arg1
        if (current->arg1 != NULL) {
            char* arrayName = NULL;
            char* indexStr = NULL;
            if (parseArrayAccess(current->arg1, &arrayName, &indexStr)) {
                // Check if we have a mapping for this array element
                VarValue* var = varTable;
                while (var != NULL) {
                    if (var->index != NULL &&
                        strcmp(var->varName, arrayName) == 0 &&
                        strcmp(var->index, indexStr) == 0) {
                        printf("Propagating constant: Replacing '%s' with '%s' in arg1\n", current->arg1, var->value);
                        safeStrReplace(&current->arg1, var->value);
                        changed = true;
                        break;
                    }
                    var = var->next;
                }
                free(arrayName);
                free(indexStr);
            }
        }

        // Replace array accesses in arg2
        if (current->arg2 != NULL) {
            char* arrayName = NULL;
            char* indexStr = NULL;
            if (parseArrayAccess(current->arg2, &arrayName, &indexStr)) {
                // Check if we have a mapping for this array element
                VarValue* var = varTable;
                while (var != NULL) {
                    if (var->index != NULL &&
                        strcmp(var->varName, arrayName) == 0 &&
                        strcmp(var->index, indexStr) == 0) {
                        printf("Propagating constant: Replacing '%s' with '%s' in arg2\n", current->arg2, var->value);
                        safeStrReplace(&current->arg2, var->value);
                        changed = true;
                        break;
                    }
                    var = var->next;
                }
                free(arrayName);
                free(indexStr);
            }
        }

        // Existing logic for operations
        // If the operation is '=', handle assignments
        if (current->op != NULL && strcmp(current->op, "=") == 0) {
                // Attempt to replace arg1 if it's a variable with a known constant value
                if (current->arg1 != NULL && !isNumericConstant(current->arg1) && !isBooleanConstant(current->arg1)) {
                    VarValue* var = varTable;
                    while (var != NULL) {
                        if (var->index == NULL && strcmp(current->arg1, var->varName) == 0) {
                            printf("Propagating constant: Replacing '%s' with '%s' in arg1\n", current->arg1, var->value);
                            safeStrReplace(&current->arg1, var->value);
                            changed = localChanged = true;
                            break;
                        }
                        var = var->next;
                    }
                }

                // Now, check if arg1 is a constant (after possible substitution)
                if (isNumericConstant(current->arg1) || isBooleanConstant(current->arg1)) {
                    // Map result variable to constant value
                    if (current->result != NULL && strlen(current->result) > 0) {
                        // Update or add the mapping
                        VarValue* var = varTable;
                        bool found = false;
                        while (var != NULL) {
                            if (var->index == NULL && strcmp(var->varName, current->result) == 0) {
                                // Update existing entry
                                free(var->value);
                                var->value = strdup(current->arg1);
                                found = true;
                                printf("Updated mapping: %s = %s\n", var->varName, var->value);
                                break;
                            }
                            var = var->next;
                        }
                        if (!found) {
                            // Add new entry to varTable
                            VarValue* newVar = malloc(sizeof(VarValue));
                            newVar->varName = strdup(current->result);
                            newVar->index = NULL;  // Scalar variable
                            newVar->value = strdup(current->arg1);
                            newVar->next = varTable;
                            varTable = newVar;
                            printf("Added mapping: %s = %s\n", newVar->varName, newVar->value);
                        }
                    } else {
                        // Invalid result, skip mapping
                        printf("Skipping mapping for instruction with invalid result.\n");
                    }
                } else {
                    // Handle assignment to a non-constant (invalidate any existing mapping)
                    if (current->result != NULL && strlen(current->result) > 0) {
                        // Remove existing mapping if any
                        VarValue** indirect = &varTable;
                        while (*indirect) {
                            if ((*indirect)->index == NULL && strcmp((*indirect)->varName, current->result) == 0) {
                                VarValue* temp = *indirect;
                                *indirect = (*indirect)->next;
                                free(temp->varName);
                                free(temp->value);
                                free(temp);
                                printf("Removed mapping for variable: %s (assigned non-constant)\n", current->result);
                                break;
                            }
                            indirect = &(*indirect)->next;
                        }
                    }
                }
            }
// Handle function calls
        else if (current->op != NULL && strcmp(current->op, "call") == 0) {
            // current->arg1: function name
            // current->result: variable receiving the return value

            // Check if we have a return mapping for this function
            FunctionReturn* funcRet = functionReturns;
            while (funcRet != NULL) {
                if (strcmp(funcRet->functionName, current->arg1) == 0) {
                    // Check if the return variable has a known constant value
                    VarValue* var = varTable;
                    while (var != NULL) {
                        if (strcmp(var->varName, funcRet->returnVar) == 0) {
                            // Replace the function call with an assignment
                            printf("Propagating constant: Replacing function call '%s' with value '%s'\n", current->arg1, var->value);
                            safeStrReplace(&current->op, "=");
                            safeStrReplace(&current->arg1, var->value);
                            safeStrReplace(&current->arg2, NULL);
                            changed = true;
                            break;
                        }
                        var = var->next;
                    }
                    break;
                }
                funcRet = funcRet->next;
            }
        }
        else {
                // For other operations, attempt to replace arg1 and arg2 with constants
                // Replace arg1 if it's a variable with a known constant value
                if (current->arg1 != NULL && !isNumericConstant(current->arg1) && !isBooleanConstant(current->arg1)) {
                    VarValue* var = varTable;
                    while (var != NULL) {
                        if (var->index == NULL && strcmp(current->arg1, var->varName) == 0) {
                            printf("Propagating constant: Replacing '%s' with '%s' in arg1\n", current->arg1, var->value);
                            safeStrReplace(&current->arg1, var->value);
                            changed = localChanged = true;
                            break;
                        }
                        var = var->next;
                    }
                }

                // Replace arg2 if it's a variable with a known constant value
                if (current->arg2 != NULL && !isNumericConstant(current->arg2) && !isBooleanConstant(current->arg2)) {
                    VarValue* var = varTable;
                    while (var != NULL) {
                        if (var->index == NULL && strcmp(current->arg2, var->varName) == 0) {
                            printf("Propagating constant: Replacing '%s' with '%s' in arg2\n", current->arg2, var->value);
                            safeStrReplace(&current->arg2, var->value);
                            changed = localChanged = true;
                            break;
                        }
                        var = var->next;
                    }
                }

                // Now, if the operation is a logical operation, and both arguments are constants, we can compute the result
                if ((strcmp(current->op, "!") == 0 || strcmp(current->op, "&&") == 0 || strcmp(current->op, "||") == 0) &&
                    (isBooleanConstant(current->arg1)) &&
                    (current->arg2 == NULL || isBooleanConstant(current->arg2))) {

                    bool left = strcmp(current->arg1, "true") == 0;
                    bool right = current->arg2 ? (strcmp(current->arg2, "true") == 0) : false;
                    bool result;

                    if (strcmp(current->op, "!") == 0) {
                        result = !left;
                    } else if (strcmp(current->op, "&&") == 0) {
                        result = left && right;
                    } else if (strcmp(current->op, "||") == 0) {
                        result = left || right;
                    } else {
                        // Unsupported logical operator
                        current = current->next;
                        continue;
                    }

                    // Replace the operation with the resulting constant
                    safeStrReplace(&current->arg1, result ? "true" : "false");
                    safeStrReplace(&current->op, "=");
                    safeStrReplace(&current->arg2, NULL);
                    changed = localChanged = true;

                    // Map the result variable to the constant value
                    if (current->result != NULL && strlen(current->result) > 0) {
                        // Update or add the mapping
                        VarValue* var = varTable;
                        bool found = false;
                        while (var != NULL) {
                            if (var->index == NULL && strcmp(var->varName, current->result) == 0) {
                                // Update existing entry
                                free(var->value);
                                var->value = strdup(current->arg1);
                                found = true;
                                printf("Updated mapping: %s = %s\n", var->varName, var->value);
                                break;
                            }
                            var = var->next;
                        }
                        if (!found) {
                            // Add new entry to varTable
                            VarValue* newVar = malloc(sizeof(VarValue));
                            newVar->varName = strdup(current->result);
                            newVar->index = NULL;  // Scalar variable
                            newVar->value = strdup(current->arg1);
                            newVar->next = varTable;
                            varTable = newVar;
                            printf("Added mapping: %s = %s\n", newVar->varName, newVar->value);
                        }
                    }
                }

                // Invalidate mapping if the operation assigns to a variable
                if (current->result != NULL && strlen(current->result) > 0) {
                    VarValue** indirect = &varTable;
                    while (*indirect) {
                        if ((*indirect)->index == NULL && strcmp((*indirect)->varName, current->result) == 0) {
                            VarValue* temp = *indirect;
                            *indirect = (*indirect)->next;
                            free(temp->varName);
                            free(temp->value);
                            free(temp);
                            printf("Removed mapping for variable: %s (modified by operation)\n", current->result);
                            break;
                        }
                        indirect = &(*indirect)->next;
                    }
                }
            }

            current = current->next;
        }

    } while (localChanged);

    // Properly free varTable entries
    VarValue* var = varTable;
    while (var != NULL) {
        VarValue* temp = var;
        var = var->next;
        free(temp->varName);
        if (temp->index) free(temp->index);
        free(temp->value);
        free(temp);
    }

    return changed;
}




void optimizeTAC(TAC** head) {
    bool changed;
    int passCount = 0;

    do {
        changed = false;  // Reset flag before each pass
        printf("=== Optimization Pass %d ===\n", passCount++);

        // Apply constant propagation multiple times until no changes
        while (constantPropagation(head)) {
            changed = true;
            printf("Constant propagation applied\n");
        }

        // Apply constant folding multiple times until no changes
        while (constantFolding(head)) {
            changed = true;
            printf("Constant folding applied\n");
        }

        // Apply dead code elimination
        if (deadCodeElimination(head, false)) {
            changed = true;
            printf("Dead Code Elimination applied\n");
        }
        

    } while (changed);  // Repeat until no more changes are made

    deadCodeEliminationFinal(head, false);

    printOptimizedTAC("TACOptimized.ir", *head);
    printf("Optimized TAC written to TACOptimized.ir\n");

    // Free all remaining memory by performing dead code elimination with freeAll=true
    //deadCodeElimination(head, true);
}


void printOptimizedTAC(const char* filename, TAC* head) {
    FILE* outputFile = fopen(filename, "w");
    if (outputFile == NULL) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    TAC* current = head;
    while (current != NULL) {
        // Skip empty TAC nodes
        if (current->result == NULL && current->arg1 == NULL && current->op == NULL && current->arg2 == NULL) {
            current = current->next;
            continue;
        }

        // Handle 'write' operation
        if (current->op != NULL && strcmp(current->op, "write") == 0) {
            fprintf(outputFile, "%s %s\n", current->op, current->arg1);  // Write operation is special case
        }
        // Handle array element assignments
        else if (current->op != NULL && strcmp(current->op, "[]=") == 0) {
            fprintf(outputFile, "%s[%s] = %s\n", current->result, current->arg1, current->arg2);
        }
        // Handle standard operations (result = arg1 op arg2)
        else if (current->op != NULL && strcmp(current->op, "=") == 0) {
            fprintf(outputFile, "%s = %s\n", current->result, current->arg1);
        }
        else {
            // Print normal operations
            if (current->result != NULL) {
                fprintf(outputFile, "%s = ", current->result);
            }
            if (current->arg1 != NULL) {
                fprintf(outputFile, "%s", current->arg1);
            }
            if (current->op != NULL && strcmp(current->op, "=") != 0) {
                fprintf(outputFile, " %s", current->op);
            }
            if (current->arg2 != NULL) {
                fprintf(outputFile, " %s", current->arg2);
            }
            fprintf(outputFile, "\n");
        }

        current = current->next;
    }

    fclose(outputFile);
}

TAC* createTAC(const char* result, const char* arg1, const char* op, const char* arg2) {
    TAC* tac = (TAC*)malloc(sizeof(TAC));
    if (tac == NULL) {
        perror("Failed to allocate memory for TAC");
        exit(EXIT_FAILURE);
    }
    addToFreeList(tac);

    tac->result = result ? strdup(result) : NULL;
    if (tac->result) addToFreeList(tac->result);

    tac->arg1 = arg1 ? strdup(arg1) : NULL;
    if (tac->arg1) addToFreeList(tac->arg1);

    tac->op = op ? strdup(op) : NULL;
    if (tac->op) addToFreeList(tac->op);

    tac->arg2 = arg2 ? strdup(arg2) : NULL;
    if (tac->arg2) addToFreeList(tac->arg2);

    tac->next = NULL;
    return tac;
}



void freeTACList(TAC* head) {
    TAC* current = head;
    while (current != NULL) {
        TAC* next = current->next;
        printf("Freeing TAC node: %p\n", (void*)current);  // Debug line
        if (current->result) free(current->result);
        if (current->arg1) free(current->arg1);
        if (current->op) free(current->op);
        if (current->arg2) free(current->arg2);
        free(current);
        current = next;
    }
}