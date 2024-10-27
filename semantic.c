#include "semantic.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>  // For strdup and strcmp

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
            semanticAnalysis(node->program.varDeclList, symTab);
            semanticAnalysis(node->program.stmtList, symTab);
            break;

        case NodeType_ArrayDecl:
            if (lookupSymbol(symTab, node->arrayDecl.varName) == NULL) {
                addSymbol(symTab, node->arrayDecl.varName, node->arrayDecl.varType, 0, NULL, NULL);
            } else {
                fprintf(stderr, "Semantic error: Array %s is already declared\n", node->arrayDecl.varName);
            }
            break;

        case NodeType_VarDeclList:
            semanticAnalysis(node->varDeclList.varDecl, symTab);
            semanticAnalysis(node->varDeclList.varDeclList, symTab);
            break;

        case NodeType_VarDecl:
            if (lookupSymbol(symTab, node->varDecl.varName) == NULL) {
                addSymbol(symTab, node->varDecl.varName, node->varDecl.varType, 0, NULL, NULL);
            } else {
                fprintf(stderr, "Semantic error: Variable %s is already declared\n", node->varDecl.varName);
            }
            break;

        case NodeType_BinOp: {  // Updated from NodeType_Expr
            printf("Performing semantic analysis on binary operation\n");

            semanticAnalysis(node->binOp.left, symTab);  // Use binOp.left instead of expr.left
            semanticAnalysis(node->binOp.right, symTab); // Use binOp.right instead of expr.right

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

        case NodeType_WriteStmt:
            printf("Generating TAC for write statement\n");

            char* operand = createOperand(node->writeStmt.expr);

            TAC* tac = (TAC*)malloc(sizeof(TAC));
            tac->arg1 = operand;
            tac->op = strdup("write");
            tac->result = NULL;
            tac->next = NULL;

            appendTAC(&tacHead, tac);
            break;

        case NodeType_StmtList:
            semanticAnalysis(node->stmtList.stmt, symTab);
            semanticAnalysis(node->stmtList.stmtList, symTab);
            break;

        case NodeType_AssignStmt:
            printf("Generating TAC for assignment statement\n");

            char* rhs = createOperand(node->assignStmt.expr);

            if (node->assignStmt.lvalue->type == NodeType_SimpleID) {
                TAC* tac = (TAC*)malloc(sizeof(TAC));
                tac->arg1 = rhs;
                tac->op = strdup("=");
                tac->result = strdup(node->assignStmt.lvalue->simpleID.name);
                tac->next = NULL;
                appendTAC(&tacHead, tac);
            }
            break;

        default:
            fprintf(stderr, "Unknown Node Type\n");
            break;
    }
}
TAC* generateTACForExpr(ASTNode* expr) {
    if (!expr) return NULL;

    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    if (!instruction) return NULL;

    switch (expr->type) {
        case NodeType_BinOp: {
            printf("Generating TAC for binary operation\n");

            // Get left and right operands
            char* leftOperand = createOperand(expr->binOp.left);
            char* rightOperand = createOperand(expr->binOp.right);

            // Determine if mixed types (int and float) and handle accordingly
            bool isLeftFloat = (expr->binOp.left->type == NodeType_SimpleExpr && 
                                expr->binOp.left->simpleExpr.valueType == 'f');
            bool isRightFloat = (expr->binOp.right->type == NodeType_SimpleExpr && 
                                 expr->binOp.right->simpleExpr.valueType == 'f');
            bool isLeftInt = (expr->binOp.left->type == NodeType_SimpleExpr && 
                              expr->binOp.left->simpleExpr.valueType == 'i');
            bool isRightInt = (expr->binOp.right->type == NodeType_SimpleExpr && 
                               expr->binOp.right->simpleExpr.valueType == 'i');

            // Type promotion: If one operand is int and the other is float, promote the int to float
            if (isLeftInt && isRightFloat) {
                // Promote left operand (int) to float
                leftOperand = promoteIntToFloat(leftOperand);
            } else if (isLeftFloat && isRightInt) {
                // Promote right operand (int) to float
                rightOperand = promoteIntToFloat(rightOperand);
            }

            // Handle different operations based on the operator type
            switch (expr->binOp.operator) {
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

            // Set the arguments and the result
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

        case NodeType_ArrayAccess: {
            printf("Generating TAC for array access\n");
            char* arrayName = strdup(expr->arrayAccess.arrayName);
            char* index = createOperand(expr->arrayAccess.index);

            instruction->arg1 = strdup(arrayName);
            instruction->arg2 = strdup(index);
            instruction->op = strdup("array_access");
            instruction->result = createTempVar();  // Result stored in a temp variable
            break;
        }

        default:
            fprintf(stderr, "Unsupported node type in expression\n");
            free(instruction);
            return NULL;
    }

    instruction->next = NULL;  // Null-terminate the instruction
    appendTAC(&tacHead, instruction);  // Add instruction to the TAC list
    return instruction;
}


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

        case NodeType_BinOp:
        case NodeType_FuncCall: {
            // Instead of using undefined functions, use generateTACForExpr
            TAC* exprTAC = generateTACForExpr(node);  // Recursively generate TAC for any complex expression (BinOp or FuncCall)
            if (exprTAC == NULL) {
                fprintf(stderr, "Error generating TAC for expression\n");
                exit(1);
            }
            return strdup(exprTAC->result);  // Return the result temp variable from the TAC
        }

        case NodeType_ArrayAccess: {
            // Generate the operand for array access (e.g., "arr[5]")
            char* arrayName = strdup(node->arrayAccess.arrayName);
            char* index = createOperand(node->arrayAccess.index);  // Recursively handle the index expression

            char* buffer = malloc(50);  // Allocate space for the operand
            snprintf(buffer, 50, "%s[%s]", arrayName, index);  // Format it as "arr[index]"
            free(arrayName);
            free(index);
            return buffer;
        }

        default:
            return NULL;
    }
}

// Definition (this should already be in your file):
char* promoteIntToFloat(char* intOperand) {
    // Create a new TAC instruction to convert the int to a float
    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    instruction->op = strdup("int_to_float");  // Operation for int to float conversion
    instruction->arg1 = strdup(intOperand);
    instruction->result = createTempVar();  // Create a temp variable to hold the float result
    appendTAC(&tacHead, instruction);  // Add the conversion instruction to the TAC list
    
    return instruction->result;  // Return the temp variable holding the float result
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
