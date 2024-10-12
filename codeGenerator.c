#include "codeGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  // For isdigit
#include <string.h> // For strlen, strcmp, strdup

#define VAR_PREFIX "var_"  // Prefix for variable names in assembly

int stackOffset = 0;  // Initialize at the start of the program
static FILE* outputFile;

// Array of temporary registers (defined in codeGenerator.h)
MIPSRegister tempRegisters[NUM_TEMP_REGISTERS] = {
    {"$t0", false, false, NULL},
    {"$t1", false, false, NULL},
    {"$t2", false, false, NULL},
    {"$t3", false, false, NULL},
    {"$t4", false, false, NULL},
    {"$t5", false, false, NULL},
    {"$t6", false, false, NULL},
    {"$t7", false, false, NULL},
    {"$t8", false, false, NULL},
    {"$t9", false, false, NULL}
};


// List to track variable declarations dynamically
VarList* declaredVariables = NULL;

// Function implementations

// Declare variables dynamically from the TAC
void declareVariablesFromTAC(TAC* tacInstructions) {
    TAC* current = tacInstructions;
    while (current != NULL) {
        // For each TAC instruction, check for variables in result, arg1, and arg2
        if (current->result != NULL && !isTemporaryVariable(current->result)) {
            declareVariable(current->result);  // Declare the result variable
        }
        if (current->arg1 != NULL && !isImmediate(current->arg1) && !isTemporaryVariable(current->arg1)) {
            declareVariable(current->arg1);  // Declare the first argument variable
        }
        if (current->arg2 != NULL && !isImmediate(current->arg2) && !isTemporaryVariable(current->arg2)) {
            declareVariable(current->arg2);  // Declare the second argument variable
        }
        current = current->next;  // Move to the next TAC instruction
    }

    // Once all variables are added to the declaredVariables list, generate the .data section
    generateDataSection();
}

// Add variable to the list of declared variables
void declareVariable(char* varName) {
    if (!isVariableDeclared(varName)) {
        VarList* newVar = (VarList*)malloc(sizeof(VarList));
        newVar->varName = strdup(varName);  // Copy the variable name
        newVar->next = declaredVariables;
        declaredVariables = newVar;
    }
}

// Check if the variable has already been declared
bool isVariableDeclared(char* varName) {
    VarList* current = declaredVariables;
    while (current != NULL) {
        if (strcmp(current->varName, varName) == 0) {
            return true;  // The variable has already been declared
        }
        current = current->next;
    }
    return false;
}

// Generate the .data section for all declared variables
void generateDataSection() {
    fprintf(outputFile, ".data\n");
    VarList* current = declaredVariables;
    while (current != NULL) {
        fprintf(outputFile, "%s%s: .word 0\n", VAR_PREFIX, current->varName);  // Declare variable with initial value 0
        current = current->next;
    }
}

// Initialize code generator
void initCodeGenerator(const char* outputFilename) {
    outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }
}

