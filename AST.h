#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdio.h>

// NodeType enum to differentiate between different 
// kinds of AST nodes
typedef enum {
    NodeType_Program,
    NodeType_VarDeclList,
    NodeType_VarDecl,
    NodeType_SimpleExpr,
    NodeType_SimpleID,
    NodeType_Expr,
    NodeType_StmtList,
    NodeType_AssignStmt,
    NodeType_BinOp,
    NodeType_WriteStmt,
    NodeType_BlockStmt,
    NodeType_ArrayDecl,    // Add this
    NodeType_ArrayAccess   // Add this
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            struct ASTNode* stmtList;  // Block holds a list of statements
        } blockStmt;
        struct {
            char* varType;  // "int" or "float"
            char* varName;
            int size;       // Add this for array size
        } varDecl;

        struct {
            struct ASTNode* varDecl;
            struct ASTNode* varDeclList;
        } varDeclList;

        struct {
            int intValue;      // For integer constants
            float floatValue;  // For float constants
            char valueType;    // 'i' for int, 'f' for float
        } simpleExpr;

        struct {
            char* name;
        } simpleID;

        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            char operator;
        } expr;

        struct {
            struct ASTNode* stmt;
            struct ASTNode* stmtList;
        } stmtList;

        struct {
            struct ASTNode* lvalue;  // Left-hand side can be an ID or an array access
            struct ASTNode* expr;
        } assignStmt;

        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            char operator;
        } binOp;

        struct {
            struct ASTNode* expr;
        } writeStmt;

        struct {
            struct ASTNode* varDeclList;
            struct ASTNode* stmtList;
        } program;

        struct {
            char* varType;  // "int" or "float"
            char* varName;
            int size;       // Array size
        } arrayDecl;        // Add this

        struct {
            char* arrayName;
            struct ASTNode* index;
        } arrayAccess;      // Add this

    };
} ASTNode;

// Function prototypes for AST handling
ASTNode* createNode(NodeType type);
ASTNode* createIntNode(int value);         // Create a node for integer values
ASTNode* createFloatNode(float value);     // Create a node for float values
void freeAST(ASTNode* node);
void traverseAST(ASTNode* node, int level);

#endif // AST_H
