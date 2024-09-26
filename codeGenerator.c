#include "codeGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // Include for isdigit

int stackOffset = 0;  // Initialize this at the start of the program

static FILE* outputFile;

typedef struct {
    char* name;   // Name of the register, e.g., "$t0"
    bool inUse;   // Whether the register is currently in use
    bool spilled; // Whether the register has been spilled
} MIPSRegister;


// Array of temporary registers, used for register allocation
// and tracking which registers are currently in use

MIPSRegister tempRegisters[NUM_TEMP_REGISTERS] = {
    {"$t0", false}, {"$t1", false}, {"$t2", false}, {"$t3", false},
    {"$t4", false}, {"$t5", false}, {"$t6", false}, {"$t7", false},
    {"$t8", false}, {"$t9", false}
};

void initCodeGenerator(const char* outputFilename) {
    outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }
    // Write the data section header
    fprintf(outputFile, ".data\n");
}

void generateMIPS(TAC* tacInstructions) {
    TAC* current = tacInstructions;
    fprintf(outputFile, ".text\n.globl main\nmain:\n");

    while (current != NULL) {
        printCurrentTAC(current);

        if (strcmp(current->op, "=") == 0) {
            int regIndex = allocateRegister();
            if (regIndex == -1) {
                fprintf(stderr, "Register allocation failed\n");
                return;
            }

            // Check if arg1 is an immediate value
            if (isImmediate(current->arg1)) {
                fprintf(outputFile, "\tli %s, %s\n", tempRegisters[regIndex].name, current->arg1);  // Load immediate value
            } else {
                fprintf(outputFile, "\tlw %s, %s\n", tempRegisters[regIndex].name, current->arg1);  // Load variable from memory
            }

            // Store into a variable (only if the result is a real variable, not a temporary one)
            if (!isTemporaryVariable(current->result)) {
                fprintf(outputFile, "\tsw %s, %s\n", tempRegisters[regIndex].name, current->result);  // Store the result in memory
            }

            deallocateRegister(regIndex);
        }

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

            // Perform the addition and store the result in regRes
            fprintf(outputFile, "\tadd %s, %s, %s\n", tempRegisters[regRes].name, tempRegisters[reg1].name, tempRegisters[reg2].name);

            // Store the result in memory only if it is not a temporary variable
            if (!isTemporaryVariable(current->result)) {
                fprintf(outputFile, "\tsw %s, %s\n", tempRegisters[regRes].name, current->result);
            }

            deallocateRegister(reg1);
            deallocateRegister(reg2);
            deallocateRegister(regRes);
        }

        // Handle write operation (write x)
        else if (strcmp(current->op, "write") == 0) {
            fprintf(outputFile, "\tli $v0, 1\n");       // Load syscall code for print integer
            if (!isImmediate(current->result)) {
                fprintf(outputFile, "\tlw $a0, %s\n", current->result);  // Load value from memory
            } else {
                fprintf(outputFile, "\tli $a0, %s\n", current->result);  // Load immediate value
            }
            fprintf(outputFile, "\tsyscall\n");
        }

        current = current->next;
    }

    fprintf(outputFile, "\tli $v0, 10\n\tsyscall\n");
}

bool isTemporaryVariable(char* varName) {
    // Assuming temporary variables are named like t0, t1, t2, etc.
    return (varName[0] == 't' && isdigit(varName[1]));
}

void finalizeCodeGenerator(const char* outputFilename) {
    if (outputFile) {
        fclose(outputFile);
        printf("MIPS code generated and saved to file %s\n", outputFilename);
        outputFile = NULL;
    }
}

/*
 * Helper function: Check if an operand is an immediate value or a variable
 */
bool isImmediate(char* operand) {
    for (int i = 0; i < strlen(operand); i++) {
        if (!isdigit(operand[i])) {
            return false;
        }
    }
    return true;
}

/*
 * Register allocation logic
 */
int allocateRegister() {
    for (int i = 0; i < NUM_TEMP_REGISTERS; i++) {
        if (!tempRegisters[i].inUse) {
            tempRegisters[i].inUse = true;
            return i; // Return the register index
        }
    }
    // If no registers are available, pick one to spill
    int regToSpill = pickRegisterToSpill();
    spillRegister(regToSpill); // Spill the chosen register
    tempRegisters[regToSpill].inUse = true; // Mark it as in use again
    return regToSpill;
}

void spillRegister(int regIndex) {
    fprintf(outputFile, "\tsw %s, %d($sp)\n", tempRegisters[regIndex].name, stackOffset);
    stackOffset -= 4;  // Adjust the stack pointer
    tempRegisters[regIndex].inUse = false;
    tempRegisters[regIndex].spilled = true;  // Track that this register was spilled
}

void restoreRegister(int regIndex) {
    if (tempRegisters[regIndex].spilled) {  // Only restore if the register was spilled
        fprintf(outputFile, "\tlw %s, %d($sp)\n", tempRegisters[regIndex].name, stackOffset + 4);
        stackOffset += 4;  // Move stack pointer back up
        tempRegisters[regIndex].spilled = false;  // Mark as restored
    }
}

int pickRegisterToSpill() {
    // Iterate through the temporary registers to find one to spill
    for (int i = 0; i < NUM_TEMP_REGISTERS; i++) {
        if (tempRegisters[i].inUse) {
            return i; // Return the first in-use register found
        }
    }
    // If no registers are in use, this shouldn't happen, but handle as an error
    fprintf(stderr, "Error: No register available for spilling.\n");
    exit(EXIT_FAILURE); // Or handle this more gracefully if needed
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
        if (strcmp(current->op,"=") == 0) {
            printf("%s = %s\n", current->result, current->arg1);
        } 
        else {
            if(current->result != NULL)
                printf("%s = ", current->result);
            if(current->arg1 != NULL)
                printf("%s ", current->arg1);
            if(current->op != NULL)
                printf("%s ", current->op);
            if(current->arg2 != NULL)
                printf("%s ", current->arg2);
            printf("\n");
        }
        current = current->next;
    }   
}
