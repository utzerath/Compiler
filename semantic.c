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
            semanticAnalysis(node->program.funcDeclList, symTab);
            break;

        case NodeType_VarDeclList:
            semanticAnalysis(node->varDeclList.varDecl, symTab);
            semanticAnalysis(node->varDeclList.varDeclList, symTab);
            break;

        case NodeType_VarDecl:
            if (lookupSymbol(symTab, node->varDecl.varName , 1) == NULL) {
                addSymbol(symTab, node->varDecl.varName, node->varDecl.varType, 0, NULL);
            } else {
                fprintf(stderr, "Semantic error: Variable %s is already declared\n", node->varDecl.varName);
            }
            break;

        case NodeType_SimpleExpr:
            // No specific analysis needed here.
            break;

        case NodeType_SimpleID:
            if (lookupSymbol(symTab, node->simpleID.name, 1) == NULL) {
                fprintf(stderr, "Semantic error: Variable %s has not been declared\n", node->simpleID.name);
            }
            break;

        case NodeType_BinOp:
            semanticAnalysis(node->binOp.left, symTab);
            semanticAnalysis(node->binOp.right, symTab);
            break;

        case NodeType_AssignStmt:
            semanticAnalysis(node->assignStmt.lvalue, symTab);
            semanticAnalysis(node->assignStmt.expr, symTab);
            break;

        case NodeType_WriteStmt:
            semanticAnalysis(node->writeStmt.expr, symTab);
            break;

        case NodeType_ArrayDecl:
            if (lookupSymbol(symTab, node->arrayDecl.varName, 1) == NULL) {
                addSymbol(symTab, node->arrayDecl.varName, node->arrayDecl.varType, 1, NULL);
            } else {
                fprintf(stderr, "Semantic error: Array %s is already declared\n", node->arrayDecl.varName);
            }
            break;

        case NodeType_ArrayAccess:
            semanticAnalysis(node->arrayAccess.index, symTab);
            if (lookupSymbol(symTab, node->arrayAccess.arrayName, 1) == NULL) {
                fprintf(stderr, "Semantic error: Array %s has not been declared\n", node->arrayAccess.arrayName);
            }
            break;

        case NodeType_StmtList:
            semanticAnalysis(node->stmtList.stmt, symTab);
            semanticAnalysis(node->stmtList.stmtList, symTab);
            break;

        case NodeType_BlockStmt:
            semanticAnalysis(node->blockStmt.stmtList, symTab);
            break;

        case NodeType_FuncDecl:
            if (lookupSymbol(symTab, node->funcDecl.name , 1) == NULL) {
                //addSymbol(symTab, node->funcDecl.name, node->funcDecl.returnType, 1, node->funcDecl.paramList, NULL);
                semanticAnalysis(node->funcDecl.paramList, symTab);
                semanticAnalysis(node->funcDecl.stmtList, symTab);
                if (node->funcDecl.returnStmt) {
                    semanticAnalysis(node->funcDecl.returnStmt, symTab);
                }
            } else {
                fprintf(stderr, "Semantic error: Function %s is already declared\n", node->funcDecl.name);
            }
            break;

        case NodeType_FuncDeclList:
            semanticAnalysis(node->funcDeclList.funcDecl, symTab);
            semanticAnalysis(node->funcDeclList.funcDeclList, symTab);
            break;

        case NodeType_ParamList:
            semanticAnalysis(node->paramList.param, symTab);
            semanticAnalysis(node->paramList.paramList, symTab);
            break;

        case NodeType_Param:
            if (lookupSymbol(symTab, node->param.varName, 1) == NULL) {
                //addSymbol(symTab, node->param.varName, node->param.varType, 0, NULL, NULL);
            } else {
                fprintf(stderr, "Semantic error: Parameter %s is already declared\n", node->param.varName);
            }
            break;

        case NodeType_FuncCall: {
            Symbol* funcSymbol = lookupSymbol(symTab, node->funcCall.name, 1);
            if (funcSymbol == NULL || !funcSymbol->isFunction) {
                fprintf(stderr, "Semantic error: Function %s has not been declared\n", node->funcCall.name);
                break;
            }
            semanticAnalysis(node->funcCall.args, symTab);
            break;
        }

        case NodeType_ReturnStmt:
            semanticAnalysis(node->returnStmt.expr, symTab);
            break;

        case NodeType_ArgList:
            semanticAnalysis(node->argList.arg, symTab);
            semanticAnalysis(node->argList.next, symTab);
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
        case NodeType_SimpleExpr: {
            char buffer[20];
            if (expr->simpleExpr.valueType == 'i') {
                snprintf(buffer, sizeof(buffer), "%d", expr->simpleExpr.intValue);
            } else if (expr->simpleExpr.valueType == 'f') {
                snprintf(buffer, sizeof(buffer), "%f", expr->simpleExpr.floatValue);
            }
            instruction->arg1 = strdup(buffer);
            instruction->op = "=";
            instruction->result = createTempVar();
            appendTAC(&tacHead, instruction);
            break;
        }

        case NodeType_SimpleID: {
            instruction->arg1 = strdup(expr->simpleID.name);
            instruction->op = "assign";
            instruction->result = createTempVar();
            appendTAC(&tacHead, instruction);
            break;
        }

        case NodeType_BinOp: {
            char* leftOperand = createOperand(expr->binOp.left);
            char* rightOperand = createOperand(expr->binOp.right);

            instruction->op = strdup(
                (expr->binOp.operator == '+') ? "+" :
                (expr->binOp.operator == '-') ? "-" :
                (expr->binOp.operator == '*') ? "*" : "/"
            );
            instruction->arg1 = leftOperand;
            instruction->arg2 = rightOperand;
            instruction->result = createTempVar();
            appendTAC(&tacHead, instruction);
            break;
        }

        case NodeType_ArrayAccess: {
            instruction->op = "array_access";
            instruction->arg1 = strdup(expr->arrayAccess.arrayName);
            instruction->arg2 = createOperand(expr->arrayAccess.index);
            instruction->result = createTempVar();
            appendTAC(&tacHead, instruction);
            break;
        }

        case NodeType_FuncCall: {
            TAC* argTACHead = NULL;
            ASTNode* argNode = expr->funcCall.args;
            while (argNode) {
                TAC* argTAC = generateTACForExpr(argNode);
                appendTAC(&argTACHead, argTAC);
                argNode = argNode->argList.next;
            }

            TAC* currentArgTAC = argTACHead;
            while (currentArgTAC) {
                TAC* pushArg = (TAC*)malloc(sizeof(TAC));
                pushArg->op = strdup("push");
                pushArg->arg1 = strdup(currentArgTAC->result);
                appendTAC(&tacHead, pushArg);
                currentArgTAC = currentArgTAC->next;
            }

            instruction->op = strdup("call");
            instruction->arg1 = strdup(expr->funcCall.name);
            instruction->result = createTempVar();
            appendTAC(&tacHead, instruction);
            break;
        }

        case NodeType_ReturnStmt: {
            instruction->op = "return";
            instruction->arg1 = createOperand(expr->returnStmt.expr);
            appendTAC(&tacHead, instruction);
            break;
        }

        default:
            fprintf(stderr, "Unsupported node type in expression\n");
            free(instruction);
            return NULL;
    }

    instruction->next = NULL;
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
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    TAC* current = tac;
    while (current != NULL) {
        if (current->op != NULL) {
            if (strcmp(current->op, "write") == 0) {
                // Write statement
                fprintf(file, "%s %s\n", current->op, current->arg1);  // Format: "write x"
            } else if (strcmp(current->op, "=") == 0) {
                // Assignment statement
                fprintf(file, "%s = %s\n", current->result, current->arg1);  // Format: "x = y"
            } else if (strcmp(current->op, "call") == 0) {
                // Function call statement
                fprintf(file, "%s = call %s\n", current->result, current->arg1);  // Format: "t1 = call function_name"
            } else if (strcmp(current->op, "push") == 0) {
                // Push argument for function call
                fprintf(file, "push %s\n", current->arg1);  // Format: "push argument"
            } else if (strcmp(current->op, "return") == 0) {
                // Return statement
                fprintf(file, "return %s\n", current->arg1 ? current->arg1 : "");  // Format: "return x"
            } else if (strcmp(current->op, "array_access") == 0) {
                // Array access operation
                fprintf(file, "%s = %s[%s]\n", current->result, current->arg1, current->arg2);  // Format: "t1 = array[index]"
            } else {
                // General arithmetic or other operation
                if (current->result != NULL)
                    fprintf(file, "%s = ", current->result);  // Format result if it exists
                if (current->arg1 != NULL)
                    fprintf(file, "%s ", current->arg1);  // Print first argument
                if (current->op != NULL)
                    fprintf(file, "%s ", current->op);  // Print operation
                if (current->arg2 != NULL)
                    fprintf(file, "%s ", current->arg2);  // Print second argument if it exists
                fprintf(file, "\n");
            }
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
