#include "AST.h"
#include "symbolTable.h"  // Include symbol table to access Param
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int indentValue = 2;

void printIndent(int level) {
    for (int i = 0; i < level - 1; i++) {
        printf("--");
    }
}

void traverseAST(ASTNode* node, int level) {
    if (node == NULL) {
        return; // Base case: do nothing if the node is NULL
    }

    // Print indentation based on the current level
    for (int i = 0; i < level; i++) {
        printf("--");
    }

    // Print the current node's type
    //printf("Traversing node type %d at level %d\n", node->type, level);

    // Switch on the type of node to handle specific cases
    switch (node->type) {
        case NodeType_Program:
            printf("Program\n");
            traverseAST(node->program.varDeclList, level + 1); // Traverse variable declarations
            traverseAST(node->program.funcDeclList, level + 1); // Traverse function declarations
            if (node->program.mainFunc != NULL) {
                traverseAST(node->program.mainFunc, level + 1); // Traverse main function node
            }
            if (node->program.stmtList != NULL) {
                traverseAST(node->program.stmtList, level + 1); // Traverse statements
            }
            break;
        case NodeType_MainFunc:
            printf("Main Function\n");
            traverseAST(node->mainFunc.stmtList, level + 1); // Traverse statements inside main
            break;
        case NodeType_VarDeclList:
            printf("VarDeclList\n");
            traverseAST(node->varDeclList.varDecl, level + 1); // Traverse each variable declaration
            traverseAST(node->varDeclList.varDeclList, level + 1); // Traverse the next variable declarations
            break;

        case NodeType_VarDecl:
            printf("VarDecl: %s %s\n", node->varDecl.varType, node->varDecl.varName);
            break;

        case NodeType_SimpleExpr:
            if (node->simpleExpr.valueType == 'i') {
                printf("Int: %d\n", node->simpleExpr.intValue);
            } else if (node->simpleExpr.valueType == 'f') {
                printf("Float: %f\n", node->simpleExpr.floatValue);
            }
            break;

        case NodeType_SimpleID:
            printf("Identifier: %s\n", node->simpleID.name);
            break;

        case NodeType_BinOp:
            printf("BinOp: %c\n", node->binOp.operator);
            traverseAST(node->binOp.left, level + 1); // Traverse left operand
            traverseAST(node->binOp.right, level + 1); // Traverse right operand
            break;

        case NodeType_ArrayDecl:
            printf("ArrayDecl: %s %s[%d]\n", node->arrayDecl.varType, node->arrayDecl.varName, node->arrayDecl.size);
            break;

        case NodeType_ArrayAccess:
            printf("ArrayAccess: %s\n", node->arrayAccess.arrayName);
            if (node->arrayAccess.index != NULL) { // Check if index is valid before traversing
                traverseAST(node->arrayAccess.index, level + 1);
            } else {
                printf("Warning: Array access index is NULL.\n");
            }
            break;

        case NodeType_StmtList:
            printf("StmtList\n");
            if (node->stmtList.stmt != NULL) {
                traverseAST(node->stmtList.stmt, level + 1); // Traverse the first statement
            }
            if (node->stmtList.stmtList != NULL) {
                traverseAST(node->stmtList.stmtList, level); // Traverse the rest of the statements
            }
            break;

        case NodeType_AssignStmt:
            printf("AssignStmt\n");
            if (node->assignStmt.lvalue != NULL) {
                traverseAST(node->assignStmt.lvalue, level + 1); // Traverse left-hand side
            } else {
                printf("Warning: AssignStmt lvalue is NULL.\n");
            }
            if (node->assignStmt.expr != NULL) {
                traverseAST(node->assignStmt.expr, level + 1); // Traverse right-hand side
            } else {
                printf("Warning: AssignStmt expr is NULL.\n");
            }
            break;

        case NodeType_WriteStmt:
            printf("WriteStmt\n");
            if (node->writeStmt.expr != NULL) {
                traverseAST(node->writeStmt.expr, level + 1); // Traverse the expression being written
            } else {
                printf("Warning: WriteStmt expr is NULL.\n");
            }
            break;

        case NodeType_BlockStmt:
            printf("BlockStmt\n");
            if (node->blockStmt.stmtList != NULL) {
                traverseAST(node->blockStmt.stmtList, level + 1); // Traverse statements within the block
            } else {
                printf("Warning: BlockStmt stmtList is NULL.\n");
            }
            break;

        case NodeType_Param:
            printf("Parameter: %s of type %s\n", node->param.varName, node->param.varType);
            break;

        case NodeType_ParamList:
            printf("Parameter List\n");
            
            // Check if the current parameter node is not NULL
            if (node->paramList.param != NULL) {
                traverseAST(node->paramList.param, level + 1); // Traverse the current parameter
            } else {
                printf("Warning: Parameter List param is NULL.\n");
            }

            // Traverse the next parameter in the list, if it exists
            if (node->paramList.paramList != NULL) {
                traverseAST(node->paramList.paramList, level + 1); // Traverse the next parameter list
            } else {
                printf("Warning: No more parameters in the Parameter List.\n");
            }
            break;
        case NodeType_FuncDeclList: {
        printf("Function Declaration List:\n");
        ASTNode* currentList = node;
        while (currentList != NULL) {
            ASTNode* funcDeclNode = currentList->funcDeclList.funcDecl;
            if (funcDeclNode != NULL) {
                traverseAST(funcDeclNode, level + 1); // Traverse the current FuncDecl
            }
            currentList = currentList->funcDeclList.funcDeclList; // Move to the next FuncDeclList node
        }
        break;
}


        case NodeType_FuncDecl: {
            printf("Function Declaration: %s, Return Type: %s\n", node->funcDecl.name, node->funcDecl.returnType);
            
            if (node->funcDecl.paramList != NULL) {
                traverseAST(node->funcDecl.paramList, level + 1); // Traverse parameters
            } else {
                printf("Warning: Function Declaration paramList is NULL.\n");
            }
            
            if (node->funcDecl.stmtList != NULL) {
                traverseAST(node->funcDecl.stmtList, level + 1); // Traverse statements
            } else {
                printf("Warning: Function Declaration stmtList is NULL.\n");
            }
            
            if (node->funcDecl.returnStmt != NULL) {
                traverseAST(node->funcDecl.returnStmt, level + 1); // Traverse return statement
            } else {
                printf("Warning: Function Declaration returnStmt is NULL.\n");
            }
            break;
        }


        case NodeType_FuncCall:
            printf("Function Call: %s\n", node->funcCall.name);
            if (node->funcCall.args != NULL) {
                traverseAST(node->funcCall.args, level + 1); // Traverse arguments
            } else {
                printf("Warning: Function Call args is NULL.\n");
            }
            break;

        case NodeType_ReturnStmt:
            printf("Return Statement\n");
            if (node->returnStmt.expr) {
                traverseAST(node->returnStmt.expr, level + 1);
            } else {
                printf("Warning: Return Statement expr is NULL.\n");
            }
            break;

        case NodeType_ArgList:
            printf("Argument List\n");
            if (node->argList.arg != NULL) {
                traverseAST(node->argList.arg, level + 1); // Traverse the current argument
            } else {
                printf("Warning: Argument List arg is NULL.\n");
            }
            if (node->argList.next != NULL) {
                traverseAST(node->argList.next, level + 1); // Traverse the next argument
            }
            break;
        case NodeType_IfStmt:
            printf("IfStmt\n");
            printf("-- Condition:\n");
            traverseAST(node->ifStmt.condition, level + 1);
            printf("-- True Branch:\n");
            traverseAST(node->ifStmt.trueBranch, level + 1);
            if (node->ifStmt.falseBranch != NULL) {
                printf("-- False Branch:\n");
                traverseAST(node->ifStmt.falseBranch, level + 1);
            }
            break;

        case NodeType_LogicalOp:
            printf("LogicalOp: %s\n", node->logicalOp.operator);
            printf("-- Left Operand:\n");
            traverseAST(node->logicalOp.left, level + 1);
            if (node->logicalOp.right != NULL) {
                printf("-- Right Operand:\n");
                traverseAST(node->logicalOp.right, level + 1);
            }
            break;

        case NodeType_ComparisonOp:
            printf("ComparisonOp: %s\n", node->comparisonOp.operator);
            printf("-- Left Operand:\n");
            traverseAST(node->comparisonOp.left, level + 1);
            printf("-- Right Operand:\n");
            traverseAST(node->comparisonOp.right, level + 1);
            break;

        case NodeType_BoolLiteral:
            printf("Bool: %s\n", node->boolLiteral.value ? "true" : "false");
            break;

        case NodeType_WhileStmt:
            printf("WhileStmt\n");
            printf("-- Condition:\n");
            traverseAST(node->whileStmt.condition, level + 1); // Traverse the condition of the while loop
            printf("-- Body:\n");
            traverseAST(node->whileStmt.body, level + 1); // Traverse the body (statements inside the loop)
            break;

            
        default:
            printf("Unknown node type: %d\n", node->type); // Print unknown node types
            return; // Stop further processing on invalid nodes
        
    }
}

