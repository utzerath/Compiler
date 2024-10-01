#include "optimizer.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void safeStrReplace(char** target, const char* source) {
    if (source != NULL) {
        char* newStr = strdup(source);
        if (newStr == NULL) {
            perror("Failed to allocate memory for string duplication");
            exit(EXIT_FAILURE);
        }
        *target = newStr;  // Assign the new string
    } else {
        *target = NULL;  // Set target to NULL if source is NULL
    }
}

bool constantFolding(TAC** head) {
    TAC* current = *head;
    bool changed = false;

    while (current != NULL) {
        // Debug: Print TAC info before processing
        printf("Folding TAC: result='%s', arg1='%s', op='%s', arg2='%s'\n",
               current->result ? current->result : "NULL",
               current->arg1 ? current->arg1 : "NULL",
               current->op ? current->op : "NULL",
               current->arg2 ? current->arg2 : "NULL");

        if (current->op != NULL && strcmp(current->op, "+") == 0 &&
            isConstant(current->arg1) && isConstant(current->arg2)) {
            int left = atoi(current->arg1);
            int right = atoi(current->arg2);
            int result = left + right;

            // Replace the operation with the result
            char resultStr[20];
            sprintf(resultStr, "%d", result);
            safeStrReplace(&current->arg1, resultStr);  // Replace with the result string
            safeStrReplace(&current->op, "=");          // Properly replace 'op'
            safeStrReplace(&current->arg2, NULL);       // Clear arg2
            changed = true;

            // Debug: Confirm folding worked
            printf("Folding applied: %s = %d\n", current->result, result);
        }

        current = current->next;
    }

    return changed;
}

bool constantPropagation(TAC** head) {
    TAC* current = *head;
    bool changed = false;

    // Map to keep track of variable values
    typedef struct VarValue {
        char* varName;
        char* value;
        struct VarValue* next;
    } VarValue;

    VarValue* varTable = NULL;

    while (current != NULL) {
        // Skip write statements entirely
        if (current->op != NULL && strcmp(current->op, "write") == 0) {
            current = current->next;
            continue;
        }
        // Debug: Print TAC info before processing
        printf("Propagating TAC: result='%s', arg1='%s', op='%s', arg2='%s'\n",
               current->result ? current->result : "NULL",
               current->arg1 ? current->arg1 : "NULL",
               current->op ? current->op : "NULL",
               current->arg2 ? current->arg2 : "NULL");

        // If the operation is '=', and arg1 is a constant
        if (current->op != NULL && strcmp(current->op, "=") == 0 && isConstant(current->arg1)) {
            // Store the variable and its constant value
            VarValue* newVar = malloc(sizeof(VarValue));
            newVar->varName = strdup(current->result);
            newVar->value = strdup(current->arg1);
            newVar->next = varTable;
            varTable = newVar;
        } else {
            // Replace arg1 if it's a variable with a known constant value
            if (current->arg1 != NULL && !isConstant(current->arg1)) {
                VarValue* var = varTable;
                while (var != NULL) {
                    if (strcmp(current->arg1, var->varName) == 0) {
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

    // Free the varTable
    VarValue* var = varTable;
    while (var != NULL) {
        VarValue* temp = var;
        var = var->next;
        free(temp->varName);
        free(temp->value);
        free(temp);
    }

    return changed;
}

// Main optimization loop
void optimizeTAC(TAC** head) {
    bool changed;
    int passCount = 0;

    do {
        changed = false;  // Reset flag before each pass
        passCount++;
        printf("=== Optimization Pass %d ===\n", passCount);

        // Apply constant propagation
        if (constantPropagation(head)) {
            changed = true;  // Only mark changed if constantPropagation made changes
            printf("Constant propagation applied\n");
        }

        // Apply constant folding (for addition only)
        if (constantFolding(head)) {
            changed = true;  // Only mark changed if constantFolding made changes
            printf("Constant folding applied\n");
        }

    } while (changed);  // Repeat until no more changes are made

    printOptimizedTAC("TACOptimized.ir", *head);
    printf("Optimized TAC written to TACOptimized.ir\n");


}

// Utility function to check if a string is a valid constant
bool isConstant(const char* str) {
    if (str == NULL || *str == '\0') {
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

        // Free all dynamically allocated strings
        if (current->result) free(current->result);
        if (current->arg1) free(current->arg1);
        if (current->op) free(current->op);
        if (current->arg2) free(current->arg2);

        // Free the TAC node itself
        free(current);

        current = next;
    }
}
