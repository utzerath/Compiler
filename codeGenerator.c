// codeGenerator.c
#include "codeGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  // For isdigit
#include <string.h> // For strlen, strcmp, strdup>

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

// Helper functions to process array accesses
bool isArrayAccess(char* operand);
void parseArrayAccess(char* operand, char** arrayName, char** indexExpr);
void generateCodeForArrayAccess(char* arrayName, char* indexExpr, char* reg);

// Function implementations

// Declare variables dynamically from the TAC
void declareVariablesFromTAC(TAC* tacInstructions) {
    TAC* current = tacInstructions;
    while (current != NULL) {
        // For each TAC instruction, check for variables in result, arg1, and arg2
        if (current->result != NULL && !isTemporaryVariable(current->result)) {
            // Handle array declarations
            if (isArrayAccess(current->result)) {
                char* arrayName = NULL;
                char* indexExpr = NULL;
                parseArrayAccess(current->result, &arrayName, &indexExpr);
                declareVariable(arrayName);
                free(arrayName);
                free(indexExpr);
            } else {
                declareVariable(current->result);  // Declare the result variable
            }
        }
        if (current->arg1 != NULL && !isImmediate(current->arg1) && !isTemporaryVariable(current->arg1)) {
            if (isArrayAccess(current->arg1)) {
                char* arrayName = NULL;
                char* indexExpr = NULL;
                parseArrayAccess(current->arg1, &arrayName, &indexExpr);
                declareVariable(arrayName);
                free(arrayName);
                free(indexExpr);
            } else {
                declareVariable(current->arg1);  // Declare the first argument variable
            }
        }
        if (current->arg2 != NULL && !isImmediate(current->arg2) && !isTemporaryVariable(current->arg2)) {
            if (isArrayAccess(current->arg2)) {
                char* arrayName = NULL;
                char* indexExpr = NULL;
                parseArrayAccess(current->arg2, &arrayName, &indexExpr);
                declareVariable(arrayName);
                free(arrayName);
                free(indexExpr);
            } else {
                declareVariable(current->arg2);  // Declare the second argument variable
            }
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
        fprintf(outputFile, "%s%s: .space 40\n", VAR_PREFIX, current->varName);  // Adjust space based on array size (assuming max size 10 elements)
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
        printCurrentTAC(current);  // For debugging purposes

        if (strcmp(current->op, "=") == 0) {
            // Handle assignments
            if (isArrayAccess(current->result)) {
                // Assignment to array element: arr[index] = value
                char* arrayName = NULL;
                char* indexExpr = NULL;
                parseArrayAccess(current->result, &arrayName, &indexExpr);

                // Generate code to compute the address of arr[index]
                char addressReg[5] = "$a0";  // Use $a0 as temporary register for address

                generateCodeForArrayAccess(arrayName, indexExpr, addressReg);

                // Generate code to load the value into a register
                int regValue;
                if (isImmediate(current->arg1)) {
                    regValue = allocateRegister();
                    if (regValue == -1) {
                        fprintf(stderr, "Register allocation failed\n");
                        return;
                    }
                    fprintf(outputFile, "\tli %s, %s\n", tempRegisters[regValue].name, current->arg1);
                } else if (isTemporaryVariable(current->arg1)) {
                    regValue = getRegisterForVariable(current->arg1);
                } else {
                    regValue = allocateRegister();
                    fprintf(outputFile, "\tlw %s, %s%s\n", tempRegisters[regValue].name, VAR_PREFIX, current->arg1);
                }

                // Store the value into the array element
                fprintf(outputFile, "\tsw %s, 0(%s)\n", tempRegisters[regValue].name, addressReg);

                // Deallocate registers and free memory
                deallocateRegister(regValue);
                free(arrayName);
                free(indexExpr);
            } else {
                // Assignment to variable: var = value
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
        }
        // Handle arithmetic operations
        else if (strcmp(current->op, "+") == 0 || strcmp(current->op, "-") == 0 ||
                 strcmp(current->op, "*") == 0 || strcmp(current->op, "/") == 0) {
            // Similar handling as before, include support for array accesses in operands
            int regArg1 = getOperandRegister(current->arg1);
            int regArg2 = getOperandRegister(current->arg2);

            int regResult = allocateRegisterForVariable(current->result);
            if (regResult == -1) {
                fprintf(stderr, "Register allocation failed\n");
                return;
            }

            // Generate the arithmetic instruction
            char* mipsOp = getMipsOperator(current->op);
            fprintf(outputFile, "\t%s %s, %s, %s\n", mipsOp, tempRegisters[regResult].name, tempRegisters[regArg1].name, tempRegisters[regArg2].name);

            // Store the result if necessary
            if (!isTemporaryVariable(current->result)) {
                fprintf(outputFile, "\tsw %s, %s%s\n", tempRegisters[regResult].name, VAR_PREFIX, current->result);
            }

            // Deallocate temporary variables
            deallocateRegister(regArg1);
            deallocateRegister(regArg2);
            if (isTemporaryVariable(current->result)) {
                deallocateRegister(regResult);
            }
        }
        else if (strcmp(current->op, "write") == 0) {
            // Handle write operation
            fprintf(outputFile, "\tli $v0, 1\n");  // Load syscall code for print integer
            int regArg1 = getOperandRegister(current->arg1);

            // Move the value to $a0 for the syscall
            fprintf(outputFile, "\tmove $a0, %s\n", tempRegisters[regArg1].name);

            // Print the integer
            fprintf(outputFile, "\tsyscall\n");

            // Add newline character
            fprintf(outputFile, "\tli $v0, 11\n");  // Syscall for print character
            fprintf(outputFile, "\tli $a0, 10\n");  // Newline character (ASCII 10)
            fprintf(outputFile, "\tsyscall\n");     // Print the newline

            // Deallocate register
            deallocateRegister(regArg1);
        }

        current = current->next;
    }

    // Add program termination syscall
    fprintf(outputFile, "\tli $v0, 10\n\tsyscall\n");
}

// Helper function to check if an operand represents an array access
bool isArrayAccess(char* operand) {
    return strchr(operand, '[') != NULL;
}

// Helper function to parse array access operands
void parseArrayAccess(char* operand, char** arrayName, char** indexExpr) {
    char* leftBracket = strchr(operand, '[');
    char* rightBracket = strchr(operand, ']');

    if (leftBracket && rightBracket && leftBracket < rightBracket) {
        size_t nameLen = leftBracket - operand;
        *arrayName = strndup(operand, nameLen);

        size_t indexLen = rightBracket - leftBracket - 1;
        *indexExpr = strndup(leftBracket + 1, indexLen);
    } else {
        *arrayName = strdup(operand);
        *indexExpr = NULL;
    }
}

// Generate code to compute the address of an array element
void generateCodeForArrayAccess(char* arrayName, char* indexExpr, char* reg) {
    // Assuming array elements are 4 bytes (word)
    int regIndex = getOperandRegister(indexExpr);
    if (regIndex == -1) {
        fprintf(stderr, "Failed to get register for index expression\n");
        exit(EXIT_FAILURE);
    }

    // Multiply index by 4 to get byte offset
    fprintf(outputFile, "\tsll %s, %s, 2\n", tempRegisters[regIndex].name, tempRegisters[regIndex].name);

    // Load base address of the array
    fprintf(outputFile, "\tla %s, %s%s\n", reg, VAR_PREFIX, arrayName);

    // Add offset to base address
    fprintf(outputFile, "\tadd %s, %s, %s\n", reg, reg, tempRegisters[regIndex].name);

    deallocateRegister(regIndex);
}

// Helper function to get a register for an operand (variable, immediate, or array access)
int getOperandRegister(char* operand) {
    if (isImmediate(operand)) {
        int reg = allocateRegister();
        fprintf(outputFile, "\tli %s, %s\n", tempRegisters[reg].name, operand);
        return reg;
    } else if (isArrayAccess(operand)) {
        char* arrayName = NULL;
        char* indexExpr = NULL;
        parseArrayAccess(operand, &arrayName, &indexExpr);

        char addressReg[5] = "$a0";  // Use $a0 as temporary register for address

        generateCodeForArrayAccess(arrayName, indexExpr, addressReg);

        // Load the value from the array element
        int reg = allocateRegister();
        fprintf(outputFile, "\tlw %s, 0(%s)\n", tempRegisters[reg].name, addressReg);

        free(arrayName);
        free(indexExpr);
        return reg;
    } else if (isTemporaryVariable(operand)) {
        return getRegisterForVariable(operand);
    } else {
        int reg = allocateRegister();
        fprintf(outputFile, "\tlw %s, %s%s\n", tempRegisters[reg].name, VAR_PREFIX, operand);
        return reg;
    }
}

// Helper function to get the MIPS operator for arithmetic operations
char* getMipsOperator(char* op) {
    if (strcmp(op, "+") == 0) {
        return "add";
    } else if (strcmp(op, "-") == 0) {
        return "sub";
    } else if (strcmp(op, "*") == 0) {
        return "mul";
    } else if (strcmp(op, "/") == 0) {
        return "div";
    }
    return NULL;
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
    // Handle negative numbers
    int i = 0;
    if (operand[0] == '-') {
        i = 1;
    }
    for (; i < strlen(operand); i++) {
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
    tempRegisters[regToSpill].inUse = true;
    tempRegisters[regToSpill].varName = NULL;
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