// Generate MIPS code from TAC
void generateMIPS(TAC* tacInstructions) {
    TAC* current = tacInstructions;

    // Declare variables in memory
    declareVariablesFromTAC(tacInstructions);

    // Start the text section and main label
    fprintf(outputFile, ".text\n.globl main\nmain:\n");

    while (current != NULL) {
        // Assignment operation (e.g., x = y)
        if (strcmp(current->op, "=") == 0) {
            int regResult;

            // Handle immediate value
            if (isImmediate(current->arg1)) {
                regResult = allocateRegisterForVariable(current->result);
                if (regResult == -1) {
                    fprintf(stderr, "Register allocation failed\n");
                    return;
                }
                fprintf(outputFile, "\tli %s, %s\n", tempRegisters[regResult].name, current->arg1);
            } else {
                // Get or load arg1 into a register
                int regArg1 = getRegisterForVariable(current->arg1);
                if (regArg1 == -1) {
                    regArg1 = allocateRegisterForVariable(current->arg1);
                    if (regArg1 == -1) {
                        fprintf(stderr, "Register allocation failed\n");
                        return;
                    }
                    fprintf(outputFile, "\tlw %s, %s%s\n", tempRegisters[regArg1].name, VAR_PREFIX, current->arg1);
                }
                regResult = allocateRegisterForVariable(current->result);
                if (regResult == -1) {
                    fprintf(stderr, "Register allocation failed\n");
                    return;
                }
                fprintf(outputFile, "\tmove %s, %s\n", tempRegisters[regResult].name, tempRegisters[regArg1].name);
            }

            // Store the result into memory if it's a variable
            if (!isTemporaryVariable(current->result)) {
                fprintf(outputFile, "\tsw %s, %s%s\n", tempRegisters[regResult].name, VAR_PREFIX, current->result);
            }

            // Deallocate temporary variables
            if (isTemporaryVariable(current->arg1)) {
                deallocateRegister(regResult);
            }
        }
        // Addition operation (e.g., x = y + z)
        else if (strcmp(current->op, "+") == 0) {
            int regArg1, regArg2, regResult;

            // Handle arg1
            if (isImmediate(current->arg1)) {
                regArg1 = allocateRegister();
                if (regArg1 == -1) {
                    fprintf(stderr, "Register allocation failed\n");
                    return;
                }
                fprintf(outputFile, "\tli %s, %s\n", tempRegisters[regArg1].name, current->arg1);
            } else {
                regArg1 = getRegisterForVariable(current->arg1);
                if (regArg1 == -1) {
                    regArg1 = allocateRegisterForVariable(current->arg1);
                    if (regArg1 == -1) {
                        fprintf(stderr, "Register allocation failed\n");
                        return;
                    }
                    fprintf(outputFile, "\tlw %s, %s%s\n", tempRegisters[regArg1].name, VAR_PREFIX, current->arg1);
                }
            }

            // Handle arg2
            if (isImmediate(current->arg2)) {
                regArg2 = allocateRegister();
                if (regArg2 == -1) {
                    fprintf(stderr, "Register allocation failed\n");
                    return;
                }
                fprintf(outputFile, "\tli %s, %s\n", tempRegisters[regArg2].name, current->arg2);
            } else {
                regArg2 = getRegisterForVariable(current->arg2);
                if (regArg2 == -1) {
                    regArg2 = allocateRegisterForVariable(current->arg2);
                    if (regArg2 == -1) {
                        fprintf(stderr, "Register allocation failed\n");
                        return;
                    }
                    fprintf(outputFile, "\tlw %s, %s%s\n", tempRegisters[regArg2].name, VAR_PREFIX, current->arg2);
                }
            }

            // Perform the addition
            regResult = allocateRegisterForVariable(current->result);
            if (regResult == -1) {
                fprintf(stderr, "Register allocation failed\n");
                return;
            }
            fprintf(outputFile, "\tadd %s, %s, %s\n", tempRegisters[regResult].name, tempRegisters[regArg1].name, tempRegisters[regArg2].name);

            // Store the result if necessary
            if (!isTemporaryVariable(current->result)) {
                fprintf(outputFile, "\tsw %s, %s%s\n", tempRegisters[regResult].name, VAR_PREFIX, current->result);
            }

            // Deallocate temporary variables
            if (isTemporaryVariable(current->arg1)) {
                deallocateRegister(regArg1);
            }
            if (isTemporaryVariable(current->arg2)) {
                deallocateRegister(regArg2);
            }
            if (isTemporaryVariable(current->result)) {
                deallocateRegister(regResult);
            }
        }
        else if (strcmp(current->op, "write") == 0) {
            fprintf(outputFile, "\tli $v0, 1\n");  // Load syscall code for print integer
            // Handle immediate value or variable
            if (!isImmediate(current->arg1)) {
                int regArg1 = getRegisterForVariable(current->arg1);
                if (regArg1 == -1) {
                    regArg1 = allocateRegisterForVariable(current->arg1);
                    if (regArg1 == -1) {
                        fprintf(stderr, "Register allocation failed for write\n");
                        return;
                    }
                    // Load variable from memory into a register
                    fprintf(outputFile, "\tlw %s, %s%s\n", tempRegisters[regArg1].name, VAR_PREFIX, current->arg1);
                }
                // Move the value to $a0 for the syscall
                fprintf(outputFile, "\tmove $a0, %s\n", tempRegisters[regArg1].name);
            } else {
                // If it's an immediate value, load it directly into $a0
                fprintf(outputFile, "\tli $a0, %s\n", current->arg1);  // Use arg1, not result
            }
            
            // Print the integer
            fprintf(outputFile, "\tsyscall\n");

            // Add newline character
            fprintf(outputFile, "\tli $v0, 11\n");  // Syscall for print character
            fprintf(outputFile, "\tli $a0, 10\n");  // Newline character (ASCII 10)
            fprintf(outputFile, "\tsyscall\n");     // Print the newline
        }



        // Deallocate temporary variables
        if (isTemporaryVariable(current->result)) {
            int regIndex = getRegisterForVariable(current->result);
            if (regIndex != -1) {
                deallocateRegister(regIndex);
            }
        }

        current = current->next;
    }

    // Add program termination syscall
    fprintf(outputFile, "\tli $v0, 10\n\tsyscall\n");
}



