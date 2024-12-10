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
    NodeType_ArgList,
    NodeType_MainFunc,
    NodeType_IfStmt,          // New node type for if statements
    NodeType_LogicalOp,       // New node type for logical operations
    NodeType_ComparisonOp,   // New node type for comparison operations
    NodeType_BoolLiteral,
    NodeType_WhileStmt
} NodeType;

// Single typedef and struct definition for ASTNode
typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            struct ASTNode* varDeclList;
            struct ASTNode* mainFunc;
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
            int operator;
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
            struct ASTNode* stmtList;  // Main function's statement list
        } mainFunc;

        struct {
            struct ASTNode* paramList;
            char* returnType;
            char* name;
            struct ASTNode* stmtList;
            struct ASTNode* returnStmt;
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

        // New fields for `if` statements
        struct {
            struct ASTNode* condition;    // The expression condition
            struct ASTNode* trueBranch;   // Statement to execute if true
            struct ASTNode* falseBranch;  // Optional else statement
        } ifStmt;

        // New fields for logical operations (e.g., `&&`, `||`, `!`)
        struct {
            struct ASTNode* left;         // Left operand
            struct ASTNode* right;        // Right operand (NULL if unary, e.g., `!`)
            char* operator;               // The operator as a string (e.g., "&&")
        } logicalOp;

        // New fields for comparison operations (e.g., `==`, `<`, `>`)
        struct {
            struct ASTNode* left;         // Left operand
            struct ASTNode* right;        // Right operand
            char* operator;               // The operator as a string (e.g., "==")
        } comparisonOp;

        // New fields for boolean literals (true or false)
        struct {
            int value;    // 1 for true, 0 for false
        } boolLiteral;
        struct {
            struct ASTNode* condition; // Condition for the while loop
            struct ASTNode* body;      // Body of the loop (statements inside the loop)
        } whileStmt;

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
ASTNode* createParamListNode(ASTNode* param, ASTNode* nextParamList);

// New function prototypes for conditional, logical, and comparison nodes
ASTNode* createIfNode(ASTNode* condition, ASTNode* trueBranch, ASTNode* falseBranch);
ASTNode* createLogicalNode(const char* operator, ASTNode* left, ASTNode* right);
ASTNode* createComparisonNode(const char* operator, ASTNode* left, ASTNode* right);
ASTNode* createIDNode(const char* name);
ASTNode* createBoolNode(int value);
ASTNode* createWhileNode(ASTNode* condition, ASTNode* body);

void freeAST(ASTNode* node);
void traverseAST(ASTNode* node, int level);

#endif // AST_H