void freeAST(ASTNode* node) {
    if (node == NULL) {
        return; // Base case: do nothing if the node is NULL
    }

    // Free based on the node type
    switch (node->type) {
        case NodeType_Program:
            freeAST(node->program.varDeclList);
            freeAST(node->program.funcDeclList);
            freeAST(node->program.stmtList);
            freeAST(node->program.mainFunc);
            break;
        case NodeType_MainFunc:
            freeAST(node->mainFunc.stmtList); // Free the statement list in main
            break;
        case NodeType_VarDeclList:
            freeAST(node->varDeclList.varDecl);
            freeAST(node->varDeclList.varDeclList);
            break;

        case NodeType_VarDecl:
            free(node->varDecl.varType); // Free allocated string
            free(node->varDecl.varName); // Free allocated string
            break;

        case NodeType_SimpleExpr:
            // Free any members if necessary
            break;

        case NodeType_SimpleID:
            free(node->simpleID.name); // Free allocated string
            break;

        case NodeType_BinOp:
            freeAST(node->binOp.left);
            freeAST(node->binOp.right);
            break;

        case NodeType_ArrayDecl:
            free(node->arrayDecl.varType); // Free allocated string
            free(node->arrayDecl.varName); // Free allocated string
            break;

        case NodeType_ArrayAccess:
            free(node->arrayAccess.arrayName); // Free allocated string
            freeAST(node->arrayAccess.index);
            break;

        case NodeType_StmtList:
            freeAST(node->stmtList.stmt);
            freeAST(node->stmtList.stmtList);
            break;

        case NodeType_AssignStmt:
            freeAST(node->assignStmt.lvalue);
            freeAST(node->assignStmt.expr);
            break;

        case NodeType_WriteStmt:
            freeAST(node->writeStmt.expr);
            break;

        case NodeType_BlockStmt:
            freeAST(node->blockStmt.stmtList);
            break;

        case NodeType_Param:
            free(node->param.varType); // Free allocated string
            free(node->param.varName); // Free allocated string
            break;

        case NodeType_ParamList:
            freeAST(node->paramList.param); // Free the parameter node
            freeAST(node->paramList.paramList); // Free the next parameter list
            break;

        case NodeType_FuncDeclList:
            freeAST(node->funcDeclList.funcDecl); // Free the function declaration
            freeAST(node->funcDeclList.funcDeclList); // Free the next function declaration list
            break;

        case NodeType_FuncDecl:
            free(node->funcDecl.returnType); // Free allocated string
            free(node->funcDecl.name); // Free allocated string
            freeAST(node->funcDecl.paramList); // Free the parameter list
            freeAST(node->funcDecl.stmtList); // Free the statement list
            freeAST(node->funcDecl.returnStmt); // Free the return statement
            break;

        case NodeType_FuncCall:
            free(node->funcCall.name); // Free allocated string
            freeAST(node->funcCall.args); // Free the arguments list
            break;

        case NodeType_ReturnStmt:
            freeAST(node->returnStmt.expr); // Free the expression in the return statement
            break;

        case NodeType_ArgList:
            freeAST(node->argList.arg); // Free the argument node
            freeAST(node->argList.next); // Free the next argument
            break;
        case NodeType_WhileStmt:  // Add case for WhileStmt
            printf("Freeing WhileStmt\n");
            freeAST(node->whileStmt.condition); // Free the condition expression
            freeAST(node->whileStmt.body); // Free the body (statements inside the loop)
            break;
        default:
            printf("Unknown node type: %d\n", node->type);
            break;
    }

    // Finally free the node itself
    free(node);
}
ASTNode* createNode(NodeType type) {
    ASTNode* newNode = (ASTNode*)malloc(sizeof(ASTNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed for ASTNode\n");
        exit(EXIT_FAILURE); // Handle memory allocation failure
    }

    newNode->type = type; // Set the type of the node
    
    printf("Created node of type %d at address %p\n", type, (void*)newNode);

    // Initialize all members to NULL or default values to avoid undefined behavior
    // Initialize the common node types
    switch (type) {
        case NodeType_Program:
            newNode->program.varDeclList = NULL;
            newNode->program.funcDeclList = NULL;
            newNode->program.stmtList = NULL;
            newNode->program.mainFunc = NULL;
            break;
        case NodeType_MainFunc:
            newNode->mainFunc.stmtList = NULL; // Initialize stmtList for main function
            break;
        case NodeType_VarDeclList:
            newNode->varDeclList.varDecl = NULL;
            newNode->varDeclList.varDeclList = NULL;
            break;

        case NodeType_VarDecl:
            newNode->varDecl.varType = NULL;
            newNode->varDecl.varName = NULL;
            break;

        case NodeType_SimpleExpr:
            newNode->simpleExpr.intValue = 0;
            newNode->simpleExpr.floatValue = 0.0;
            newNode->simpleExpr.valueType = 'i'; // Default to integer
            break;

        case NodeType_SimpleID:
            newNode->simpleID.name = NULL;
            break;

        case NodeType_BinOp:
            newNode->binOp.operator = '\0';
            newNode->binOp.left = NULL;
            newNode->binOp.right = NULL;
            break;

        case NodeType_ArrayDecl:
            newNode->arrayDecl.varType = NULL;
            newNode->arrayDecl.varName = NULL;
            newNode->arrayDecl.size = 0;
            break;

        case NodeType_ArrayAccess:
            newNode->arrayAccess.arrayName = NULL;
            newNode->arrayAccess.index = NULL;
            break;

        case NodeType_StmtList:
            newNode->stmtList.stmt = NULL;
            newNode->stmtList.stmtList = NULL;
            break;

        case NodeType_AssignStmt:
            newNode->assignStmt.lvalue = NULL;
            newNode->assignStmt.expr = NULL;
            break;

        case NodeType_WriteStmt:
            newNode->writeStmt.expr = NULL;
            break;

        case NodeType_BlockStmt:
            newNode->blockStmt.stmtList = NULL;
            break;

        case NodeType_FuncDeclList:
            newNode->funcDeclList.funcDecl = NULL;
            newNode->funcDeclList.funcDeclList = NULL;
            break;

        case NodeType_FuncDecl:
            newNode->funcDecl.returnType = NULL; // Initialize returnType
            newNode->funcDecl.name = NULL;        // Initialize name
            newNode->funcDecl.paramList = NULL;   // Initialize paramList
            newNode->funcDecl.stmtList = NULL;    // Initialize statement list
            newNode->funcDecl.returnStmt = NULL;  // Initialize return statement
            break;

        case NodeType_FuncCall:
            newNode->funcCall.name = NULL; // Initialize function call name
            newNode->funcCall.args = NULL;  // Initialize arguments list
            break;

        case NodeType_ReturnStmt:
            newNode->returnStmt.expr = NULL; // Initialize return statement expression
            break;

        case NodeType_ArgList:
            newNode->argList.arg = NULL; // Initialize argument node
            newNode->argList.next = NULL; // Initialize next argument
            break;
        case NodeType_IfStmt:
            freeAST(newNode->ifStmt.condition);
            freeAST(newNode->ifStmt.trueBranch);
            freeAST(newNode->ifStmt.falseBranch);
            break;

        case NodeType_LogicalOp:
            free(newNode->logicalOp.operator); // Free the operator string
            freeAST(newNode->logicalOp.left);
            freeAST(newNode->logicalOp.right);
            break;

        case NodeType_ComparisonOp:
            free(newNode->comparisonOp.operator); // Free the operator string
            freeAST(newNode->comparisonOp.left);
            freeAST(newNode->comparisonOp.right);
            break;

        case NodeType_WhileStmt:  // New case for while loop
            newNode->whileStmt.condition = NULL; // Initialize condition
            newNode->whileStmt.body = NULL;      // Initialize body (statements inside the loop)
            break;
        // Add cases for any additional node types you've defined
        default:
            break;
    }

    return newNode; // Return the newly created node
}

ASTNode* createIntNode(int value) {
    ASTNode* node = createNode(NodeType_SimpleExpr);
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for ASTNode\n");
        exit(1);
    }
    node->simpleExpr.intValue = value;
    node->simpleExpr.valueType = 'i';  // Mark this node as holding an int
    return node;
}