// Check if a variable is a temporary variable
bool isTemporaryVariable(char* varName) {
    return (varName != NULL && varName[0] == 't' && isdigit(varName[1]));
}

// Finalize the code generator by closing the output file
void finalizeCodeGenerator(const char* outputFilename) {
    if (outputFile) {
        fclose(outputFile);
        printf("MIPS code generated and saved to file %s\n", outputFilename);
        outputFile = NULL;
    }
}

// Helper function: Check if an operand is an immediate value
bool isImmediate(char* operand) {
    if (operand == NULL) return false;
    for (int i = 0; i < strlen(operand); i++) {
        if (!isdigit(operand[i])) {
            return false;
        }
    }
    return true;
}

// Register allocation logic
int allocateRegister() {
    for (int i = 0; i < NUM_TEMP_REGISTERS; i++) {
        if (!tempRegisters[i].inUse && !tempRegisters[i].spilled) {
            tempRegisters[i].inUse = true;
            tempRegisters[i].varName = NULL;  // Initialize varName
            return i; // Return the register index
        }
    }
    // All registers are in use; need to spill one
    int regToSpill = pickRegisterToSpill(); 
    spillRegister(regToSpill);
    return regToSpill;
}

// Spill a register to memory
void spillRegister(int regIndex) {
    // Adjust stack pointer to allocate space
    fprintf(outputFile, "\taddi $sp, $sp, -4\n");
    // Store the current register value to the stack
    fprintf(outputFile, "\tsw %s, 0($sp)\n", tempRegisters[regIndex].name);
    tempRegisters[regIndex].spilled = true;
    tempRegisters[regIndex].inUse = false;
}

// Restore a spilled register from memory
void restoreRegister(int regIndex) {
    if (tempRegisters[regIndex].spilled) {
        // Load the value from the stack
        fprintf(outputFile, "\tlw %s, 0($sp)\n", tempRegisters[regIndex].name);
        // Adjust stack pointer to deallocate space
        fprintf(outputFile, "\taddi $sp, $sp, 4\n");
        tempRegisters[regIndex].spilled = false;
        tempRegisters[regIndex].inUse = true;
    }
}

// Pick a register to spill (simple strategy: pick the first in-use register)
int pickRegisterToSpill() {
    for (int i = 0; i < NUM_TEMP_REGISTERS; i++) {
        if (tempRegisters[i].inUse) {
            return i;
        }
    }
    fprintf(stderr, "Error: No register available for spilling.\n");
    exit(EXIT_FAILURE);
}

// Deallocate a register
void deallocateRegister(int regIndex) {
    if (regIndex >= 0 && regIndex < NUM_TEMP_REGISTERS) {
        tempRegisters[regIndex].inUse = false;
        if (tempRegisters[regIndex].varName != NULL) {
            free(tempRegisters[regIndex].varName);
            tempRegisters[regIndex].varName = NULL;
        }
    }
}

// Get the register index for a variable
int getRegisterForVariable(char* varName) {
    for (int i = 0; i < NUM_TEMP_REGISTERS; i++) {
        if (tempRegisters[i].varName != NULL && strcmp(tempRegisters[i].varName, varName) == 0) {
            if (tempRegisters[i].inUse) {
                return i;  // Register is in use
            } else if (tempRegisters[i].spilled) {
                restoreRegister(i);
                return i;
            }
        }
    }
    return -1; // Variable not found in any register
}

// Allocate a register for a variable
int allocateRegisterForVariable(char* varName) {
    int regIndex = allocateRegister();
    if (regIndex != -1) {
        if (tempRegisters[regIndex].varName != NULL) {
            free(tempRegisters[regIndex].varName);
        }
        tempRegisters[regIndex].varName = strdup(varName);
        tempRegisters[regIndex].inUse = true;
    }
    return regIndex;
}

// Print the current TAC instruction (for debugging)
void printCurrentTAC(TAC* tac) {
    if (tac != NULL) {
        printf("\n--- CURRENT TAC %s ---\n", tac->op);
        if (strcmp(tac->op, "=") == 0) {
            printf("%s = %s\n", tac->result, tac->arg1);
        } else {
            if (tac->result != NULL) printf("%s = ", tac->result);
            if (tac->arg1 != NULL) printf("%s ", tac->arg1);
            if (tac->op != NULL) printf("%s ", tac->op);
            if (tac->arg2 != NULL) printf("%s ", tac->arg2);
            printf("\n");
        }
    }
}
