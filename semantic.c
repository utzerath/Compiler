#include "semantic.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>  // For strdup and strcmp

void removeTACNode(TAC** head, TAC* nodeToRemove) {
    if (!head || !*head || !nodeToRemove) return;

    TAC* current = *head;
    TAC* prev = NULL;

    while (current) {
        if (current == nodeToRemove) {
            if (prev) {
                prev->next = current->next;
            } else {
                *head = current->next; // If the node to remove is the head
            }
            free(current->op);
            free(current->arg1);
            free(current->arg2);
            free(current->result);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

TAC* copyTACNode(TAC* original) {
    if (!original) return NULL;

    TAC* copy = (TAC*)malloc(sizeof(TAC));
    if (!copy) {
        fprintf(stderr, "Memory allocation failed for TAC copy\n");
        exit(EXIT_FAILURE);
    }

    copy->op = original->op ? strdup(original->op) : NULL;
    copy->arg1 = original->arg1 ? strdup(original->arg1) : NULL;
    copy->arg2 = original->arg2 ? strdup(original->arg2) : NULL;
    copy->result = original->result ? strdup(original->result) : NULL;
    copy->next = NULL; // New node is not connected to any other node yet

    return copy;
}

TAC* insertAfterTACNode(TAC** head, TAC* insertionPoint, TAC* newNode) {
    if (!newNode) return NULL;

    if (!head || !*head) {
        // If the list is empty, make the new node the head
        *head = newNode;
        return newNode;
    }

    if (!insertionPoint) {
        // If insertionPoint is NULL, insert at the beginning
        newNode->next = *head;
        *head = newNode;
        return newNode;
    }

    // Insert after the specified node
    newNode->next = insertionPoint->next;
    insertionPoint->next = newNode;

    return newNode;
}



int tempVars[20]; // Array for tracking temp variables
static int tempVarCounter = 0;
static char* currentFunctionName = NULL;


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

        case NodeType_BlockStmt:
            printf("Processing Block Statement\n");
            semanticAnalysis(node->blockStmt.stmtList, symTab); // Analyze the statements within the block
            break;
        case NodeType_SimpleID:  // Node Type 3
            printf("Processing Identifier: %s\n", node->simpleID.name);
            // Check if the identifier is declared
            if (!lookupSymbol(symTab, node->simpleID.name, symTab->currentScopeLevel)) {
                fprintf(stderr, "Semantic error: Undeclared variable '%s'.\n", node->simpleID.name);
            }
            break;

        case NodeType_SimpleExpr:  // Node Type 4
            printf("Processing Simple Expression: %d\n", node->simpleExpr.intValue);
            // No further action needed for literals
            break;
        case NodeType_FuncDecl:
            currentFunctionName = strdup(node->funcDecl.name);
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

            // Assign paramX to the actual parameter name
            ASTNode* paramListNode = node->funcDecl.paramList;
            int paramIndex = 0;
            while (paramListNode != NULL) {
                // Extract the current parameter node
                ASTNode* paramNode = paramListNode->paramList.param; // Current parameter node
                paramIndex++;
                char paramVarName[20];
                snprintf(paramVarName, sizeof(paramVarName), "param%d", paramIndex);

                // Assign paramVarName to the actual parameter name
                TAC* assignTAC = (TAC*)malloc(sizeof(TAC));
                assignTAC->op = strdup("=");
                assignTAC->arg1 = strdup(paramVarName);
                assignTAC->result = strdup(paramNode->param.varName);
                assignTAC->next = NULL;
                appendTAC(&tacHead, assignTAC);

                // **Add the parameter to the symbol table**
                if (lookupSymbol(funcSymTab, paramNode->param.varName, 1) == NULL) {
                    addSymbol(funcSymTab, paramNode->param.varName, paramNode->param.varType, 0, NULL, NULL);
                } else {
                    fprintf(stderr, "Semantic error: Parameter %s is already declared\n", paramNode->param.varName);
                }

                // Move to the next parameter list node
                paramListNode = paramListNode->paramList.paramList;
            }

            semanticAnalysis(node->funcDecl.stmtList, funcSymTab);  // Analyze function body
            semanticAnalysis(node->funcDecl.returnStmt, funcSymTab); // Analyze return statement if any
            free(currentFunctionName); // Free the allocated memory
            currentFunctionName = NULL; // Reset the function name
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
            printf("Generating TAC for return statement\n");
            // Generate TAC for the expression being returned
            TAC* exprTAC = generateTACForExpr(node->returnStmt.expr);
            if (!exprTAC || !exprTAC->result) {
                fprintf(stderr, "Error generating TAC for return expression\n");
                return;
            }
            // Create the 'return' instruction
            TAC* returnTAC = (TAC*)malloc(sizeof(TAC));
            returnTAC->op = strdup("return");
            returnTAC->arg1 = strdup(exprTAC->result);
            returnTAC->arg2 = strdup(currentFunctionName); // Include function name
            returnTAC->result = NULL;
            returnTAC->next = NULL;
            appendTAC(&tacHead, returnTAC);
            break;



        case NodeType_BinOp:
            printf("Performing semantic analysis on binary operation: %c\n", node->binOp.operator);

            // Recursively analyze both operands
            semanticAnalysis(node->binOp.left, symTab);
            semanticAnalysis(node->binOp.right, symTab);

            // Check operator type for arithmetic or comparison
            if (node->binOp.operator == '+' || node->binOp.operator == '-' || 
                node->binOp.operator == '*' || node->binOp.operator == '/') {
                // Ensure operands are numeric for arithmetic operators
                if (!isNumericType(node->binOp.left, symTab) || !isNumericType(node->binOp.right, symTab)) {
                    fprintf(stderr, "Semantic error: Arithmetic operators require numeric operands\n");
                    exit(1);
                }
            } else if (node->binOp.operator == '<' || node->binOp.operator == '>' || 
                    node->binOp.operator == '=' || node->binOp.operator == '!') {
                // Ensure operands are comparable for comparison operators
                if (!areComparableTypes(node->binOp.left, node->binOp.right, symTab)) {
                    fprintf(stderr, "Semantic error: Comparison operators require compatible operands\n");
                    exit(1);
                }
            } else {
                fprintf(stderr, "Semantic error: Unsupported binary operator '%c'\n", node->binOp.operator);
                exit(1);
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

        case NodeType_AssignStmt: {
    printf("Generating TAC for assignment statement\n");
    char* rhs = createOperand(node->assignStmt.expr);
    char* lhsName = node->assignStmt.lvalue->simpleID.name;

    // Check if LHS is a declared variable
    Symbol* lhsSymbol = lookupSymbol(symTab, lhsName, 1);
    if (!lhsSymbol) {
        fprintf(stderr, "Semantic error: Variable %s not declared\n", lhsName);
        return;
    }

    // Type compatibility check
    if (strcmp(lhsSymbol->type, "bool") == 0) {
        // Ensure RHS evaluates to a boolean
        if (node->assignStmt.expr->type != NodeType_BoolLiteral &&
            node->assignStmt.expr->type != NodeType_LogicalOp &&
            node->assignStmt.expr->type != NodeType_BinOp) {
            fprintf(stderr, "Semantic error: Assignment to boolean variable %s must be a boolean expression\n", lhsName);
            return;
        }
    }

    // Generate TAC for assignment
    TAC* tac = (TAC*)malloc(sizeof(TAC));
    tac->op = strdup("=");
    tac->arg1 = rhs;  // Fully evaluated RHS
    tac->result = strdup(lhsName);  // LHS
    appendTAC(&tacHead, tac);
    break;
}



        case NodeType_LogicalOp:
            printf("Performing semantic analysis on logical operation: %s\n", node->logicalOp.operator);

            // Ensure left operand is boolean
            semanticAnalysis(node->logicalOp.left, symTab);
            if (!isBooleanType(node->logicalOp.left, symTab)) {
                fprintf(stderr, "Semantic error: Left operand of logical operator '%s' must be boolean\n", node->logicalOp.operator);
                exit(1);
            }

            // Ensure right operand is boolean (except for NOT)
            if (strcmp(node->logicalOp.operator, "!") != 0) { // NOT is unary, no right operand
                semanticAnalysis(node->logicalOp.right, symTab);
                if (!isBooleanType(node->logicalOp.right, symTab)) {
                    fprintf(stderr, "Semantic error: Right operand of logical operator '%s' must be boolean\n", node->logicalOp.operator);
                    exit(1);
                }
            }
            break;


        case NodeType_VarDeclList:
            printf("Processing Variable Declaration List\n");
            semanticAnalysis(node->varDeclList.varDecl, symTab);
            semanticAnalysis(node->varDeclList.varDeclList, symTab);
            break;

        case NodeType_VarDecl:
            printf("Processing Variable Declaration: %s of type %s\n", node->varDecl.varName, node->varDecl.varType);
            if (strcmp(node->varDecl.varType, "bool") == 0 || 
                strcmp(node->varDecl.varType, "int") == 0 || 
                strcmp(node->varDecl.varType, "float") == 0) {
                if (lookupSymbol(symTab, node->varDecl.varName, 1) == NULL) {
                    addSymbol(symTab, node->varDecl.varName, node->varDecl.varType, 0, NULL, NULL);
                } else {
                    fprintf(stderr, "Semantic error: Variable %s is already declared\n", node->varDecl.varName);
                }
            } else {
                fprintf(stderr, "Semantic error: Unsupported variable type %s\n", node->varDecl.varType);
            }
            break;


        case NodeType_FuncDeclList:
            printf("Processing Function Declaration List\n");
            semanticAnalysis(node->funcDeclList.funcDecl, symTab);
            semanticAnalysis(node->funcDeclList.funcDeclList, symTab);
            break;
            
        case NodeType_IfStmt: {
            printf("Generating TAC for if statement\n");

            // Generate TAC for the condition
            TAC* conditionTAC = generateTACForExpr(node->ifStmt.condition);
            if (!conditionTAC || !conditionTAC->result) {
                fprintf(stderr, "Error: Failed to generate TAC for if condition.\n");
                return;
            }

        char* endLabel = createLabel();

        if (!node->ifStmt.falseBranch) {
            // if (condition) { ... } with no else
            // ifFalse conditionResult goto endLabel
            TAC* ifFalseTac = (TAC*)malloc(sizeof(TAC));
            ifFalseTac->op = strdup("ifFalse");
            ifFalseTac->arg1 = strdup(conditionTAC->result);
            ifFalseTac->arg2 = NULL;
            ifFalseTac->result = strdup(endLabel);
            ifFalseTac->next = NULL;
            appendTAC(&tacHead, ifFalseTac);

            // then-block
            semanticAnalysis(node->ifStmt.trueBranch, symTab);

            // end label
            appendLabelTAC(endLabel);
        } else {
            // if (condition) { ... } else { ... }
            char* elseLabel = createLabel();

            // ifFalse conditionResult goto elseLabel
            TAC* ifFalseTac = (TAC*)malloc(sizeof(TAC));
            ifFalseTac->op = strdup("ifFalse");
            ifFalseTac->arg1 = strdup(conditionTAC->result);
            ifFalseTac->arg2 = NULL;
            ifFalseTac->result = strdup(elseLabel);
            ifFalseTac->next = NULL;
            appendTAC(&tacHead, ifFalseTac);

            // then-block
            semanticAnalysis(node->ifStmt.trueBranch, symTab);

            // goto endLabel
            TAC* gotoEnd = (TAC*)malloc(sizeof(TAC));
            gotoEnd->op = strdup("goto");
            gotoEnd->arg1 = NULL;
            gotoEnd->arg2 = NULL;
            gotoEnd->result = strdup(endLabel);
            gotoEnd->next = NULL;
            appendTAC(&tacHead, gotoEnd);

            // else label
            appendLabelTAC(elseLabel);
            semanticAnalysis(node->ifStmt.falseBranch, symTab);

            // end label
            appendLabelTAC(endLabel);
        }

    break;
}

        
case NodeType_ComparisonOp:
            printf("Processing Comparison Operation: %s\n", node->comparisonOp.operator);
            // Perform semantic analysis on the left and right operands
            semanticAnalysis(node->comparisonOp.left, symTab);
            semanticAnalysis(node->comparisonOp.right, symTab);
            break;
case NodeType_WhileStmt: {
            printf("Processing While Statement\n");

            // Generate labels for the start and end of the loop
            char* startLabel = createLabel();
            char* endLabel = createLabel();

            // Append the start label to the TAC
            appendLabelTAC(startLabel);

            // Perform semantic analysis and generate TAC for the condition
            TAC* conditionTAC = generateTACForExpr(node->whileStmt.condition);
            if (!conditionTAC || !conditionTAC->result) {
                fprintf(stderr, "Error: Failed to generate TAC for while loop condition.\n");
                return;
            }

            // Create the conditional jump to exit the loop if the condition is false
            TAC* ifFalseJump = (TAC*)malloc(sizeof(TAC));
            ifFalseJump->op = strdup("ifFalse");
            ifFalseJump->arg1 = strdup(conditionTAC->result); // The result of the condition evaluation
            ifFalseJump->arg2 = NULL;
            ifFalseJump->result = strdup(endLabel); // Jump to endLabel if condition is false
            ifFalseJump->next = NULL;
            appendTAC(&tacHead, ifFalseJump);

            // Perform semantic analysis on the loop body, generating TAC as needed
            semanticAnalysis(node->whileStmt.body, symTab);

            // Unconditional jump back to the start of the loop
            TAC* gotoStart = (TAC*)malloc(sizeof(TAC));
            gotoStart->op = strdup("goto");
            gotoStart->arg1 = NULL;
            gotoStart->arg2 = NULL;
            gotoStart->result = strdup(startLabel); // Jump back to the start label
            gotoStart->next = NULL;
            appendTAC(&tacHead, gotoStart);

            // Append the end label to the TAC
            appendLabelTAC(endLabel);

            break;
        }




        default:
            fprintf(stderr, "Unknown Node Type: %d\n", node->type);
            break;
    }
}

TAC* generateTACForExpr(ASTNode* expr) {
    if (!expr) return NULL;

    printf("Generating TAC for Node Type: %d\n", expr->type);
    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    TAC* leftTAC = NULL;
    TAC* rightTAC = NULL;
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
            // No need to generate a new instruction for a simple ID
            free(instruction); // Free the initially allocated instruction
            TAC* temp = (TAC*)malloc(sizeof(TAC));
            temp->result = strdup(expr->simpleID.name);
            temp->op = NULL;
            temp->arg1 = NULL;
            temp->arg2 = NULL;
            temp->next = NULL;
            return temp;
        }

      case NodeType_ArrayAccess: {
        printf("Generating TAC for array access\n");
        char* arrayName = strdup(expr->arrayAccess.arrayName);
        ASTNode* indexNode = expr->arrayAccess.index;
        
        // Ensure the index is a constant integer
        if (indexNode->type != NodeType_SimpleExpr || indexNode->simpleExpr.valueType != 'i') {
            fprintf(stderr, "Error: Non-constant index in array access not supported\n");
            free(instruction);
            free(arrayName);
            return NULL;
        }
        int indexValue = indexNode->simpleExpr.intValue;
        
        // Create the variable name arr2
        char* arrayElementName = (char*)malloc(strlen(arrayName) + 20);
        snprintf(arrayElementName, strlen(arrayName) + 20, "%s%d", arrayName, indexValue);
        
        free(arrayName);
        free(instruction); // Since we're not generating an instruction here
        
        // Return a TAC node with the result as arr2
        TAC* temp = (TAC*)malloc(sizeof(TAC));
        temp->result = arrayElementName; // arr2
        temp->op = NULL;
        temp->arg1 = NULL;
        temp->arg2 = NULL;
        temp->next = NULL;
        return temp;
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
            int argCount = 0;

            // Collect arguments into a list
            ASTNode* argList[10]; // Adjust size as needed
            while (argNode && argCount < 10) {
                argList[argCount++] = argNode->argList.arg;
                argNode = argNode->argList.next;
            }

            // Generate assignments for arguments
            for (int i = 0; i < argCount; i++) {
                TAC* argTAC = generateTACForExpr(argList[i]);
                if (!argTAC || !argTAC->result) {
                    fprintf(stderr, "Error: TAC generation failed for function call argument.\n");
                    free(instruction);
                    return NULL;
                }

                char paramName[20];
                snprintf(paramName, sizeof(paramName), "param%d", i + 1);

                // Generate assignment: paramX = arg
                TAC* assignArg = (TAC*)malloc(sizeof(TAC));
                assignArg->op = strdup("=");
                assignArg->arg1 = strdup(argTAC->result);
                assignArg->result = strdup(paramName);
                assignArg->next = NULL;
                appendTAC(&tacHead, assignArg);
            }

            // Generate the call instruction
            instruction->op = strdup("call");
            instruction->arg1 = strdup(expr->funcCall.name);
            instruction->result = createTempVar(); // Temporary variable to hold function return value
            instruction->next = NULL;
            appendTAC(&tacHead, instruction);

            return instruction; // Return the TAC instruction containing the function call result
        }
     
            case NodeType_BinOp: {
            printf("Generating TAC for binary operation: %c\n", expr->binOp.operator);

            TAC* leftTAC = generateTACForExpr(expr->binOp.left);
            TAC* rightTAC = generateTACForExpr(expr->binOp.right);

            char* leftResult = leftTAC ? leftTAC->result : createOperand(expr->binOp.left);
            char* rightResult = rightTAC ? rightTAC->result : createOperand(expr->binOp.right);

            TAC* tac = (TAC*)malloc(sizeof(TAC));
            tac->op = strdup(
                (expr->binOp.operator == '+') ? "+" :
                (expr->binOp.operator == '-') ? "-" :
                (expr->binOp.operator == '*') ? "*" :
                (expr->binOp.operator == '/') ? "/" :
                NULL
            );

            if (!tac->op) {
                fprintf(stderr, "Unsupported binary operator: %c\n", expr->binOp.operator);
                free(leftResult);
                free(rightResult);
                free(tac);
                return NULL;
            }

            tac->arg1 = leftResult;
            tac->arg2 = rightResult;
            tac->result = createTempVar();
            tac->next = NULL;

            appendTAC(&tacHead, tac);
            printf("Generated TAC: %s = %s %s %s\n", tac->result, tac->arg1, tac->op, tac->arg2);

            return tac;
        }

        case NodeType_ComparisonOp: {
    printf("Generating TAC for comparison operation: %s\n", expr->comparisonOp.operator);

    char* leftOperand = createOperand(expr->comparisonOp.left);
    char* rightOperand = createOperand(expr->comparisonOp.right);

    if (!leftOperand || !rightOperand) {
        fprintf(stderr, "Error: Operand generation failed for ComparisonOp.\n");
        free(leftOperand);
        free(rightOperand);
        return NULL;
    }

    TAC* tac = (TAC*)malloc(sizeof(TAC));
    if (!tac) {
        fprintf(stderr, "Memory allocation failed for TAC node.\n");
        free(leftOperand);
        free(rightOperand);
        return NULL;
    }

    // Map multi-character operators
    const char* operatorStr = 
        (strcmp(expr->comparisonOp.operator, ">=") == 0) ? ">=" :
        (strcmp(expr->comparisonOp.operator, "<=") == 0) ? "<=" :
        (strcmp(expr->comparisonOp.operator, "==") == 0) ? "==" :
        (strcmp(expr->comparisonOp.operator, "!=") == 0) ? "!=" :
        (strcmp(expr->comparisonOp.operator, ">") == 0) ? ">" :
        (strcmp(expr->comparisonOp.operator, "<") == 0) ? "<" :
        NULL;

    if (!operatorStr) {
        fprintf(stderr, "Unsupported comparison operator: %s\n", expr->comparisonOp.operator);
        free(leftOperand);
        free(rightOperand);
        free(tac);
        return NULL;
    }

    tac->op = strdup(operatorStr);
    tac->arg1 = leftOperand;
    tac->arg2 = rightOperand;
    tac->result = createTempVar();
    tac->next = NULL;

    appendTAC(&tacHead, tac);
    printf("Generated TAC for comparison: %s = %s %s %s\n", tac->result, tac->arg1, tac->op, tac->arg2);
    return tac;
}


        case NodeType_LogicalOp: {
            printf("Generating TAC for logical operation: %s\n", expr->logicalOp.operator);

            TAC* leftTAC = generateTACForExpr(expr->logicalOp.left);
            TAC* rightTAC = generateTACForExpr(expr->logicalOp.right);

            char* leftResult = leftTAC ? leftTAC->result : createOperand(expr->logicalOp.left);
            char* rightResult = rightTAC ? rightTAC->result : createOperand(expr->logicalOp.right);

            TAC* tac = (TAC*)malloc(sizeof(TAC));
            tac->op = strdup(expr->logicalOp.operator);  // "&&" or "||"

            tac->arg1 = leftResult;
            tac->arg2 = rightResult;
            tac->result = createTempVar();
            tac->next = NULL;

            appendTAC(&tacHead, tac);
            printf("Generated TAC for logical operation: %s = %s %s %s\n", tac->result, tac->arg1, tac->op, tac->arg2);

            return tac;
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
            if (!buffer) {
                fprintf(stderr, "Memory allocation failed for SimpleExpr operand.\n");
                return NULL;
            }
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
            printf("Created operand for SimpleID: %s\n", node->simpleID.name);
            return strdup(node->simpleID.name);

        case NodeType_BinOp:
        case NodeType_LogicalOp:
        case NodeType_ComparisonOp:
        case NodeType_FuncCall: {
            TAC* exprTAC = generateTACForExpr(node);
            if (!exprTAC || !exprTAC->result) {
                fprintf(stderr, "Error generating TAC for expression\n");
                return NULL;
            }
            printf("Generated operand for complex expression: %s\n", exprTAC->result);
            return exprTAC->result;
        }
        case NodeType_BoolLiteral: {
        char* buffer = malloc(6); // Enough for "true" or "false"
        snprintf(buffer, 6, "%s", node->boolLiteral.value ? "true" : "false");
        return buffer;
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

        // Write the TAC instruction to the file
        if (current->op != NULL) {
            if (strcmp(current->op, "write") == 0) {
                // Handle 'write' instruction
                fprintf(file, "write %s\n", current->arg1);
            } else if (strcmp(current->op, "return") == 0) {
                // Handle 'return' instruction
                if (current->arg1) {
                    fprintf(file, "return %s\n", current->arg1);
                } else {
                    fprintf(file, "return\n");
                }
            } else if (strcmp(current->op, "ifFalse") == 0) {
                // Handle 'ifFalse' conditional jump
                fprintf(file, "ifFalse %s goto %s\n", current->arg1, current->result);
            } else if (strcmp(current->op, "if") == 0) {
                // Handle 'if' conditional jump
                fprintf(file, "if %s goto %s\n", current->arg1, current->arg2);
            } else if (strcmp(current->op, "goto") == 0) {
                // Handle 'goto' jump
                fprintf(file, "goto %s\n", current->result);
            } else if (strcmp(current->op, "label") == 0) {
                // Handle 'label'
                fprintf(file, "label %s\n", current->arg1);
            } else if (strcmp(current->op, "=") == 0) {
                // Handle simple assignment
                fprintf(file, "%s = %s\n", current->result, current->arg1);
            } else if (strcmp(current->op, "call") == 0) {
                // Handle function call
                fprintf(file, "%s = call %s\n", current->result, current->arg1);
            } else if (strcmp(current->op, "push") == 0) {
                // Handle argument push
                fprintf(file, "push %s\n", current->arg1);
            } else if (strcmp(current->op, "array_access") == 0) {
                // Handle array access
                fprintf(file, "%s = %s[%s]\n", current->result, current->arg1, current->arg2);
            } else if (current->arg2 == NULL) {
                // Handle unary operations like '!'
                fprintf(file, "%s = %s %s\n", current->result, current->op, current->arg1);
            } else {
                // General case for binary operations
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
    // General debug output for TAC instructions
    printf("Appended TAC: %s %s %s -> %s\n",
           newInstruction->op ? newInstruction->op : "(null)",
           newInstruction->arg1 ? newInstruction->arg1 : "(null)",
           newInstruction->arg2 ? newInstruction->arg2 : "(null)",
           newInstruction->result ? newInstruction->result : "(null)");

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


bool isBooleanType(ASTNode* node, SymbolTable* symTab) {
    if (!node) return false;

    if (node->type == NodeType_BoolLiteral) return true; // Literal true/false
    if (node->type == NodeType_SimpleID) {
        Symbol* symbol = lookupSymbol(symTab, node->simpleID.name, 1);
        return symbol && strcmp(symbol->type, "bool") == 0;
    }
    return false;
}


bool isNumericType(ASTNode* node, SymbolTable* symTab) {
    if (node->type == NodeType_SimpleExpr) {
        return node->simpleExpr.valueType == 'i' || node->simpleExpr.valueType == 'f';
    }
    if (node->type == NodeType_SimpleID) {
        Symbol* symbol = lookupSymbol(symTab, node->simpleID.name, 1);
        return symbol && (strcmp(symbol->type, "int") == 0 || strcmp(symbol->type, "float") == 0);
    }
    return false;
}

bool areComparableTypes(ASTNode* left, ASTNode* right, SymbolTable* symTab) {
    if (!left || !right) return false;

    const char* leftType = (left->type == NodeType_SimpleID) 
        ? lookupSymbol(symTab, left->simpleID.name, 1)->type 
        : getNodeType(left);

    const char* rightType = (right->type == NodeType_SimpleID) 
        ? lookupSymbol(symTab, right->simpleID.name, 1)->type 
        : getNodeType(right);

    return leftType && rightType && strcmp(leftType, rightType) == 0;
}

const char* getNodeType(ASTNode* node) {
    if (!node) return NULL;

    switch (node->type) {
        case NodeType_SimpleExpr:
            return (node->simpleExpr.valueType == 'i') ? "int" :
                   (node->simpleExpr.valueType == 'f') ? "float" : NULL;
        case NodeType_BoolLiteral:
            return "bool";
        default:
            return NULL; // Handle additional cases as needed
    }
}
char* createLabel() {
    static int labelCounter = 0;
    char* label = (char*)malloc(10);
    snprintf(label, 10, "L%d", labelCounter++);
    return label;
}
void appendLabelTAC(const char* label) {
    TAC* labelTAC = (TAC*)malloc(sizeof(TAC));
    labelTAC->op = strdup("label");
    labelTAC->arg1 = strdup(label);
    labelTAC->arg2 = NULL;
    labelTAC->result = NULL;
    labelTAC->next = NULL;
    appendTAC(&tacHead, labelTAC);
}


