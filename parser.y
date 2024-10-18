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
    struct ASTNode* ast;
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

%type <ast> Program VarDecl VarDeclList Stmt StmtList Expr LValue
%type <character> BinOp  // Declare BinOp to return a char

%start Program

%left PLUS MINUS
%left MULTIPLY DIVIDE
%nonassoc OPEN_PAREN CLOSE_PAREN

%%

Program: VarDeclList StmtList    { 
                                    printf("The PARSER has started\n"); 
                                    root = malloc(sizeof(ASTNode));
                                    root->type = NodeType_Program;
                                    root->program.varDeclList = $1;
                                    root->program.stmtList = $2;
                                }
;

VarDeclList: /* empty */ {
    $$ = NULL;
}
| VarDecl VarDeclList {
    $$ = malloc(sizeof(ASTNode));  
    $$->type = NodeType_VarDeclList;  
    $$->varDeclList.varDecl = $1;  
    $$->varDeclList.varDeclList = $2;
}
;

VarDecl: TYPE ID SEMICOLON {
    printf("PARSER: Recognized variable declaration: %s\n", $2);
    symbol = lookupSymbol(symTab, $2);
    if (symbol != NULL) {
        printf("PARSER: Variable %s at line %d has already been declared - COMPILATION HALTED\n", $2, yylineno);
        exit(0);
    } else {
        $$ = malloc(sizeof(ASTNode));
        $$->type = NodeType_VarDecl;
        $$->varDecl.varType = strdup($1);  // Can be "int" or "float"
        $$->varDecl.varName = strdup($2);
        addSymbol(symTab, $2, $1, 0);  // Add symbol to symbol table
    }
}
| TYPE ID OPEN_BRACKET NUMBER CLOSE_BRACKET SEMICOLON {
    printf("PARSER: Recognized array declaration: %s[%d]\n", $2, $4);
    symbol = lookupSymbol(symTab, $2);
    if (symbol != NULL) {
        printf("PARSER: Array %s at line %d has already been declared - COMPILATION HALTED\n", $2, yylineno);
        exit(0);
    } else {
        $$ = malloc(sizeof(ASTNode));
        $$->type = NodeType_ArrayDecl;
        $$->arrayDecl.varType = strdup($1);  // Can be "int" or "float"
        $$->arrayDecl.varName = strdup($2);
        $$->arrayDecl.size = $4;
        addSymbol(symTab, $2, $1, $4);  // Add symbol to symbol table
    }
}
        | TYPE ID {
                  printf ("Missing semicolon after declaring variable: %s\n", $2);
             }
            

StmtList:  {/*empty*/} 
    | Stmt StmtList { printf("PARSER: Recognized statement list\n");
                        $$ = malloc(sizeof(ASTNode));
                        $$->type = NodeType_StmtList;
                        $$->stmtList.stmt = $1;
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
    $$->type = NodeType_Expr;
    $$->expr.left = $1;
    $$->expr.right = $3;
    $$->expr.operator = $2;  // Use operator directly from BinOp
}
| NUMBER {
    printf("PARSER: Recognized integer\n");
    $$ = createIntNode($1);  // Create a node for integer value
}
| FLOAT_LITERAL {
    printf("PARSER: Recognized float\n");
    $$ = createFloatNode($1);  // Create a node for float value
}
| ID {
    printf("PARSER: Recognized identifier\n");
    $$ = malloc(sizeof(ASTNode));
    $$->type = NodeType_SimpleID;
    $$->simpleID.name = strdup($1);
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

BinOp: PLUS   { $$ = '+'; }
     | MINUS  { $$ = '-'; }
     | MULTIPLY { $$ = '*'; }
     | DIVIDE  { $$ = '/'; }
;

%%

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

void yyerror(const char* s) {
    // Enhanced error message with line number and a suggestion for missing semicolons
    if (strstr(s, "syntax error")) {
        fprintf(stderr, "Syntax error: %s at line %d. Did you forget a semicolon or a closing brace?\n", s, yylineno);
    } else {
        fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
    }
}
