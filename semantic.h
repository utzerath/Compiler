// semantic.h
#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "AST.h"
#include "symbolTable.h"

// Define a structure for TAC instructions
typedef struct TAC {
    char* op;         // Operator
    char* arg1;       // Argument 1
    char* arg2;       // Argument 2
    char* result;     // Result
    struct TAC* next; // Pointer to the next instruction
} TAC;

extern int tempVars[20]; // Array for tracking temp variables
extern TAC* tacHead;     // Global head of the TAC instructions list

// Function declarations
void initializeTempVars();                              // Initialize temp variables
char* createTempVar();                                  // Create a new temp variable
void appendTAC(TAC** head, TAC* newInstruction);        // Append TAC to the list
TAC* generateTACForExpr(ASTNode* expr);                 // Generate TAC for an expression
char* createOperand(ASTNode* node);                     // Create operand for TAC
void printTAC(TAC* tac);                                // Print TAC to the console
void printTACToFile(const char* filename, TAC* tac);    // Print TAC to a file
void semanticAnalysis(ASTNode* node, SymbolTable* symTab); // Perform semantic analysis

#endif // SEMANTIC_H