ASTNode* createFloatNode(float value) {
    ASTNode* node = createNode(NodeType_SimpleExpr);
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for ASTNode\n");
        exit(1);
    }
    node->simpleExpr.floatValue = value;
    node->simpleExpr.valueType = 'f';  // Mark this node as holding a float
    return node;
}

ASTNode* createFuncDeclNode(char* returnType, char* name, ASTNode* paramList, ASTNode* stmtList, ASTNode* returnStmt) {
    ASTNode* node = createNode(NodeType_FuncDecl);
    node->funcDecl.returnType = strdup(returnType);
    node->funcDecl.name = strdup(name);
    node->funcDecl.paramList = paramList; // Make sure this is linked correctly
    node->funcDecl.stmtList = stmtList;
    node->funcDecl.returnStmt = returnStmt;
    return node;
}


ASTNode* createFuncCallNode(char* name, ASTNode* args) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NodeType_FuncCall;
    node->funcCall.name = strdup(name);
    node->funcCall.args = args;
    return node;
}
ASTNode* createArgList(ASTNode* arg, ASTNode* next) {
    ASTNode* node = createNode(NodeType_ArgList);
    node->argList.arg = arg;
    node->argList.next = next;
    return node;
}
ASTNode* createParamNode(char* varType, char* varName) {
    ASTNode* node = createNode(NodeType_Param);
    node->param.varType = strdup(varType); // Copy varType
    node->param.varName = strdup(varName); // Copy varName
    return node;
}

