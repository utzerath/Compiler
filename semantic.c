#include "semantic.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>  // For strdup and strcmp

int tempVars[20]; // Array for tracking temp variables
static int tempVarCounter = 0;

char* createTempVar() {
    int tempIndex = allocateNextAvailableTempVar(tempVars);
    if (tempIndex == -1) {
        fprintf(stderr, "Error: No available temp variables.\n");
        exit(1);
    }
    char* tempVar = (char*)malloc(10);
    snprintf(tempVar, 10, "t%d", tempIndex); // Creates temp variable name like t1, t2, etc.
    return tempVar;
}

// Perform semantic analysis on the AST
TAC* tacHead = NULL;
void semanticAnalysis(ASTNode* node, SymbolTable* symTab) {
    //printf("Entering Semantic Analysis with Symbol Table at address: %p\n", (void*)symTab);
    if (node == NULL) {printf("Node is NULL, skipping processing.\n"); return;}
    printf("Processing Node of Type: %d\n", node->type);
    switch (node->type) {
        case NodeType_Program:
            semanticAnalysis(node->program.varDeclList, symTab);
            semanticAnalysis(node->program.funcDeclList, symTab); // Handle function declarations
            semanticAnalysis(node->program.stmtList, symTab);
            if (node->program.mainFunc != NULL) {
                semanticAnalysis(node->program.mainFunc, symTab); // Analyze main function
            }
            break;
        case NodeType_MainFunc:
            printf("Processing Main Function\n");
            semanticAnalysis(node->mainFunc.stmtList, symTab); // Analyze statements within main
            break;
      case NodeType_ArrayDecl:
            // Check if the array is already declared in the current scope
            if (lookupSymbol(symTab, node->arrayDecl.varName, 1) == NULL) {
                // Append "[]" to type to indicate it's an array
                char arrayType[50];
                snprintf(arrayType, sizeof(arrayType), "%s[]", node->arrayDecl.varType); // E.g., "int[]" for an int array
                addSymbol(symTab, node->arrayDecl.varName, arrayType, 0, NULL, NULL); // Add array to symbol table with array type
                printf("Semantic Analysis: Added array %s of type %s\n", node->arrayDecl.varName, arrayType);
            } else {
                fprintf(stderr, "Semantic error: Array %s is already declared\n", node->arrayDecl.varName);
            }
            break;


            case NodeType_FuncDecl:
            if (strcmp(node->funcDecl.name, "main") == 0) {
                // Special handling for main, if needed
                if (node->funcDecl.paramList != NULL) {
                    fprintf(stderr, "Error: main function should not have parameters.\n");
                }
                if (strcmp(node->funcDecl.returnType, "void") != 0) {
                    fprintf(stderr, "Error: main function must have a void return type.\n");
                }
            }

            // Check for function existence
            if (lookupSymbol(symTab, node->funcDecl.name, 1) == NULL) {
                fprintf(stderr, "Semantic error: Function %s is missing from symbol table\n", node->funcDecl.name);
                return;
            }

            // Proceed with analysis of parameters, body, and return statement
            SymbolTable* funcSymTab = createSymbolTable(50); // Local symbol table for the function scope
            semanticAnalysis(node->funcDecl.paramList, funcSymTab); // Analyze parameters in function scope
            semanticAnalysis(node->funcDecl.stmtList, funcSymTab);  // Analyze function body
            semanticAnalysis(node->funcDecl.returnStmt, funcSymTab); // Analyze return statement if any
            break;


        case NodeType_ParamList:
            semanticAnalysis(node->paramList.param, symTab);
            semanticAnalysis(node->paramList.paramList, symTab);
            break;

        case NodeType_Param:
            if (lookupSymbol(symTab, node->param.varName, 1) == NULL) {
                addSymbol(symTab, node->param.varName, node->param.varType, 0, NULL, NULL);
            } else {
                fprintf(stderr, "Semantic error: Parameter %s is already declared\n", node->param.varName);
            }
            break;

        case NodeType_FuncCall:
            if (lookupSymbol(symTab, node->funcCall.name, 1) == NULL) {
                fprintf(stderr, "Semantic error: Function %s has not been declared\n", node->funcCall.name);
            }
            semanticAnalysis(node->funcCall.args, symTab); // Analyze function call arguments
            break;

        case NodeType_ArgList:
            semanticAnalysis(node->argList.arg, symTab);
            semanticAnalysis(node->argList.next, symTab);
            break;

        case NodeType_ReturnStmt:
            semanticAnalysis(node->returnStmt.expr, symTab); // Analyze return expression
            break;

        case NodeType_BinOp:
            printf("Performing semantic analysis on binary operation\n");
            semanticAnalysis(node->binOp.left, symTab);
            semanticAnalysis(node->binOp.right, symTab);

            if (node->binOp.left->type == NodeType_SimpleID &&
                lookupSymbol(symTab, node->binOp.left->simpleID.name, 1) == NULL) {
                fprintf(stderr, "Semantic error: Variable %s has not been declared\n", node->binOp.left->simpleID.name);
            }

            if (node->binOp.right->type == NodeType_SimpleID &&
                lookupSymbol(symTab, node->binOp.right->simpleID.name, 1) == NULL) {
                fprintf(stderr, "Semantic error: Variable %s has not been declared\n", node->binOp.right->simpleID.name);
            }
            break;

        case NodeType_SimpleID:
            if (lookupSymbol(symTab, node->simpleID.name, 1) == NULL) {
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
    char* rhs = NULL;

    // Check if the right-hand side is a function call or an expression
    if (node->assignStmt.expr->type == NodeType_FuncCall) {
        TAC* funcCallTAC = generateTACForExpr(node->assignStmt.expr);
        if (!funcCallTAC) {
            fprintf(stderr, "Error generating TAC for function call in assignment\n");
            return;
        }
        rhs = funcCallTAC->result;
    } else {
        rhs = createOperand(node->assignStmt.expr);
    }

    if (node->assignStmt.lvalue->type == NodeType_SimpleID) {
        // Standard variable assignment
        TAC* tac = (TAC*)malloc(sizeof(TAC));
        tac->arg1 = rhs;
        tac->op = strdup("=");
        tac->result = strdup(node->assignStmt.lvalue->simpleID.name);
        appendTAC(&tacHead, tac);
    } else if (node->assignStmt.lvalue->type == NodeType_ArrayAccess) {
        // Array assignment
        char* arrayName = strdup(node->assignStmt.lvalue->arrayAccess.arrayName);
        char* index = createOperand(node->assignStmt.lvalue->arrayAccess.index);

        TAC* tac = (TAC*)malloc(sizeof(TAC));
        tac->op = strdup("array_assign");
        tac->arg1 = rhs;
        tac->arg2 = index;
        tac->result = arrayName;
        appendTAC(&tacHead, tac);

        printf("Generated TAC for array assignment: %s[%s] = %s\n", arrayName, index, rhs);
    }
    break;





        case NodeType_VarDeclList:
            printf("Processing Variable Declaration List\n");
            semanticAnalysis(node->varDeclList.varDecl, symTab);
            semanticAnalysis(node->varDeclList.varDeclList, symTab);
            break;

        case NodeType_VarDecl:
            printf("Processing Variable Declaration: %s of type %s\n", node->varDecl.varName, node->varDecl.varType);
            if (lookupSymbol(symTab, node->varDecl.varName, 1) == NULL) {
                addSymbol(symTab, node->varDecl.varName, node->varDecl.varType, 0, NULL, NULL);
            } else {
                fprintf(stderr, "Semantic error: Variable %s is already declared\n", node->varDecl.varName);
            }
            break;

        case NodeType_FuncDeclList:
            printf("Processing Function Declaration List\n");
            semanticAnalysis(node->funcDeclList.funcDecl, symTab);
            semanticAnalysis(node->funcDeclList.funcDeclList, symTab);
            break;
        default:
            fprintf(stderr, "Unknown Node Type\n");
            break;
    }
}
TAC* generateTACForExpr(ASTNode* expr) {
    if (!expr) return NULL;

    printf("Generating TAC for Node Type: %d\n", expr->type);
    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    if (!instruction) {
        fprintf(stderr, "Error: Memory allocation failed for TAC instruction.\n");
        return NULL;
    }

    switch (expr->type) {
        case NodeType_SimpleExpr: {
            char buffer[20];
            if (expr->simpleExpr.valueType == 'i') {
                snprintf(buffer, sizeof(buffer), "%d", expr->simpleExpr.intValue);
            } else if (expr->simpleExpr.valueType == 'f') {
                snprintf(buffer, sizeof(buffer), "%f", expr->simpleExpr.floatValue);
            }
            instruction->arg1 = strdup(buffer);
            instruction->op = strdup("=");
            instruction->result = createTempVar();
            appendTAC(&tacHead, instruction);
            return instruction;  // Return the TAC instruction
        }

        case NodeType_SimpleID: {
            instruction->arg1 = strdup(expr->simpleID.name);
            instruction->op = strdup("assign");
            instruction->result = createTempVar();
            appendTAC(&tacHead, instruction);
            return instruction;
        }

        case NodeType_BinOp: {
            char* leftOperand = createOperand(expr->binOp.left);
            char* rightOperand = createOperand(expr->binOp.right);
            if (!leftOperand || !rightOperand) {
                fprintf(stderr, "Error: Operand generation failed for BinOp.\n");
                free(instruction);
                free(leftOperand);
                free(rightOperand);
                return NULL;
            }

            instruction->op = strdup(
                (expr->binOp.operator == '+') ? "+" :
                (expr->binOp.operator == '-') ? "-" :
                (expr->binOp.operator == '*') ? "*" : "/"
            );
            instruction->arg1 = leftOperand;
            instruction->arg2 = rightOperand;
            instruction->result = createTempVar();
            appendTAC(&tacHead, instruction);
            return instruction;
        }

      case NodeType_ArrayAccess: {
    char* arrayName = strdup(expr->arrayAccess.arrayName);
    char* index = createOperand(expr->arrayAccess.index);  // Evaluate the index

    // Check for successful operand generation
    if (!arrayName || !index) {
        fprintf(stderr, "Error: Operand generation failed for ArrayAccess.\n");
        free(arrayName);
        free(index);
        free(instruction);
        return NULL;
    }

    instruction->op = strdup("array_access");
    instruction->arg1 = arrayName;      // Array name
    instruction->arg2 = index;          // Index within the array
    instruction->result = createTempVar();  // Temporary variable to store accessed value

    appendTAC(&tacHead, instruction);  // Append TAC instruction to the list

    // Debug output for verification
    printf("Generated TAC for array access: %s = %s[%s]\n", instruction->result, arrayName, index);

    return instruction;
}


        case NodeType_ArgList: {
    while (expr) {
        TAC* argTAC = generateTACForExpr(expr->argList.arg);  // Process each argument
        if (!argTAC || !argTAC->result) {  // Check if argTAC is valid and has a result
            fprintf(stderr, "Error: TAC generation failed for function argument.\n");
            free(instruction);
            return NULL;
        }
        
        TAC* pushArg = (TAC*)malloc(sizeof(TAC));
        pushArg->op = strdup("push");
        pushArg->arg1 = strdup(argTAC->result); // Push argument result
        pushArg->result = NULL;
        pushArg->next = NULL;
        
        appendTAC(&tacHead, pushArg);  // Append push instruction
        expr = expr->argList.next;     // Move to the next argument
    }
    free(instruction);  // Free the unused `instruction`
    return NULL;        // Return NULL as ArgList does not return an instruction
}


       case NodeType_FuncCall: {
    ASTNode* argNode = expr->funcCall.args;
    
    // Push arguments onto the stack
    while (argNode) {
        TAC* argTAC = generateTACForExpr(argNode->argList.arg);
        if (!argTAC || !argTAC->result) {
            fprintf(stderr, "Error: TAC generation failed for function call argument.\n");
            free(instruction);
            return NULL;
        }
        
        // Generate push instruction for each argument
        TAC* pushArg = (TAC*)malloc(sizeof(TAC));
        if (!pushArg) {
            fprintf(stderr, "Error: Memory allocation failed for pushArg.\n");
            free(instruction);
            return NULL;
        }
        pushArg->op = strdup("push");
        pushArg->arg1 = strdup(argTAC->result);
        pushArg->result = NULL;
        appendTAC(&tacHead, pushArg);
        
        argNode = argNode->argList.next;
    }

    // Generate the call instruction and store the result in a temporary variable
    instruction->op = strdup("call");
    instruction->arg1 = strdup(expr->funcCall.name);
    instruction->result = createTempVar(); // Temporary variable to hold function return value
    appendTAC(&tacHead, instruction);

    return instruction; // Return the TAC instruction containing the function call result
}

        default:
            fprintf(stderr, "Unsupported node type in expression (Node Type: %d)\n", expr->type);
            free(instruction);
            return NULL;
    }
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
            } else {
                fprintf(stderr, "Unsupported SimpleExpr value type\n");
                free(buffer);
                return NULL;
            }
            return buffer;
        }

        case NodeType_SimpleID:
            return strdup(node->simpleID.name);

        case NodeType_BinOp:
        case NodeType_FuncCall: {
            // Generate TAC for complex expressions and return result variable
            TAC* exprTAC = generateTACForExpr(node);
            if (exprTAC == NULL) {
                fprintf(stderr, "Error generating TAC for expression\n");
                return NULL;
            }
            return exprTAC->result;
        }

        case NodeType_ArrayAccess: {
            // Generate TAC for array access
            TAC* arrayAccessTAC = generateTACForExpr(node);
            if (!arrayAccessTAC) {
                fprintf(stderr, "Error generating TAC for array access expression\n");
                return NULL;
            }
            return arrayAccessTAC->result;
        }

        default:
            fprintf(stderr, "Unsupported node type in operand creation (Node Type: %d)\n", node->type);
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
        printf("%s %s\n", tac->op, tac->arg1);
    } else if (tac->result && strstr(tac->result, "[")) {
        // Special handling for array assignments
        printf("%s = %s\n", tac->result, tac->arg1);
    } else {
        // General case for binary or other operations
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

void printTACToFile(const char* filename, TAC* tac) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    TAC* current = tac;
    int lineNumber = 0;  // Line counter for debugging purposes

    while (current != NULL) {
        // Debug output to track each instruction before writing to the file
        printf("Writing TAC to file (line %d): op=%s, arg1=%s, arg2=%s, result=%s\n", 
               lineNumber, 
               current->op ? current->op : "(null)",
               current->arg1 ? current->arg1 : "(null)", 
               current->arg2 ? current->arg2 : "(null)", 
               current->result ? current->result : "(null)");

        // Write the TAC instruction to the file including the `op` for debugging
        if (current->op != NULL) {
            //fprintf(file, "// op: %s\n", current->op);  // Print the operation type as a comment for debugging
            if (strcmp(current->op, "write") == 0) {
                fprintf(file, "%s %s\n", current->op, current->arg1);
            } else if (strcmp(current->op, "=") == 0) {
                fprintf(file, "%s = %s\n", current->result, current->arg1);
            } else if (strcmp(current->op, "call") == 0) {
                fprintf(file, "%s = call %s\n", current->result, current->arg1);
            } else if (strcmp(current->op, "push") == 0) {
                fprintf(file, "push %s\n", current->arg1);
            } else if (strcmp(current->op, "return") == 0) {
                fprintf(file, "return %s\n", current->arg1 ? current->arg1 : "");
            } else if (strcmp(current->op, "array_access") == 0) {
                fprintf(file, "%s = %s[%s]\n", current->result, current->arg1, current->arg2);
            } else if (strcmp(current->op, "array_assign") == 0) {
                // Handle array assignment in file output
                fprintf(file, "%s[%s] = %s\n", current->result, current->arg2, current->arg1);
            } else {
                // General case for binary or other operations
                fprintf(file, "%s = %s %s %s\n", current->result, current->arg1, current->op, current->arg2);
            }
        } else {
            fprintf(file, "// Warning: TAC node has no operation, skipping...\n");
        }

        current = current->next;
        lineNumber++;  // Increment line number for debugging
    }

    fclose(file);
    printf("TAC written to %s with %d lines.\n", filename, lineNumber);
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
    // Check if the instruction is specifically for array assignment
    if (newInstruction->op && strcmp(newInstruction->op, "array_assign") == 0) {
        // Format the debug output specifically for array assignments
        printf("Appending array assignment TAC: %s[%s] = %s\n", 
               newInstruction->result ? newInstruction->result : "(null)", 
               newInstruction->arg2 ? newInstruction->arg2 : "(null)", 
               newInstruction->arg1 ? newInstruction->arg1 : "(null)");
    } else {
        // General debug output for other TAC instructions
        printf("Appended TAC: %s %s %s -> %s\n", 
               newInstruction->op ? newInstruction->op : "(null)", 
               newInstruction->arg1 ? newInstruction->arg1 : "(null)", 
               newInstruction->arg2 ? newInstruction->arg2 : "(null)", 
               newInstruction->result ? newInstruction->result : "(null)");
    }

    // Append the new instruction to the end of the TAC list
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


