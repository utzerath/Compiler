%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"
#include "symbolTable.h"
#include "semantic.h"
#include "codeGenerator.h"
#include "optimizer.h"

#define TABLE_SIZE 100

extern int yylex();   // Declare yylex, the lexer function
extern int yyparse(); // Declare yyparse, the parser function
extern FILE* yyin;    // Declare yyin, the file pointer for the input file
extern int yylineno;  // Declare yylineno, the line number counter
extern TAC* tacHead;  // Declare the head of the linked list of TAC entries

void yyerror(const char* s);

ASTNode* root = NULL; 
SymbolTable* symTab = NULL;
Symbol* symbol = NULL;
%}

%union {
    int number;
    float float_number;  // Add float support
    char character;      // BinOp returns a char
    char* string;
    char* operator;
    struct ASTNode* ast; // Use ASTNode for representing AST parts, including function params and list nodes
}

%token <string> TYPE
%token <number> NUMBER
%token <float_number> FLOAT_LITERAL  // Add float literal token
%token <char> SEMICOLON
%token <operator> EQ
%token <operator> PLUS
%token <operator> MINUS
%token <operator> MULTIPLY
%token <operator> DIVIDE
%token <string> WRITE
%token <char> OPEN_BRACE CLOSE_BRACE
%token <char> OPEN_PAREN CLOSE_PAREN
%token <string> ID
%token <char> OPEN_BRACKET CLOSE_BRACKET
%token <string> RETURN
%token <string> VOID
%token COMMA  // Add COMMA token

%type <ast> Program CompleteProgram PartialProgram OnlyVarDecls OnlyFuncs OnlyStmts
%type <ast> FuncDeclList FuncDecl ParamList Param ReturnStmt FuncCall ArgList VarDecl VarDeclList Stmt StmtList Expr LValue
%type <ast> GlobalDeclList // Add this line
%type <character> BinOp
%type <ast> scope_enter scope_exit



%start Program

%left PLUS MINUS
%left MULTIPLY DIVIDE

%%
Program:
    CompleteProgram
    | PartialProgram
    | OnlyVarDecls
    | OnlyFuncs
    | OnlyStmts
    | /* empty */ {
        printf("The PARSER has started with an empty program\n");
        root = createNode(NodeType_Program);
        root->program.varDeclList = NULL; // Initialize with NULL to indicate no global variables
        root->program.funcDeclList = NULL; // No functions
        root->program.stmtList = NULL; // No statements
    }
;


CompleteProgram: GlobalDeclList FuncDeclList StmtList { 
    $$ = createNode(NodeType_Program); 
    $$->program.varDeclList = $1; // Global variables
    $$->program.funcDeclList = $2; // Function declarations
    $$->program.stmtList = $3; // Statements
}
| FuncDeclList StmtList { // Handle the case where there are no global vars
    $$ = createNode(NodeType_Program);
    $$->program.varDeclList = createNode(NodeType_VarDeclList); // Initialize with an empty VarDeclList
    $$->program.funcDeclList = $1; 
    $$->program.stmtList = $2; 
};



PartialProgram:
    VarDeclList StmtList {
        printf("The PARSER has started with variable declarations and statements\n");
        root = malloc(sizeof(ASTNode));
        root->type = NodeType_Program;
        root->program.varDeclList = $1;
        root->program.funcDeclList = NULL;
        root->program.stmtList = $2;
    }
;

OnlyVarDecls:
    VarDeclList {
        printf("The PARSER has started with only variable declarations\n");
        root = malloc(sizeof(ASTNode));
        root->type = NodeType_Program;
        root->program.varDeclList = $1;
        root->program.funcDeclList = NULL;
        root->program.stmtList = NULL;
    }
;

OnlyFuncs:
    FuncDeclList {
        printf("The PARSER has started with only function declarations\n");
        root = malloc(sizeof(ASTNode));
        root->type = NodeType_Program;
        root->program.varDeclList = NULL;
        root->program.funcDeclList = $1;
        root->program.stmtList = NULL;
    }
