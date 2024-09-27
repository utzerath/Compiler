#include "codeGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  // Include for isdigit
#include <string.h> // Include for strlen and strcmp

int stackOffset = 0;  // Initialize this at the start of the program
static FILE* outputFile;

// Structure to represent temporary registers
typedef struct {
    char* name;   // Name of the register, e.g., "$t0"
    bool inUse;   // Whether the register is currently in use
    bool spilled; // Whether the register has been spilled
} MIPSRegister;

// Array of temporary registers, used for register allocation and tracking
MIPSRegister tempRegisters[NUM_TEMP_REGISTERS] = {
    {"$t0", false}, {"$t1", false}, {"$t2", false}, {"$t3", false},
    {"$t4", false}, {"$t5", false}, {"$t6", false}, {"$t7", false},
    {"$t8", false}, {"$t9", false}
};

// List to track variable declarations dynamically
typedef struct VarList {
    char* varName;
    struct VarList* next;
} VarList;

VarList* declaredVariables = NULL;

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
        fprintf(outputFile, "%s: .word 0\n", current->varName);  // Declare variable with initial value 0
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

void generateMIPS(TAC* tacInstructions) {
    TAC* current = tacInstructions;

    // Declare variables in memory (only variables, not immediate values)
    declareVariablesFromTAC(tacInstructions);  // Dynamically declare variables from TAC

    // Start the text section and main label
    fprintf(outputFile, ".text\n.globl main\nmain:\n");

    while (current != NULL) {
        printCurrentTAC(current);

        // Assignment operation (e.g., x = 1)
        if (strcmp(current->op, "=") == 0) {
            int regIndex = allocateRegister();
            if (regIndex == -1) {
                fprintf(stderr, "Register allocation failed\n");
                return;
            }

            // If the right-hand side (arg1) is an immediate value, use li (load immediate)
            if (isImmediate(current->arg1)) {
                fprintf(outputFile, "\tli %s, %s\n", tempRegisters[regIndex].name, current->arg1);
            } else {
                // Otherwise, load the variable from memory
                fprintf(outputFile, "\tlw %s, %s\n", tempRegisters[regIndex].name, current->arg1);
            }

            // Store the result into memory (only for variables, not for temporary variables)
            if (!isTemporaryVariable(current->result)) {
                fprintf(outputFile, "\tsw %s, %s\n", tempRegisters[regIndex].name, current->result);
            }

            deallocateRegister(regIndex);
        }

        // Addition operation (e.g., x = y + z)
        else if (strcmp(current->op, "+") == 0) {
            int reg1 = allocateRegister();
            int reg2 = allocateRegister();
            int regRes = allocateRegister();

            // Load arg1 (either as an immediate or from memory)
            if (isImmediate(current->arg1)) {
                fprintf(outputFile, "\tli %s, %s\n", tempRegisters[reg1].name, current->arg1);
            } else {
                fprintf(outputFile, "\tlw %s, %s\n", tempRegisters[reg1].name, current->arg1);
            }

            // Load arg2 (either as an immediate or from memory)
            if (isImmediate(current->arg2)) {
                fprintf(outputFile, "\tli %s, %s\n", tempRegisters[reg2].name, current->arg2);
            } else {
                fprintf(outputFile, "\tlw %s, %s\n", tempRegisters[reg2].name, current->arg2);
            }

            // Perform the addition
            fprintf(outputFile, "\tadd %s, %s, %s\n", tempRegisters[regRes].name, tempRegisters[reg1].name, tempRegisters[reg2].name);

            // Store the result in memory only if it's a variable (not a temporary)
            if (!isTemporaryVariable(current->result)) {
                fprintf(outputFile, "\tsw %s, %s\n", tempRegisters[regRes].name, current->result);
            }

            deallocateRegister(reg1);
            deallocateRegister(reg2);
            deallocateRegister(regRes);
        }

        // Handle write operation (write x)
        else if (strcmp(current->op, "write") == 0) {
            fprintf(outputFile, "\tli $v0, 1\n");  // Load syscall code for print integer
            if (!isImmediate(current->result)) {
                fprintf(outputFile, "\tlw $a0, %s\n", current->result);  // Load variable from memory
            } else {
                fprintf(outputFile, "\tli $a0, %s\n", current->result);  // Load immediate value directly
            }
            fprintf(outputFile, "\tsyscall\n");
        }

        current = current->next;
    }

    // Add program termination syscall
    fprintf(outputFile, "\tli $v0, 10\n\tsyscall\n");
}


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

// Helper function: Check if an operand is an immediate value or a variable
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
        if (!tempRegisters[i].inUse) {
            tempRegisters[i].inUse = true;
            return i; // Return the register index
        }
    }
    int regToSpill = pickRegisterToSpill();
    spillRegister(regToSpill); 
    tempRegisters[regToSpill].inUse = true; 
    return regToSpill;
}

void spillRegister(int regIndex) {
    fprintf(outputFile, "\tsw %s, %d($sp)\n", tempRegisters[regIndex].name, stackOffset);
    stackOffset -= 4;
    tempRegisters[regIndex].inUse = false;
    tempRegisters[regIndex].spilled = true;
}

void restoreRegister(int regIndex) {
    if (tempRegisters[regIndex].spilled) {
        fprintf(outputFile, "\tlw %s, %d($sp)\n", tempRegisters[regIndex].name, stackOffset + 4);
        stackOffset += 4;
        tempRegisters[regIndex].spilled = false;
    }
}

int pickRegisterToSpill() {
    for (int i = 0; i < NUM_TEMP_REGISTERS; i++) {
        if (tempRegisters[i].inUse) {
            return i;
        }
    }
    fprintf(stderr, "Error: No register available for spilling.\n");
    exit(EXIT_FAILURE); 
}

void deallocateRegister(int regIndex) {
    if (regIndex >= 0 && regIndex < NUM_TEMP_REGISTERS) {
        tempRegisters[regIndex].inUse = false;
    }
}

void printCurrentTAC(TAC* tac) {
    TAC* current = tac;
    while (current != NULL) {
        printf("\n--- CURRENT TAC %s ---\n", current->op);
        if (strcmp(current->op, "=") == 0) {
            printf("%s = %s\n", current->result, current->arg1);
        } else {
            if (current->result != NULL) printf("%s = ", current->result);
            if (current->arg1 != NULL) printf("%s ", current->arg1);
            if (current->op != NULL) printf("%s ", current->op);
            if (current->arg2 != NULL) printf("%s ", current->arg2);
            printf("\n");
        }
        current = current->next;
    }
}
