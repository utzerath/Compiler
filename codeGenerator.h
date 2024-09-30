#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "AST.h"     // Include your AST definition
#include "semantic.h" // Include your TAC definition
#include <stdbool.h>
#include <ctype.h>    // Include ctype.h for isdigit

#define NUM_TEMP_REGISTERS 10

// Structure to represent temporary registers
typedef struct {
    char* name;     // Name of the register, e.g., "$t0"
    bool inUse;     // Whether the register is currently in use
    bool spilled;   // Whether the register has been spilled
    char* varName;  // Name of the variable stored in this register
} MIPSRegister;

// List to track variable declarations dynamically
typedef struct VarList {
    char* varName;
    struct VarList* next;
} VarList;

// Function declarations

// Initializes code generation, setting up any necessary structures
void initCodeGenerator(const char* outputFilename);

// Generates MIPS assembly code from the provided TAC
void generateMIPS(TAC* tacInstructions);

// Finalizes code generation, closing files and cleaning up
void finalizeCodeGenerator(const char* outputFilename);

// Allocate a register
int allocateRegister();

// Deallocate a register
void deallocateRegister(int regIndex);

// Spill a register to memory (stack)
void spillRegister(int regIndex);

// Restore a spilled register from memory (stack)
void restoreRegister(int regIndex);

// Pick a register to spill (e.g., use an LRU or first available strategy)
int pickRegisterToSpill();

// Check if the operand is an immediate value
bool isImmediate(char* operand);

// Print the current TAC instruction (for debugging)
void printCurrentTAC(TAC* tac);

// Checks if a variable is a temporary variable (e.g., t0, t1, etc.)
bool isTemporaryVariable(char* varName);

// Declare variables dynamically based on TAC instructions
void declareVariablesFromTAC(TAC* tacInstructions);

// Add a variable to the list of declared variables
void declareVariable(char* varName);

// Check if a variable has already been declared
bool isVariableDeclared(char* varName);

// Generate the .data section for declared variables
void generateDataSection();

// Get the register index for a variable
int getRegisterForVariable(char* varName);

// Allocate a register for a variable
int allocateRegisterForVariable(char* varName);

#endif // CODE_GENERATOR_H
