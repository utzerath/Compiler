#include "semantic.h"
#include <stdio.h>

// Perform semantic analysis on the AST
TAC* tacHead = NULL;

void semanticAnalysis(ASTNode* node, SymbolTable* symTab) {
    if (node == NULL) return;

    switch (node->type) {
        case NodeType_Program:
            printf("Performing semantic analysis on program\n");
            semanticAnalysis(node->program.varDeclList, symTab);
            semanticAnalysis(node->program.stmtList, symTab);
            break;
        
        case NodeType_VarDeclList:
            semanticAnalysis(node->varDeclList.varDecl, symTab);
            semanticAnalysis(node->varDeclList.varDeclList, symTab);
            break;
        
        case NodeType_VarDecl:
            // Check if variable is already declared, if not, add it to the symbol table
            if (lookupSymbol(symTab, node->varDecl.varName) == NULL) {
                addSymbol(symTab, node->varDecl.varName, node->varDecl.varType);
                printf("Added variable %s of type %s to the symbol table\n", node->varDecl.varName, node->varDecl.varType);
            } else {
                // Only trigger error if this node represents a duplicate declaration
                fprintf(stderr, "Semantic error: Variable %s is already declared\n", node->varDecl.varName);
            }
            break;
        
        case NodeType_StmtList:
            semanticAnalysis(node->stmtList.stmt, symTab);
            semanticAnalysis(node->stmtList.stmtList, symTab);
            break;
        
        case NodeType_AssignStmt:
            // Ensure the variable being assigned to has been declared
            if (lookupSymbol(symTab, node->assignStmt.varName) == NULL) {
                fprintf(stderr, "Semantic error: Variable %s has not been declared\n", node->assignStmt.varName);
            }
            // Check the expression on the right-hand side
            semanticAnalysis(node->assignStmt.expr, symTab);
            break;
        
        case NodeType_Expr:
            // Check the left and right expressions
            semanticAnalysis(node->expr.left, symTab);
            semanticAnalysis(node->expr.right, symTab);
            break;
        
        case NodeType_BinOp:
            // Ensure both operands are declared variables
            if (lookupSymbol(symTab, node->binOp.left->simpleID.name) == NULL) {
                fprintf(stderr, "Semantic error: Variable %s has not been declared\n", node->binOp.left->simpleID.name);
            }
            if (lookupSymbol(symTab, node->binOp.right->simpleID.name) == NULL) {
                fprintf(stderr, "Semantic error: Variable %s has not been declared\n", node->binOp.right->simpleID.name);
            }
            // Recursively check the expressions
            semanticAnalysis(node->binOp.left, symTab);
            semanticAnalysis(node->binOp.right, symTab);
            break;
        
        case NodeType_SimpleID:
            // Check if the variable has been declared before use
            if (lookupSymbol(symTab, node->simpleID.name) == NULL) {
                fprintf(stderr, "Semantic error: Variable %s has not been declared\n", node->simpleID.name);
            }
            break;
        
        case NodeType_SimpleExpr:
            // No need to check for numbers, they're always valid
            break;
        
        case NodeType_BlockStmt:
            // Recursively analyze the list of statements in the block
            semanticAnalysis(node->blockStmt.stmtList, symTab);
            break;

        default:
            fprintf(stderr, "Unknown Node Type\n");
            break;
    }

    // Generate TAC for expressions
    if (node->type == NodeType_Expr || node->type == NodeType_SimpleExpr) {
        TAC* tac = generateTACForExpr(node);
        printTAC(tac);
    }
}




// You can add more functions related to semantic analysis here
// Implement functions to generate TAC expressions



