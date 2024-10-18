#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int tempVars[20]; // Array for tracking temp variables
static int tempVarCounter = 0;

TAC* tacHead = NULL; // Global head of the TAC instructions list

// Function to initialize temporary variables
void initializeTempVars() {
    for (int i = 0; i < 20; i++) {
        tempVars[i] = 0;
    }
}

// Function to create a new temporary variable name
char* createTempVar() {
    char* tempVar = (char*)malloc(10);
    snprintf(tempVar, 10, "t%d", tempVarCounter++);  // Create a temp variable name like t0, t1, etc.
    return tempVar;
}

// Append TAC to the linked list
void appendTAC(TAC** head, TAC* newInstruction) {
    if (!*head) {
        *head = newInstruction;
    } else {
        TAC* current = *head;
        while (current->next) {
            current = current->next;
        }
        current->next = newInstruction;
    }
}

// Generate TAC for an expression
TAC* generateTACForExpr(ASTNode* expr) {
    if (!expr) return NULL;

    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    if (!instruction) return NULL;

    switch (expr->type) {
        case NodeType_Expr: {
            printf("Generating TAC for expression\n");

            // Get left and right operands
            char* leftOperand = createOperand(expr->expr.left);
            char* rightOperand = createOperand(expr->expr.right);

            // Set the operator
            instruction->op = (char*)malloc(3);
            snprintf(instruction->op, 3, "%c", expr->expr.operator);

            // Set arguments and the result
            instruction->arg1 = leftOperand;
            instruction->arg2 = rightOperand;
            instruction->result = createTempVar();  // Create a temporary variable for the result
            instruction->next = NULL;

            appendTAC(&tacHead, instruction);
            return instruction;
        }

        case NodeType_SimpleExpr: {
            printf("Generating TAC for simple expression\n");
            char buffer[20];
            if (expr->simpleExpr.valueType == 'i') {
                snprintf(buffer, 20, "%d", expr->simpleExpr.intValue);
            } else if (expr->simpleExpr.valueType == 'f') {
                snprintf(buffer, 20, "%f", expr->simpleExpr.floatValue);
            }
            instruction->arg1 = strdup(buffer);
            instruction->op = strdup("=");  // Assignment operator
            instruction->result = createTempVar();  // Create temp variable for this constant value
            instruction->arg2 = NULL;
            instruction->next = NULL;

            appendTAC(&tacHead, instruction);
            return instruction;
        }

        case NodeType_SimpleID: {
            printf("Generating TAC for simple ID\n");
            instruction->arg1 = strdup(expr->simpleID.name);
            instruction->op = strdup("assign");
            instruction->result = createTempVar();  // Create a temp variable to hold the ID
            instruction->arg2 = NULL;
            instruction->next = NULL;

            appendTAC(&tacHead, instruction);
            return instruction;
        }

        default:
            free(instruction);
            return NULL;
    }
}

// Create operand for TAC
char* createOperand(ASTNode* node) {
    if (!node) return NULL;

    switch (node->type) {
        case NodeType_SimpleExpr: {
            char* buffer = (char*)malloc(20);
            if (node->simpleExpr.valueType == 'i') {
                snprintf(buffer, 20, "%d", node->simpleExpr.intValue);
            } else if (node->simpleExpr.valueType == 'f') {
                snprintf(buffer, 20, "%f", node->simpleExpr.floatValue);
            }
            return buffer;
        }

        case NodeType_SimpleID: {
            return strdup(node->simpleID.name);
        }

        case NodeType_Expr: {
            TAC* exprTAC = generateTACForExpr(node);  // Recursively generate TAC
            return strdup(exprTAC->result);  // Return the result temp variable from the TAC
        }

        default:
            return NULL;
    }
}

// Print TAC instruction
void printTAC(TAC* tac) {
    if (!tac) return;

    if (strcmp(tac->op, "write") == 0) {
        printf("%s %s\n", tac->op, tac->arg1);
    } else if (strcmp(tac->op, "=") == 0) {
        printf("%s = %s\n", tac->result, tac->arg1);
    } else {
        printf("%s = %s %s %s\n", tac->result, tac->arg1, tac->op, tac->arg2);
    }
}

// Print TAC to file
void printTACToFile(const char* filename, TAC* tac) {
    FILE* file = fopen(filename , "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    TAC* current = tac;
    while (current != NULL) {
        if (strcmp(current->op, "write") == 0) {
            fprintf(file, "%s %s\n", current->op, current->arg1);
        } else if (strcmp(current->op, "=") == 0) {
            fprintf(file, "%s = %s\n", current->result, current->arg1);
        } else {
            fprintf(file, "%s = %s %s %s\n", current->result, current->arg1, current->op, current->arg2);
        }
        current = current->next;
    }
    fclose(file);
    printf("TAC written to %s\n", filename);
}

// Semantic analysis function
void semanticAnalysis(ASTNode* node, SymbolTable* symTab) {
    if (node == NULL) return;

    switch (node->type) {
        // Handle different node types...
        case NodeType_AssignStmt: {
            printf("Generating TAC for assignment statement\n");

            // Handle the left-hand side
            char* lhs = createOperand(node->assignStmt.lvalue);

            // Handle the right-hand side
            char* rhs = createOperand(node->assignStmt.expr);

            TAC* tac = (TAC*)malloc(sizeof(TAC));
            tac->arg1 = rhs;
            tac->op = strdup("=");
            tac->result = lhs;
            tac->arg2 = NULL;
            tac->next = NULL;

            appendTAC(&tacHead, tac);
            break;
        }
        // Handle other node types...
        default:
            break;
    }
}