ASTNode* createParamListNode(ASTNode* param, ASTNode* nextParamList) {
    ASTNode* node = createNode(NodeType_ParamList);
    node->paramList.param = param; // Assign the current parameter
    node->paramList.paramList = nextParamList; // Link to the next parameter list
    printf("Creating ParamListNode with param: %p, nextParamList: %p\n", (void*)param, (void*)nextParamList);

    return node;
}



ASTNode* createReturnNode(ASTNode* expr) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = NodeType_ReturnStmt;
    node->returnStmt.expr = expr;
    return node;
}


// Function to create an if-statement node
ASTNode* createIfNode(ASTNode* condition, ASTNode* trueBranch, ASTNode* falseBranch) {
    ASTNode* node = createNode(NodeType_IfStmt);
    node->ifStmt.condition = condition;
    node->ifStmt.trueBranch = trueBranch;
    node->ifStmt.falseBranch = falseBranch;
    printf("Created IfStmt node at %p\n", (void*)node);
    return node;
}

// Function to create a logical operation node (e.g., &&, ||, !)
ASTNode* createLogicalNode(const char* operator, ASTNode* left, ASTNode* right) {
    ASTNode* node = createNode(NodeType_LogicalOp);
    node->logicalOp.operator = strdup(operator);  // Copy the operator string
    node->logicalOp.left = left;
    node->logicalOp.right = right;
    printf("Created LogicalOp node '%s' at %p\n", operator, (void*)node);
    return node;
}