TAC* generateTACForExpr(ASTNode* expr) {
    // Depending on your AST structure, generate the appropriate TAC
    // If the TAC is generated successfully, append it to the global TAC list
    // Return the generated TAC, so that it can be used by the caller, e.g. for printing
    if (!expr) return NULL;

    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    if (!instruction) return NULL;

    switch (expr->type) {
        case NodeType_Expr: {
            printf("Generating TAC for expression\n");
            instruction->arg1 = createOperand(expr->expr.left);
            instruction->arg2 = createOperand(expr->expr.right);
            instruction->op = strdup("+"); //strdup(expr->expr.operator);
            instruction->result = createTempVar();
            break;
        }

        case NodeType_SimpleExpr: {
            printf("Generating TAC for simple expression\n");
            char buffer[20];
            snprintf(buffer, 20, "%d", expr->simpleExpr.number);
            instruction->arg1 = strdup(buffer);
            instruction->op = "="; //strdup(expr->expr.operator);
            instruction->arg2 = NULL;
            instruction->result = createTempVar();
            break;
        }

        case NodeType_SimpleID: {
            printf("Generating TAC for simple ID\n");
            instruction->arg1 = strdup(expr->simpleID.name);
            instruction->op = strdup("assign");
            instruction->result = createTempVar();
            break;
        }

        // Add cases for other expression types...

        default:
            free(instruction);
            return NULL;
    }

    instruction->next = NULL; // Make sure to null-terminate the new instruction

    // Append to the global TAC list
    appendTAC(&tacHead, instruction);

    return instruction;
}
// Function to create a new temporary variable for TAC
char* createTempVar() {
    static int count = 0;
    char* tempVar = malloc(10); // Enough space for "t" + number
    if (!tempVar) return NULL;
    count = allocateNextAvailableTempVar(tempVars);
    sprintf(tempVar, "t%d", count++);
    return tempVar;
}

char* createOperand(ASTNode* node) {
    // Depending on your AST structure, return the appropriate string
    // representation of the operand. For example, if the operand is a simple
    // expression or an identifier, return its string representation.
    // This function needs to be implemented based on your AST structure.
    if (!node) return NULL;

    switch (node->type) {
        case NodeType_SimpleExpr: {
            char* buffer = malloc(20);
            snprintf(buffer, 20, "%d", node->simpleExpr.number);
            return buffer;
        }

        case NodeType_SimpleID: {
            return strdup(node->simpleID.name);
        }

        case NodeType_Expr: {
            return createTempVar();
        }

        // Add cases for other operand types...

        default:
            return NULL;
    }
}

void printTAC(TAC* tac) {
    if (!tac) return;

    // Print the TAC instruction with non-null fields
    if(tac->result != NULL)
        printf("%s = ", tac->result);
    if(tac->arg1 != NULL)
        printf("%s ", tac->arg1);
    if(tac->op != NULL)
        printf("%s ", tac->op);
    if(tac->arg2 != NULL)
        printf("%s ", tac->arg2);
    printf("\n");
}

// Print the TAC list to a file
// This function is provided for reference, you can modify it as needed

void printTACToFile(const char* filename, TAC* tac) {
    FILE* file = fopen(filename , "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }   
    TAC* current = tac;
    while (current != NULL) {
        if (strcmp(current->op,"=") == 0) {
            fprintf(file, "%s = %s\n", current->result, current->arg1);
        } 
        else {
            if(current->result != NULL)
                fprintf(file, "%s = ", current->result);
            if(current->arg1 != NULL)
                fprintf(file, "%s ", current->arg1);
            if(current->op != NULL)
                fprintf(file, "%s ", current->op);
            if(current->arg2 != NULL)
                fprintf(file, "%s ", current->arg2);
            fprintf(file, "\n");
    }
        current = current->next;
    }   
    fclose(file);
    printf("TAC written to %s\n", filename);
}


// Temporary variable allocation and deallocation functions //

void initializeTempVars() {
    for (int i = 0; i < 20; i++) {
        tempVars[i] = 0;
    }
}

int allocateNextAvailableTempVar(int tempVars[]) {
   // implement the temp var allocation logic
   // use the tempVars array to keep track of allocated temp vars

    // search for the next available temp var
    for (int i = 0; i < 20; i++) {
        if (tempVars[i] == 0) {
            tempVars[i] = 1;
            return i;
        }
    }
    return -1; // No available temp var
}

void deallocateTempVar(int tempVars[], int index) {
    // implement the temp var deallocation logic
    // use the tempVars array to keep track of allocated temp vars
    if (index >= 0 && index < 20) {
        tempVars[index] = 0;
    }
}   

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
