    #include "optimizer.h"
    #include <stdbool.h>
    #include <ctype.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdint.h>


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
    if (*target && *target != source) {  // Only free if target is different from source
        free(*target);  
    }
    *target = source ? strdup(source) : NULL;  
    if (*target == NULL && source != NULL) {
        perror("Failed to allocate memory for string duplication");
        exit(EXIT_FAILURE);
    }
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

        // Handle integer operations
        if (current->arg1 && current->arg2 &&
            (uintptr_t)current->arg1 != 0xbebebebebebebebeULL &&
            (uintptr_t)current->arg2 != 0xbebebebebebebebeULL &&
            isConstant(current->arg1) && isConstant(current->arg2)) {

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
        }
        // Handle floating-point operations
        else if (current->arg1 && current->arg2 &&
                 (uintptr_t)current->arg1 != 0xbebebebebebebebeULL &&
                 (uintptr_t)current->arg2 != 0xbebebebebebebebeULL &&
                 isFloatConstant(current->arg1) && isFloatConstant(current->arg2)) {
            float left = atof(current->arg1);
            float right = atof(current->arg2);
            float result = 0;

            if (strcmp(current->op, "add.s") == 0) {
                result = left + right;
            }
            else if (strcmp(current->op, "sub.s") == 0) {
                result = left - right;
            }
            else if (strcmp(current->op, "mul.s") == 0) {
                result = left * right;
            }
            else if (strcmp(current->op, "div.s") == 0 && right != 0.0f) {
                result = left / right;
            } else {
                fprintf(stderr, "Error: Division by zero encountered in constant folding (floats).\n");
            }

            foldFloatOperation(current, result);
            changed = true;
        }

        // Identity operation folding for integers, if arg2 is valid
        if (current->arg2 && (uintptr_t)current->arg2 != 0xbebebebebebebebeULL && isConstant(current->arg2)) {
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
            if (strcmp(current->op, "mul.s") == 0 && atof(current->arg2) == 1.0f) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
            else if (strcmp(current->op, "add.s") == 0 && atof(current->arg2) == 0.0f) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
            else if (strcmp(current->op, "sub.s") == 0 && atof(current->arg2) == 0.0f) {
                safeStrReplace(&current->op, "=");
                safeStrReplace(&current->arg2, NULL);
                changed = true;
            }
            else if (strcmp(current->op, "div.s") == 0 && atof(current->arg2) == 1.0f) {
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
   snprintf(resultStr, sizeof(resultStr), "%.6f", result);  // Control precision to 6 decimal places
   printf("Folding float: %s %s %s -> %s\n", current->arg1, current->op, current->arg2, resultStr);
   safeStrReplace(&current->arg1, resultStr);  // Replace arg1 with result string
   safeStrReplace(&current->op, "=");          // Replace operation with assignment
   safeStrReplace(&current->arg2, NULL);       // Clear arg2
}


// Utility function to check if a string is a valid float constant
bool isFloatConstant(const char* str) {
    if (str == NULL || (uintptr_t)str == 0xbebebebebebebebeULL || *str == '\0') {
        return false;
    }

    char* endptr;
    strtod(str, &endptr);
    return (*endptr == '\0');  // Check if entire string is a valid float
}






bool constantPropagation(TAC** head) {
    TAC* current = *head;
    bool changed = false;

    typedef struct VarValue {
        char* varName;
        char* value;
        struct VarValue* next;
    } VarValue;

    VarValue* varTable = NULL;

    while (current != NULL) {
        // Skip 'write' operations entirely
        if (current->op != NULL && strcmp(current->op, "write") == 0) {
            current = current->next;
            continue;
        }

        // Handle array accesses
        if (current->op != NULL && strcmp(current->op, "[]") == 0) {
            // Propagate constants for array indices
            if (isConstant(current->arg2)) {
                safeStrReplace(&current->arg2, current->arg2);
                changed = true;
            }
        }

        // If the operation is '=', handle assignments
        if (current->op != NULL && strcmp(current->op, "=") == 0) {
            // Attempt to replace arg1 if it's a variable with a known constant value
            if (current->arg1 != NULL && !isConstant(current->arg1)) {
                VarValue* var = varTable;
                while (var != NULL) {
                    if (strcmp(current->arg1, var->varName) == 0) {
                        printf("Propagating constant: Replacing '%s' with '%s' in arg1\n", current->arg1, var->value);
                        safeStrReplace(&current->arg1, var->value);
                        changed = true;
                        break;
                    }
                    var = var->next;
                }
            }

            // Now, check if arg1 is a constant (after possible substitution)
            if (isConstant(current->arg1)) {
                // Map result variable to constant value
                if (current->result != NULL && strlen(current->result) > 0) {
                    // Update or add the mapping
                    VarValue* var = varTable;
                    bool found = false;
                    while (var != NULL) {
                        if (strcmp(var->varName, current->result) == 0) {
                            // Update existing entry
                            free(var->value);
                            var->value = strdup(current->arg1);
                            if (var->value == NULL) {
                                perror("Failed to duplicate string in varTable");
                                exit(EXIT_FAILURE);
                            }
                            found = true;
                            printf("Updated mapping: %s = %s\n", var->varName, var->value);
                            break;
                        }
                        var = var->next;
                    }
                    if (!found) {
                        // Add new entry to varTable
                        VarValue* newVar = malloc(sizeof(VarValue));
                        if (newVar == NULL) {
                            perror("Failed to allocate memory for VarValue");
                            exit(EXIT_FAILURE);
                        }
                        newVar->varName = strdup(current->result);
                        newVar->value = strdup(current->arg1);
                        if (newVar->varName == NULL || newVar->value == NULL) {
                            perror("Failed to duplicate string for new VarValue");
                            exit(EXIT_FAILURE);
                        }
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
                        if (strcmp((*indirect)->varName, current->result) == 0) {
                            VarValue* temp = *indirect;
                            *indirect = (*indirect)->next;
                            addToFreeList(temp->varName);
                            addToFreeList(temp->value);
                            addToFreeList(temp);
                            printf("Removed mapping for variable: %s (assigned non-constant)\n", current->result);
                            break;
                        }
                        indirect = &(*indirect)->next;
                    }
                }
            }
        } else {
            // For operations other than '=', attempt to replace arg1 and arg2 with constants if possible

            // Replace arg1 if it's a variable with a known constant value
            if (current->arg1 != NULL && !isConstant(current->arg1)) {
                VarValue* var = varTable;
                while (var != NULL) {
                    if (strcmp(current->arg1, var->varName) == 0) {
                        printf("Propagating constant: Replacing '%s' with '%s' in arg1\n", current->arg1, var->value);
                        safeStrReplace(&current->arg1, var->value);
                        changed = true;
                        break;
                    }
                    var = var->next;
                }
            }

            // Replace arg2 if it's a variable with a known constant value
            if (current->arg2 != NULL && !isConstant(current->arg2)) {
                VarValue* var = varTable;
                while (var != NULL) {
                    if (strcmp(current->arg2, var->varName) == 0) {
                        printf("Propagating constant: Replacing '%s' with '%s' in arg2\n", current->arg2, var->value);
                        safeStrReplace(&current->arg2, var->value);
                        changed = true;
                        break;
                    }
                    var = var->next;
                }
            }
        }

        current = current->next;
    }

    // Properly free varTable entries using removeFromFreeList instead of addToFreeList
    VarValue* var = varTable;
    while (var != NULL) {
        VarValue* temp = var;
        var = var->next;
        removeFromFreeList(temp->varName);
        removeFromFreeList(temp->value);
        removeFromFreeList(temp);
    }

    return changed;
}




void optimizeTAC(TAC** head) {
    bool changed;
    int passCount = 0;

    printf("=== CODE OPTIMIZATION ===\n");

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



    printOptimizedTAC("TACOptimized.ir", *head);
    printf("Optimized TAC written to TACOptimized.ir\n");

    // Free all remaining memory by performing dead code elimination with freeAll=true
    //deadCodeElimination(head, true);
}





bool isConstant(const char* str) {
    if (str == NULL || (uintptr_t)str == 0xbebebebebebebebeULL || *str == '\0') {
        return false;
    }

    // Optional: Handle negative numbers
    if (*str == '-') {
        ++str;
    }

    // Check if the remaining string is numeric
    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            return false;
        }
        ++str;
    }

    return true;
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

// Use this function to create TAC entries with dynamically allocated strings
TAC* createTAC(const char* result, const char* arg1, const char* op, const char* arg2) {
    TAC* tac = (TAC*)malloc(sizeof(TAC));
    if (tac == NULL) {
        perror("Failed to allocate memory for TAC");
        exit(EXIT_FAILURE);
    }
    tac->result = result ? strdup(result) : NULL;
    tac->arg1 = arg1 ? strdup(arg1) : NULL;
    tac->op = op ? strdup(op) : NULL;
    tac->arg2 = arg2 ? strdup(arg2) : NULL;
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