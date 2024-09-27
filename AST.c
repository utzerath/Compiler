#include "AST.h"
#include <stdlib.h>
#include <stdio.h>

int indentValue = 2;

void printIndent(int level) {
    for (int i = 0; i < level - 1; i++) {
        printf("--");
    }
}

void traverseAST(ASTNode* node, int level) {
    if (!node) {
        printf("Nothing to traverse\n");
        return;
    }

    printIndent(level);

    switch (node->type) {
        case NodeType_Program:
            printf("Program\n");
            traverseAST(node->program.varDeclList, level + 1);
            traverseAST(node->program.stmtList, level + 1);
            break;
        case NodeType_VarDeclList:
            printf("VarDeclList\n");
            traverseAST(node->varDeclList.varDecl, level + 1);  // Traverse current VarDecl
            if (node->varDeclList.varDeclList != NULL) {
                traverseAST(node->varDeclList.varDeclList, level + 1);  // Traverse next VarDeclList
            }
            break;
        case NodeType_VarDecl:
            printf("VarDecl: %s %s\n", node->varDecl.varType, node->varDecl.varName);
            break;
        case NodeType_SimpleExpr:
            printf("%d\n", node->simpleExpr.number);
            break;
        case NodeType_SimpleID:
            printf("%s\n", node->simpleID.name);
            break;
        case NodeType_Expr:
            printf("Expr: %c\n", node->expr.operator);
            traverseAST(node->expr.left, level + 1);
            traverseAST(node->expr.right, level + 1);
            break;
        case NodeType_StmtList:
            printf("StmtList\n");
            traverseAST(node->stmtList.stmt, level + 1);
            traverseAST(node->stmtList.stmtList, level + 1);
            break;
        case NodeType_AssignStmt:
            printf("Stmt: %s = \n", node->assignStmt.varName);
            traverseAST(node->assignStmt.expr, level + 1);
            break;
        case NodeType_BinOp:
            printf("BinOp: %c\n", node->binOp.operator);
            traverseAST(node->binOp.left, level + 1);
            traverseAST(node->binOp.right, level + 1);
            break;
        case NodeType_WriteStmt:
            printf("WriteStmt: write %s\n", node->writeStmt.expr->simpleID.name);  // Assuming you're writing a variable
            //traverseAST(node->writeStmt.expr, level + 1);  // Traverse the expression being written this isnt working but will need if we ever add write (write x+ y)
            break;

        default:
            printf("Unknown node type: %d\n", node->type);  // Add more specific debugging
            break;
    }
}


void freeAST(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NodeType_Program:
            freeAST(node->program.varDeclList);
            freeAST(node->program.stmtList);
            break;
        case NodeType_VarDeclList:
            freeAST(node->varDeclList.varDecl);
            freeAST(node->varDeclList.varDeclList);
            break;
        case NodeType_VarDecl:
            free(node->varDecl.varType);  // Only free if dynamically allocated
            free(node->varDecl.varName);  // Only free if dynamically allocated
            break;
        case NodeType_SimpleExpr:
            // No dynamic memory to free in simple expression
            break;
        case NodeType_SimpleID:
            // Free the identifier name if dynamically allocated
            free(node->simpleID.name);
            break;
        case NodeType_Expr:
            freeAST(node->expr.left);
            freeAST(node->expr.right);
            break;
        case NodeType_StmtList:
            freeAST(node->stmtList.stmt);
            freeAST(node->stmtList.stmtList);
            break;
        case NodeType_AssignStmt:
            free(node->assignStmt.varName);  // Only free if dynamically allocated
            free(node->assignStmt.expr);
            break;
        case NodeType_BinOp:
            freeAST(node->binOp.left);
            freeAST(node->binOp.right);
            break;
        case NodeType_WriteStmt:  // <-- Add case for freeing write statements
            free(node->writeStmt.varName);
            break;
        default:
            break;
    }
    free(node);
}

ASTNode* createNode(NodeType type) {
    ASTNode* newNode = (ASTNode*)malloc(sizeof(ASTNode));
    if (newNode == NULL) {
        return NULL;  // Handle memory allocation failure
    }

    newNode->type = type;

    switch (type) {
        case NodeType_Program:
            newNode->program.varDeclList = NULL;
            newNode->program.stmtList = NULL;
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
            newNode->simpleExpr.number = 0;
            break;
        case NodeType_SimpleID:
            newNode->simpleID.name = NULL;
            break;
        case NodeType_Expr:
            newNode->expr.operator = '\0';  // Placeholder value
            newNode->expr.left = NULL;
            newNode->expr.right = NULL;
            break;
        case NodeType_StmtList:
            newNode->stmtList.stmt = NULL;
            newNode->stmtList.stmtList = NULL;
            break;
        case NodeType_AssignStmt:
            newNode->assignStmt.operator = NULL;
            newNode->assignStmt.varName = NULL;
            newNode->assignStmt.expr = NULL;
            break;
        case NodeType_BinOp:
            newNode->binOp.operator = '\0';  // Placeholder value
            newNode->binOp.left = NULL;
            newNode->binOp.right = NULL;
            break;
        case NodeType_WriteStmt:
            newNode->writeStmt.varName = NULL;
            break;
        default:
            break;
    }

    return newNode;
}
