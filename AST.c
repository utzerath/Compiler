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

    for (int i = 0; i < level; i++) {
        printf("--");
    }

    switch (node->type) {
        case NodeType_Program:
            printf("Program\n");
            traverseAST(node->program.varDeclList, level + 1);
            traverseAST(node->program.stmtList, level + 1);
            break;
        case NodeType_VarDeclList:
            printf("VarDeclList\n");
            traverseAST(node->varDeclList.varDecl, level + 1);
            traverseAST(node->varDeclList.varDeclList, level + 1);
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
        case NodeType_Expr:
            printf("Expr: %c\n", node->expr.operator);
            traverseAST(node->expr.left, level + 1);
            traverseAST(node->expr.right, level + 1);
            break;
        case NodeType_ArrayDecl:
            printf("ArrayDecl: %s %s[%d]\n", node->arrayDecl.varType, node->arrayDecl.varName, node->arrayDecl.size);
            break;
        case NodeType_ArrayAccess:
            printf("ArrayAccess: %s\n", node->arrayAccess.arrayName);
            traverseAST(node->arrayAccess.index, level + 1);
            break;
        case NodeType_StmtList:
            printf("StmtList\n");
            traverseAST(node->stmtList.stmt, level + 1);
            traverseAST(node->stmtList.stmtList, level + 1);
            break;
        case NodeType_AssignStmt:
            printf("AssignStmt\n");
            traverseAST(node->assignStmt.lvalue, level + 1);
            traverseAST(node->assignStmt.expr, level + 1);
            break;
        case NodeType_WriteStmt:
            printf("WriteStmt\n");
            traverseAST(node->writeStmt.expr, level + 1);
            break;
        case NodeType_BlockStmt:
            printf("BlockStmt\n");
            traverseAST(node->blockStmt.stmtList, level + 1);
            break;
        // Add cases for other node types as needed
        default:
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
            free(node->varDecl.varType);
            free(node->varDecl.varName);
            break;
        case NodeType_SimpleExpr:
            break;
        case NodeType_SimpleID:
            free(node->simpleID.name);
            break;
        case NodeType_Expr:
            freeAST(node->expr.left);
            freeAST(node->expr.right);
            break;
        case NodeType_ArrayDecl:
            free(node->arrayDecl.varType);
            free(node->arrayDecl.varName);
            break;
        case NodeType_ArrayAccess:
            free(node->arrayAccess.arrayName);
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
        case NodeType_BinOp:
            freeAST(node->binOp.left);
            freeAST(node->binOp.right);
            break;
        case NodeType_WriteStmt:
            freeAST(node->writeStmt.expr);
            break;
        case NodeType_BlockStmt:
            freeAST(node->blockStmt.stmtList);
            break;
        default:
            break;
    }
    free(node);
}

ASTNode* createNode(NodeType type) {
    ASTNode* newNode = (ASTNode*)malloc(sizeof(ASTNode));
    if (newNode == NULL) {
        return NULL;
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
            newNode->simpleExpr.intValue = 0;   // Initialize both int and float
            newNode->simpleExpr.floatValue = 0.0;
            newNode->simpleExpr.valueType = 'i';  // Default to int
            break;
        case NodeType_SimpleID:
            newNode->simpleID.name = NULL;
            break;
        case NodeType_Expr:
            newNode->expr.operator = '\0';
            newNode->expr.left = NULL;
            newNode->expr.right = NULL;
            break;
        case NodeType_StmtList:
            newNode->stmtList.stmt = NULL;
            newNode->stmtList.stmtList = NULL;
            break;
        case NodeType_AssignStmt:
            newNode->assignStmt.lvalue = NULL;
            newNode->assignStmt.expr = NULL;
            break;
        case NodeType_BinOp:
            newNode->binOp.operator = '\0';
            newNode->binOp.left = NULL;
            newNode->binOp.right = NULL;
            break;
        case NodeType_WriteStmt:
            newNode->writeStmt.expr = NULL;
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
        case NodeType_BlockStmt:
            newNode->blockStmt.stmtList = NULL;
            break;
        default:
            break;
    }

    return newNode;
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
