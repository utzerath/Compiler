/*
Adding an optimization phase for Three-Address Code (TAC) before generating MIPS assembly 
is a crucial step to enhance the efficiency of your generated code. 
Optimizations can reduce the number of instructions, minimize memory access, 
and improve overall execution speed. Below, we'll outline a basic approach for 
implementing a TAC optimizer. This optimizer will focus on simple peephole optimizations,
which look at small sets of instructions (or "peepholes") to apply local optimizations.

Basic Optimizations:
-------------------
1. Constant Folding: Evaluate expressions involving constants at compile time instead of runtime.
2. Constant Propagation: Replace variables with known constant values.
3. Copy Propagation: Replace uses of a variable that has been assigned the value of another variable.
4. Dead Code Elimination: Remove instructions that compute values not used by subsequent instructions 
   or the program's output.
*/

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "semantic.h"  // This should define the TAC structure
#include <stdbool.h>
#include <ctype.h>

// Optimization function to be called to start optimizing the TAC list
void optimizeTAC(TAC** head);

// Function to check if a given string represents a constant (e.g., a number)
bool isConstant(const char* str);

// Declare the function
bool isTempVariable(char* varName);

// Perform constant folding optimization (evaluating constant expressions at compile time)
bool constantFolding(TAC** head);

// Perform constant propagation optimization (replacing variables with constant values)
bool constantPropagation(TAC** head);

// Perform copy propagation (replace variables with copies of another variable's value)
bool copyPropagation(TAC** head);

// Perform dead code elimination (remove instructions whose results are not used)
bool deadCodeElimination(TAC** head);
bool isUsedLater(TAC* head, const char* varName);
void safeStrReplace(char** target, const char* source);
// Utility function to print the optimized TAC to a file
void printOptimizedTAC(const char* filename, TAC* head);

// Utility function to print the current TAC instruction to the console
void printCurrentOptimizedTAC(TAC* current);
// In optimizer.h
void freeTACList(TAC* head);

void foldFloatOperation(TAC* current, float result);
void foldOperation(TAC* current, int result);
bool isFloatConstant(const char* str);

#endif // OPTIMIZER_H
