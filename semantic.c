#include "semantic.h"
#include <stdio.h>
#include <stdbool.h>

int tempVars[20]; // Array for tracking temp variables
static int tempVarCounter = 0;

char* createTempVar() {
    char* tempVar = (char*)malloc(10);
    snprintf(tempVar, 10, "t%d", tempVarCounter++);  // Create a temp variable name like t1, t2, etc.
    return tempVar;
}

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
            if (lookupSymbol(symTab, node->varDecl.varName) == NULL) {
                addSymbol(symTab, node->varDecl.varName, node->varDecl.varType);
                printf("Added variable %s of type %s to the symbol table\n", node->varDecl.varName, node->varDecl.varType);
            } else {
                fprintf(stderr, "Semantic error: Variable %s is already declared\n", node->varDecl.varName);
            }
            break;

        case NodeType_WriteStmt: {
            printf("Generating TAC for write statement\n");

            // Get the operand for the expression to write
            char* operand = createOperand(node->writeStmt.expr);

            // Generate the TAC for the write statement
            TAC* tac = (TAC*)malloc(sizeof(TAC));
            if (!tac) {
                fprintf(stderr, "Error: Memory allocation failed for TAC\n");
                return;
            }
            tac->arg1 = operand;  // The operand being written
            tac->op = strdup("write");  // The 'write' operation
            tac->result = NULL;  // No result for 'write'
            tac->next = NULL;

            appendTAC(&tacHead, tac);
            break;
        }

        case NodeType_StmtList:
            semanticAnalysis(node->stmtList.stmt, symTab);
            semanticAnalysis(node->stmtList.stmtList, symTab);
            break;
        
        case NodeType_AssignStmt: {
            printf("Generating TAC for assignment statement\n");

            char* rhs = createOperand(node->assignStmt.expr);

            TAC* tac = (TAC*)malloc(sizeof(TAC));
            tac->arg1 = rhs;
            tac->op = strdup("=");
            tac->result = strdup(node->assignStmt.varName);
            tac->next = NULL;

            appendTAC(&tacHead, tac);
            break;
        }

        case NodeType_Expr:
            semanticAnalysis(node->expr.left, symTab);
            semanticAnalysis(node->expr.right, symTab);
            break;
        
        case NodeType_BinOp: {
            printf("Performing semantic analysis on binary operation\n");

            semanticAnalysis(node->binOp.left, symTab);
            semanticAnalysis(node->binOp.right, symTab);

            if (node->binOp.left->type == NodeType_SimpleID &&
                lookupSymbol(symTab, node->binOp.left->simpleID.name) == NULL) {
                fprintf(stderr, "Semantic error: Variable %s has not been declared\n", node->binOp.left->simpleID.name);
            }
            if (node->binOp.right->type == NodeType_SimpleID &&
                lookupSymbol(symTab, node->binOp.right->simpleID.name) == NULL) {
                fprintf(stderr, "Semantic error: Variable %s has not been declared\n", node->binOp.right->simpleID.name);
            }
            break;
        }

        case NodeType_SimpleID:
            if (lookupSymbol(symTab, node->simpleID.name) == NULL) {
                fprintf(stderr, "Semantic error: Variable %s has not been declared\n", node->simpleID.name);
            }
            break;
        
        case NodeType_SimpleExpr:
            // No need to check here, handled in expression generation
            break;

        case NodeType_BlockStmt:
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

            // Determine if mixed types (int and float) and handle accordingly
            bool isLeftFloat = (expr->expr.left->type == NodeType_SimpleExpr && 
                                expr->expr.left->simpleExpr.valueType == 'f');
            bool isRightFloat = (expr->expr.right->type == NodeType_SimpleExpr && 
                                 expr->expr.right->simpleExpr.valueType == 'f');

            // Handle different operations based on the operator type
            switch (expr->expr.operator) {  // Fix here
                case '+':
                    if (isLeftFloat || isRightFloat) {
                        instruction->op = strdup("add.s");  // Float addition
                    } else {
                        instruction->op = strdup("+");  // Integer addition
                    }
                    break;

                case '-':
                    if (isLeftFloat || isRightFloat) {
                        instruction->op = strdup("sub.s");  // Float subtraction
                    } else {
                        instruction->op = strdup("-");  // Integer subtraction
                    }
                    break;

                case '*':
                    if (isLeftFloat || isRightFloat) {
                        instruction->op = strdup("mul.s");  // Float multiplication
                    } else {
                        instruction->op = strdup("*");  // Integer multiplication
                    }
                    break;

                case '/':
                    if (isLeftFloat || isRightFloat) {
                        instruction->op = strdup("div.s");  // Float division
                    } else {
                        instruction->op = strdup("/");  // Integer division
                    }
                    break;

                default:
                    fprintf(stderr, "Unsupported binary operator\n");
                    free(instruction);
                    return NULL;
            }

            // Set arguments and the result
            instruction->arg1 = leftOperand;
            instruction->arg2 = rightOperand;
            instruction->result = createTempVar();  // Create a temporary variable for the result
            break;
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
            instruction->op = "=";  // Assignment operator
            instruction->result = createTempVar();  // Create temp variable for this constant value
            break;
        }

        case NodeType_SimpleID: {
            printf("Generating TAC for simple ID\n");
            instruction->arg1 = strdup(expr->simpleID.name);
            instruction->op = strdup("assign");
            instruction->result = createTempVar();  // Create a temp variable to hold the ID
            break;
        }

        default:
            free(instruction);
            return NULL;
    }

    instruction->next = NULL;  // Null-terminate the instruction
    appendTAC(&tacHead, instruction);  // Add instruction to the TAC list
    return instruction;
}

// Updates in createOperand to handle both int and float correctly
char* createOperand(ASTNode* node) {
    if (!node) return NULL;

    switch (node->type) {
        case NodeType_SimpleExpr: {
            char* buffer = malloc(20);
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
            return exprTAC->result;  // Return the result temp variable from the TAC
        }

        default:
            return NULL;
    }
}


void printTAC(TAC* tac) {
    if (!tac) return;

    // Handle 'write' operation first
    if (tac->op != NULL && strcmp(tac->op, "write") == 0) {
        // Print the operation first followed by the operand
        printf("%s %s\n", tac->op, tac->arg1);  // Correctly prints "write x"
    } else {
        // Handle other types of instructions
        if (tac->result != NULL)
            printf("%s = ", tac->result);
        if (tac->arg1 != NULL)
            printf("%s ", tac->arg1);
        if (tac->op != NULL)
            printf("%s ", tac->op);
        if (tac->arg2 != NULL)
            printf("%s ", tac->arg2);
        printf("\n");
    }
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
        if (strcmp(current->op, "write") == 0) {
            // Print the write operation first, followed by the argument
            fprintf(file, "%s %s\n", current->op, current->arg1); } // Correct format: "write x"
        else if (strcmp(current->op,"=") == 0) {
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
