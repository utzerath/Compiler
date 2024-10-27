#ifndef AST_H
#define AST_H

#include "symbolTable.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum {
    NodeType_Program,
    NodeType_VarDeclList,
    NodeType_VarDecl,
    NodeType_SimpleExpr,
    NodeType_SimpleID,
    NodeType_BinOp,
    NodeType_AssignStmt,
    NodeType_WriteStmt,
    NodeType_ArrayDecl,
    NodeType_ArrayAccess,
    NodeType_StmtList,
    NodeType_BlockStmt,
    NodeType_FuncDecl,
    NodeType_FuncDeclList,
    NodeType_ParamList,
    NodeType_FuncCall,
    NodeType_ReturnStmt,
    NodeType_Param,
    NodeType_ArgList
} NodeType;

// Single typedef and struct definition for ASTNode
typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            struct ASTNode* varDeclList;
            struct ASTNode* stmtList;
            struct ASTNode* funcDeclList;
        } program;

        struct {
            struct ASTNode* varDecl;
            struct ASTNode* varDeclList;
        } varDeclList;

        struct {
            char* varType;
            char* varName;
            int size;
        } varDecl;

        struct {
            int intValue;
            float floatValue;
            char valueType;
        } simpleExpr;

        struct {
            char* name;
        } simpleID;

        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            char operator;
        } binOp;

        struct {
            struct ASTNode* lvalue;
            struct ASTNode* expr;
        } assignStmt;

        struct {
            struct ASTNode* expr;
        } writeStmt;

        struct {
            struct ASTNode* stmtList;
        } blockStmt;

        struct {
            struct ASTNode* stmt;
            struct ASTNode* stmtList;
        } stmtList;

        struct {
            char* varType;
            char* varName;
            int size;
        } arrayDecl;
        
        struct {
            char* arrayName;
            struct ASTNode* index;
        } arrayAccess;

        struct {
            struct ASTNode* paramList; // This will hold a linked list of parameters as ASTNodes
            char* returnType;          // Return type for function declaration
            char* name;                // Function name
            struct ASTNode* stmtList;  // Function body
            struct ASTNode* returnStmt; // Return statement if any
        } funcDecl;
        struct {
            struct ASTNode* funcDecl;
            struct ASTNode* funcDeclList;
        } funcDeclList;

        struct {
            char* name;
            struct ASTNode* args;
        } funcCall;

        struct {
            struct ASTNode* param;
            struct ASTNode* paramList;
        } paramList;

        struct {
            char* varType;
            char* varName;
        } param;

        struct {
            struct ASTNode* expr;
        } returnStmt;

        struct {
            struct ASTNode* arg;
            struct ASTNode* next;
        } argList;
    };
} ASTNode;

// Function prototypes for AST handling
ASTNode* createNode(NodeType type);
ASTNode* createIntNode(int value);
ASTNode* createFloatNode(float value);
ASTNode* createFuncDeclNode(char* returnType, char* name, ASTNode* params, ASTNode* stmtList, ASTNode* returnStmt);
ASTNode* createFuncCallNode(char* name, ASTNode* args);
ASTNode* createArgList(ASTNode* arg, ASTNode* next);
ASTNode* createParamList(ASTNode* param);
ASTNode* createParamNode(char* varType, char* varName);
ASTNode* createReturnNode(ASTNode* expr);
// Change this line in AST.h
ASTNode* createParamListNode(ASTNode* param, ASTNode* nextParamList);


void freeAST(ASTNode* node);
void traverseAST(ASTNode* node, int level);

#endif // AST_H
