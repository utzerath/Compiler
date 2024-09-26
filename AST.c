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
            printIndent(level);
            printf("Program\n");
            traverseAST(node->program.varDeclList, level + 1);
            traverseAST(node->program.stmtList, level + 1);
            break;
        case NodeType_VarDeclList:
            printIndent(level);
            printf("VarDeclList\n");
            traverseAST(node->varDeclList.varDecl, level + 1);
            traverseAST(node->varDeclList.varDeclList, level + 1);
            break;
        case NodeType_VarDecl:
            printIndent(level);
            printf("VarDecl: %s %s\n", node->varDecl.varType, node->varDecl.varName);
            break;
        case NodeType_SimpleExpr:
            printIndent(level);
            printf("%d\n", node->simpleExpr.number);
            break;
        case NodeType_SimpleID:
            printIndent(level);
            printf("%s\n", node->simpleID.name);
            break;
        case NodeType_Expr:
            printIndent(level);
            printf("Expr: %c\n", node->expr.operator);
            traverseAST(node->expr.left, level + 1);
            traverseAST(node->expr.right, level + 1);
            break;
        case NodeType_StmtList:
            printIndent(level);
            printf("StmtList\n");
            traverseAST(node->stmtList.stmt, level + 1);
            traverseAST(node->stmtList.stmtList, level + 1);
            break;
        case NodeType_AssignStmt:
            printIndent(level);
            printf("Stmt: %s = ", node->assignStmt.varName);
            traverseAST(node->assignStmt.expr, level + 1);
            break;
        case NodeType_BinOp:
            printIndent(level);
            printf("BinOp: %c\n", node->binOp.operator);
            traverseAST(node->binOp.left, level + 1);
            traverseAST(node->binOp.right, level + 1);
            break;
        default:
            printIndent(level);
            printf("Unknown node type\n");
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
        default:
            break;
    }

    return newNode;
}