;

OnlyStmts:
    StmtList {
        printf("The PARSER has started with only statements\n");
        root = malloc(sizeof(ASTNode));
        root->type = NodeType_Program;
        root->program.varDeclList = NULL;
        root->program.funcDeclList = NULL;
        root->program.stmtList = $1;
    }
;

GlobalDeclList:
    /* empty */ { $$ = NULL; } // No global declarations
    | VarDecl GlobalDeclList {  // Recursive case
        $$ = createNode(NodeType_VarDeclList);  // Create a new VarDeclList node
        $$->varDeclList.varDecl = $1;  // The current variable declaration
        $$->varDeclList.varDeclList = $2;  // The rest of the variable declarations
    }
;



FuncDeclList:
    /* empty */ { 
        $$ = NULL;
    }
    | FuncDecl FuncDeclList {
        $$ = createNode(NodeType_FuncDeclList);
        $$->funcDeclList.funcDecl = $1;
        $$->funcDeclList.funcDeclList = $2;
    }
;


FuncDecl:
    TYPE ID OPEN_PAREN scope_enter ParamList CLOSE_PAREN SEMICOLON scope_exit {
        printf("PARSER: Recognized function declaration: %s\n", $2);
        addSymbol(symTab, $2, $1, 1, NULL); // Add function to global scope
        $$ = createFuncDeclNode($1, $2, $5, NULL, NULL); // No body
    }
    | VOID ID OPEN_PAREN scope_enter ParamList CLOSE_PAREN SEMICOLON scope_exit {
        printf("PARSER: Recognized void function declaration: %s\n", $2);
        addSymbol(symTab, $2, "void", 1, NULL); // Add void function to global scope
        $$ = createFuncDeclNode("void", $2, $5, NULL, NULL); // No body
    }
    | TYPE ID OPEN_PAREN scope_enter ParamList CLOSE_PAREN OPEN_BRACE StmtList ReturnStmt CLOSE_BRACE scope_exit {
        printf("PARSER: Recognized function declaration: %s\n", $2);
        addSymbol(symTab, $2, $1, 1, NULL); // Add function to global scope
        $$ = createFuncDeclNode($1, $2, $5, $8, $9); // Body present
    }
    | VOID ID OPEN_PAREN scope_enter ParamList CLOSE_PAREN OPEN_BRACE StmtList CLOSE_BRACE scope_exit {
        printf("PARSER: Recognized void function declaration: %s\n", $2);
        addSymbol(symTab, $2, "void", 1, NULL); // Add void function to global scope
        $$ = createFuncDeclNode("void", $2, $5, $8, NULL); // No return statement
    }
;


scope_enter:
    { 
        enterScope(symTab); 
        printf("Entering scope level %d\n", symTab->currentScopeLevel); // Debugging output
        $$ = NULL; 
    }
;

scope_exit:
    { 
        printf("Exiting scope level %d\n", symTab->currentScopeLevel); // Debugging output
        exitScope(symTab); 
        $$ = NULL; 
    }
;

Param:
    TYPE ID {
        // Add each parameter to the symbol table at the current scope level
        addSymbol(symTab, $2, $1, 0, NULL); // isFunction set to 0 for parameters
        $$ = createParamNode($1, $2);       // Assuming createParamNode returns a Param*
    }
;

ParamList:
    /* empty */ { $$ = NULL; }
| Param {
    $$ = createParamListNode($1, NULL);
}
| Param COMMA ParamList {
    $$ = createParamListNode($1, $3);
}
;