// Function to create a comparison operation node (e.g., ==, <, >)
ASTNode* createComparisonNode(const char* operator, ASTNode* left, ASTNode* right) {
    ASTNode* node = createNode(NodeType_ComparisonOp);
    node->comparisonOp.operator = strdup(operator);  // Copy the operator string
    node->comparisonOp.left = left;
    node->comparisonOp.right = right;
    printf("Created ComparisonOp node '%s' at %p\n", operator, (void*)node);
    return node;
}

// Function to create an identifier node
ASTNode* createIDNode(const char* name) {
    ASTNode* node = createNode(NodeType_SimpleID);
    node->simpleID.name = strdup(name); // Copy the identifier name
    printf("Created IDNode for identifier '%s' at %p\n", name, (void*)node);
    return node;
}

// Function to create a boolean literal node
ASTNode* createBoolNode(int value) {
    ASTNode* node = createNode(NodeType_BoolLiteral);  // Use NodeType_BoolLiteral
    node->boolLiteral.value = value;                   // Set to 1 for true, 0 for false
    return node;
}
ASTNode* createWhileNode(ASTNode* condition, ASTNode* body) {
    ASTNode* node = createNode(NodeType_WhileStmt);
    node->whileStmt.condition = condition;
    node->whileStmt.body = body;
    return node;
}