ReturnStmt: RETURN Expr SEMICOLON {
    printf("PARSER: Recognized return statement\n");
    $$ = createReturnNode($2);
}
| /* empty */ {
    $$ = NULL;  // Void functions may not have return statements
}
;
VarDeclList:
    /* empty */ { 
        $$ = createNode(NodeType_VarDeclList); // Create an empty VarDeclList node
        $$->varDeclList.varDecl = NULL;         // Initialize with NULL values
        $$->varDeclList.varDeclList = NULL; 
    }
    | VarDecl VarDeclList {
        $$ = createNode(NodeType_VarDeclList);
        $$->varDeclList.varDecl = $1;
        $$->varDeclList.varDeclList = $2;
    }
;


VarDecl: TYPE ID SEMICOLON {
    printf("PARSER: Recognized variable declaration: %s\n", $2);
    symbol = lookupSymbol(symTab, $2, 1);
    if (symbol != NULL) {
        printf("PARSER: Variable %s at line %d has already been declared - COMPILATION HALTED\n", $2, yylineno);
        exit(0);
    } else {
        $$ = createNode(NodeType_VarDecl);  // Use your node creation function
        $$->varDecl.varType = strdup($1);  // Store type
        $$->varDecl.varName = strdup($2);  // Store variable name
        addSymbol(symTab, $2, $1, 0, NULL); // Add symbol to the symbol table
    }
}
;

| TYPE ID OPEN_BRACKET NUMBER CLOSE_BRACKET SEMICOLON {
    printf("PARSER: Recognized array declaration: %s[%d]\n", $2, $4);
    symbol = lookupSymbol(symTab, $2, 1);
    if (symbol != NULL) {
        printf("PARSER: Array %s at line %d has already been declared - COMPILATION HALTED\n", $2, yylineno);
        exit(0);
    } else {
        $$ = malloc(sizeof(ASTNode));
        $$->type = NodeType_ArrayDecl;
        $$->arrayDecl.varType = strdup($1);  // Can be "int" or "float"
        $$->arrayDecl.varName = strdup($2);
        $$->arrayDecl.size = $4;
        addSymbol(symTab, $2, $1, 0, NULL);  // Add array symbol to symbol table
    }
}
;
StmtList:
    /* empty */ {
        $$ = createNode(NodeType_StmtList); // Create an empty StmtList node
        $$->stmtList.stmt = NULL;
        $$->stmtList.stmtList = NULL;
    }
    | Stmt StmtList {
        // Create a new StmtList node and link the current statement
        $$ = createNode(NodeType_StmtList);
        $$->stmtList.stmt = $1;              // Set the current statement
        $$->stmtList.stmtList = $2;          // Link to the rest of the statements
    }
    | VarDecl StmtList {  // Allow variable declarations within StmtList
        $$ = createNode(NodeType_StmtList);
        $$->stmtList.stmt = $1;  // Treat VarDecl as a statement within StmtList
        $$->stmtList.stmtList = $2;
    }
;


Stmt: LValue EQ Expr SEMICOLON {
       printf("PARSER: Recognized assignment statement\n");
       $$ = malloc(sizeof(ASTNode));
       $$->type = NodeType_AssignStmt;
       $$->assignStmt.lvalue = $1;
       $$->assignStmt.expr = $3;
     }
   | WRITE Expr SEMICOLON { 
       printf("PARSER: Recognized write statement\n");
       $$ = malloc(sizeof(ASTNode));
       $$->type = NodeType_WriteStmt;
       $$->writeStmt.expr = $2;
     }
   | FuncCall SEMICOLON {
       printf("PARSER: Recognized function call statement\n");
       $$ = $1;
     }
   | OPEN_BRACE StmtList CLOSE_BRACE {
       printf("PARSER: Recognized a block statement\n");
       $$ = malloc(sizeof(ASTNode));
       $$->type = NodeType_BlockStmt;
       $$->blockStmt.stmtList = $2;
     }
;

LValue: ID {
    printf("PARSER: Recognized LValue ID: %s\n", $1);
    $$ = malloc(sizeof(ASTNode));
    $$->type = NodeType_SimpleID;
    $$->simpleID.name = strdup($1);
}
| ID OPEN_BRACKET Expr CLOSE_BRACKET {
    printf("PARSER: Recognized LValue array access: %s[...]\n", $1);
    $$ = malloc(sizeof(ASTNode));
    $$->type = NodeType_ArrayAccess;
    $$->arrayAccess.arrayName = strdup($1);
    $$->arrayAccess.index = $3;
}
;

Expr: Expr BinOp Expr {
    printf("PARSER: Recognized binary expression\n");
    $$ = malloc(sizeof(ASTNode));
    $$->type = NodeType_BinOp;
    $$->binOp.left = $1;
    $$->binOp.right = $3;
    $$->binOp.operator = $2;
}
| NUMBER {
    printf("PARSER: Recognized integer\n");
    $$ = createIntNode($1);
}
| FLOAT_LITERAL {
    printf("PARSER: Recognized float\n");
    $$ = createFloatNode($1);
}
| ID {
    printf("PARSER: Recognized id\n");
    $$ = malloc(sizeof(ASTNode));
    $$->type = NodeType_SimpleID;
    $$->simpleID.name = strdup($1);
}
| FuncCall {
    $$ = $1;  // Return the result of function call
}
| ID OPEN_BRACKET Expr CLOSE_BRACKET {
    printf("PARSER: Recognized array access: %s[...]\n", $1);
    $$ = malloc(sizeof(ASTNode));
    $$->type = NodeType_ArrayAccess;
    $$->arrayAccess.arrayName = strdup($1);
    $$->arrayAccess.index = $3;
}
| OPEN_PAREN Expr CLOSE_PAREN {
    printf("PARSER: Recognized parenthesized expression\n");
    $$ = $2;  // Just return the expression inside parentheses
}
;

FuncCall: ID OPEN_PAREN ArgList CLOSE_PAREN {
    printf("PARSER: Recognized function call: %s\n", $1);
    $$ = createFuncCallNode($1, $3);
}
;

ArgList: /* empty */ {
    $$ = NULL;
}
| Expr {
    $$ = createArgList($1, NULL);  // Single argument
}
| Expr COMMA ArgList {
    $$ = createArgList($1, $3);  // Multiple arguments
}
;


BinOp: PLUS   { $$ = '+'; }
     | MINUS  { $$ = '-'; }
     | MULTIPLY { $$ = '*'; }
     | DIVIDE  { $$ = '/'; }
;

%%

// Main function

int main() {
    yyin = fopen("testProg.cmm", "r");

    symTab = createSymbolTable(TABLE_SIZE);
    if (symTab == NULL) {
        return EXIT_FAILURE;
    }
    symbol = malloc(sizeof(Symbol));
    initializeTempVars();

    if (yyparse() == 0) {
        printf("Parsing successful!\n");
        traverseAST(root, 0);
        printSymbolTable(symTab);
        printf("\n=== SEMANTIC ANALYSIS ===\n\n");
        semanticAnalysis(root, symTab);
        printf("\n=== TAC GENERATION ===\n");
        printTACToFile("TAC.ir", tacHead);
        printf("\n=== CODE OPTIMIZATION ===\n");
        optimizeTAC(&tacHead);
        printOptimizedTAC("TACOptimized.ir", tacHead);
        printf("\n=== CODE GENERATION ===\n");
        initCodeGenerator("output.s");
        generateMIPS(tacHead);
        finalizeCodeGenerator("output.s");


        freeAST(root);
        freeSymbolTable(symTab);
    } else {
        fprintf(stderr, "Parsing failed\n");
    }

    fclose(yyin);
    return 0;
}

// Error handling function
void yyerror(const char* s) {
    if (strstr(s, "syntax error")) {
        fprintf(stderr, "Syntax error: %s at line %d. Did you forget a semicolon or a closing brace?\n", s, yylineno);
    } else {
        fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
    }
}
