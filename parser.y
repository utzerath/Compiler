%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"
#include "symbolTable.h"
#include "semantic.h"
#include "codeGenerator.h"
#include "optimizer.h"
#define YYDEBUG 1
#define TABLE_SIZE 100

extern int yylex();   // Declare yylex, the lexer function
extern int yyparse(); // Declare yyparse, the parser function
extern FILE* yyin;    // Declare yyin, the file pointer for the input file
extern int yylineno;  // Declare yylineno, the line number counter
extern TAC* tacHead;  // Declare the head of the linked list of TAC entries

void yyerror(const char* s);
char* currentFunctionName = NULL;
char* tempFunctionName = NULL;  // Temporary variable for holding the function name for parameters


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
%token MAIN
%token WHILE
%token BOOL_LITERAL  // Token for 'true' and 'false'
%token TYPE_BOOL     // Represents `bool`
%token AND_OP "&&"
%token OR_OP "||"
%token NOT_OP "!"






// Tokens for conditional statements and logical operators
%token IF ELSE AND OR NOT EQUALS NOT_EQUALS LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL

%type <ast> Program
%type <ast> FuncDeclList FuncDecl ParamList Param ReturnStmt FuncCall ArgList VarDecl VarDeclList Stmt StmtList Expr LValue
%type <ast> GlobalDeclList // Add this line
%type <character> BinOp
%type <ast> scope_enter scope_exit SetFunctionName MainFunc
%type <string> ReturnType
%type <ast> IfStmt
%type <number> BOOL_LITERAL
%type <string> Type





%start Program

%left OR
%left AND
%left EQUALS NOT_EQUALS LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right NOT

%%
Program:
    GlobalDeclList StmtList MainFunc FuncDeclList {
        printf("DEBUG: Parsed complete program with global declarations, statements, main function, and other functions.\n");
        $$ = createNode(NodeType_Program);
        $$->program.varDeclList = $1;
        $$->program.stmtList = $2;
        $$->program.mainFunc = $3;
        $$->program.funcDeclList = $4;
        
        root = $$;  // Set root to point to the top-level AST node
    }
;



GlobalDeclList:
    /* empty */ { $$ = NULL; }
    | VarDecl GlobalDeclList {
        printf("DEBUG: Parsed global variable declaration: %s\n", $1->varDecl.varName);
        ASTNode* varDeclListNode = createNode(NodeType_VarDeclList);
        varDeclListNode->varDeclList.varDecl = $1;
        varDeclListNode->varDeclList.varDeclList = $2;
        $$ = varDeclListNode;
    }
;


MainFunc:
    VOID MAIN OPEN_PAREN CLOSE_PAREN { currentFunctionName = strdup("main"); } scope_enter OPEN_BRACE StmtList CLOSE_BRACE scope_exit {
        printf("PARSER: Recognized main function with body.\n");

        // Add `main` as a function with `Is Function: 1`
        addSymbol(symTab, currentFunctionName, "void", 1, NULL, NULL);

        // Create the AST node for main
        $$ = createFuncDeclNode("void", currentFunctionName, NULL, $8, NULL);

        // Clean up
        free(currentFunctionName);
        currentFunctionName = NULL;
    }
;




GlobalDeclList:
    /* empty */ { $$ = NULL; } 
    | VarDecl GlobalDeclList { 
        printf("DEBUG: Parsed global variable declaration: %s\n", $1->varDecl.varName);
        ASTNode* varDeclListNode = createNode(NodeType_VarDeclList);
        varDeclListNode->varDeclList.varDecl = $1;  
        varDeclListNode->varDeclList.varDeclList = $2;  
        $$ = varDeclListNode; // Return the new list node
    }
;

SetFunctionName:
    ID {
        // Create a new AST node for the function declaration
        $$ = createNode(NodeType_FuncDecl); // Create a node of type function declaration

        // Assuming 'name' and other necessary fields are part of your FuncDecl structure
        // Populate the AST node with the function name and other relevant data
        if ($$) {
            (*$$).funcDecl.name = strdup($1); // Set the function name
            printf("DEBUG: Set currentFunctionName to %s\n", (*$$).funcDecl.name);
        }

        // Optionally, set the current function name if needed for other contexts
        currentFunctionName = strdup($1); 
    }
;


FuncDecl:
    ReturnType SetFunctionName OPEN_PAREN scope_enter ParamList CLOSE_PAREN OPEN_BRACE StmtList ReturnStmt CLOSE_BRACE scope_exit {
        printf("PARSER: Recognized function declaration with body: %s of return type %s\n", currentFunctionName, $1);

        // Add the function itself with `Is Function: 1`
        addSymbol(symTab, currentFunctionName, $1, 1, NULL, NULL);

        // Create function declaration node in AST
        $$ = createFuncDeclNode($1, currentFunctionName, $5, $8, $9);

        // Clear function name after use
        free(currentFunctionName);
        currentFunctionName = NULL;
    }
;


// Define the ReturnType rule to handle `void`, `int`, and `float`
ReturnType:
    VOID { $$ = "void"; }
    | TYPE { $$ = $1; }  // Handles `int`, `float`, etc.
;

FuncDeclList:
    /* empty */ { $$ = NULL; }
    | FuncDecl FuncDeclList {
        printf("DEBUG: Parsed function declaration: %s\n", $1->funcDecl.name);
        ASTNode* node = createNode(NodeType_FuncDeclList);
        node->funcDeclList.funcDecl = $1;
        node->funcDeclList.funcDeclList = $2;
        $$ = node;
    }
;


scope_enter:
    { 
        enterScope(symTab); 
        printf("Entering scope level %d in function %s\n", symTab->currentScopeLevel, currentFunctionName); // Debugging output
        $$ = NULL; 
    }
;

scope_exit:
    { 
        printf("Exiting scope level %d in function %s\n", symTab->currentScopeLevel, currentFunctionName); // Debugging output
        exitScope(symTab); 
        $$ = NULL; 
    }
;


Param:
    TYPE ID {
        printf("PARSER: Recognized parameter: %s of type %s in function %s\n", $2, $1, currentFunctionName);

        // Directly add the parameter to the symbol table with correct order
        addSymbol(symTab, $2, $1, 1, NULL, currentFunctionName);  // `Is Function` set to 1 to match your setup

        // Create an AST node for the parameter
        $$ = createParamNode($1, $2);
    }
;




ParamList:
    /* empty */ {
        $$ = NULL;  // No parameters
    }
    | Param {
        $$ = createParamListNode($1, NULL);  // Single parameter
    }
    | Param COMMA ParamList {
        $$ = createParamListNode($1, $3);  // Append current parameter to the end of the list
    }
;





ReturnStmt:
    RETURN Expr SEMICOLON { 
        printf("PARSER: Recognized return statement with value\n");
        $$ = createReturnNode($2); // Create return node with expression
    }
    | RETURN SEMICOLON { 
        printf("PARSER: Invalid return statement in non-void function\n");
        yyerror("Cannot return without a value in a non-void function."); // Handle error
        $$ = NULL; // Set to NULL if error occurs
    }
    | /* empty */ { 
        $$ = NULL; // No return statement for void functions
    }
;


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


VarDecl:
    Type ID SEMICOLON {
        printf("PARSER: Recognized variable declaration: %s of type %s in function %s\n", $2, $1, currentFunctionName ? currentFunctionName : "global");
        addSymbol(symTab, $2, $1, 0, NULL, currentFunctionName); // Use currentFunctionName for local variables, NULL for global
        $$ = createNode(NodeType_VarDecl);  // AST node creation
        $$->varDecl.varType = strdup($1);
        $$->varDecl.varName = strdup($2);
    }
    | Type ID OPEN_BRACKET NUMBER CLOSE_BRACKET SEMICOLON {
        printf("PARSER: Recognized array declaration: %s[%d]\n", $2, $4);
        symbol = lookupSymbol(symTab, $2, 1);
        if (symbol != NULL) {
            printf("PARSER: Array %s at line %d has already been declared - COMPILATION HALTED\n", $2, yylineno);
            exit(0);
        } else {
            $$ = malloc(sizeof(ASTNode));
            $$->type = NodeType_ArrayDecl;
            $$->arrayDecl.varType = strdup($1);
            $$->arrayDecl.varName = strdup($2);
            $$->arrayDecl.size = $4;
            addSymbol(symTab, $2, $1, 0, NULL, currentFunctionName); // Handle array declarations appropriately
        }
    }
;

StmtList:
    /* empty */ { 
        $$ = NULL;  // If there are no statements, make the statement list NULL instead of creating an empty node
    }
    | Stmt StmtList {
        $$ = createNode(NodeType_StmtList); // Create a new StmtList node
        $$->stmtList.stmt = $1;              // Set the current statement
        $$->stmtList.stmtList = $2;          // Link to the rest of the statements
    }
    | VarDecl StmtList { 
        $$ = createNode(NodeType_StmtList); // Create a new StmtList node for variable declarations
        $$->stmtList.stmt = $1;              // Add VarDecl to the statement list
        $$->stmtList.stmtList = $2;          // Link to the rest of the statements
    }
;



Stmt:
    LValue EQ Expr SEMICOLON {
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
    | IfStmt { 
        $$ = $1;  // Link `IfStmt` rule to `Stmt` for conditional handling
    }
    | WHILE OPEN_PAREN Expr CLOSE_PAREN Stmt {  // Adding the while loop handling
        printf("PARSER: Recognized while loop\n");
        $$ = createWhileNode($3, $5); // $3 is the condition, $5 is the body of the loop
    }
;

Type:
    TYPE         { $$ = $1; }         // For standard types like `int`, `float`
  | TYPE_BOOL    { $$ = "bool"; }     // For `bool` type
;

IfStmt: 
      IF OPEN_PAREN Expr CLOSE_PAREN Stmt {
          printf("PARSER: Recognized if statement\n");
          $$ = createIfNode($3, $5, NULL);  // AST node with condition, true branch, and NULL for else
      }
    | IF OPEN_PAREN Expr CLOSE_PAREN Stmt ELSE Stmt {
          printf("PARSER: Recognized if-else statement\n");
          $$ = createIfNode($3, $5, $7);  // AST node with condition, true branch, and else branch
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

Expr:
      FuncCall { 
          $$ = $1;  // Treat the function call as an expression
      }
    | Expr BinOp Expr {
          printf("PARSER: Recognized binary expression\n");
          $$ = malloc(sizeof(ASTNode));
          $$->type = NodeType_BinOp;
          $$->binOp.left = $1;
          $$->binOp.right = $3;
          $$->binOp.operator = $2;
      }
    | Expr AND Expr {
          printf("PARSER: Recognized logical AND\n");
          $$ = createLogicalNode("&&", $1, $3); // Create logical AND AST node
      }
    | Expr OR Expr {
          printf("PARSER: Recognized logical OR\n");
          $$ = createLogicalNode("||", $1, $3); // Create logical OR AST node
      }
    | NOT Expr {
          printf("PARSER: Recognized logical NOT\n");
          $$ = createLogicalNode("!", $2, NULL); // Create logical NOT AST node
      }
    | Expr EQUALS Expr {
          printf("PARSER: Recognized equality check\n");
          $$ = createComparisonNode("==", $1, $3);
      }
    | Expr NOT_EQUALS Expr {
          printf("PARSER: Recognized inequality check\n");
          $$ = createComparisonNode("!=", $1, $3);
      }
    | Expr LESS_THAN Expr {
          printf("PARSER: Recognized less-than comparison\n");
          $$ = createComparisonNode("<", $1, $3);
      }
    | Expr GREATER_THAN Expr {
          printf("PARSER: Recognized greater-than comparison\n");
          $$ = createComparisonNode(">", $1, $3);
      }
    | Expr LESS_EQUAL Expr {
          printf("PARSER: Recognized less-than-or-equal comparison\n");
          $$ = createComparisonNode("<=", $1, $3);
      }
    | Expr GREATER_EQUAL Expr {
          printf("PARSER: Recognized greater-than-or-equal comparison\n");
          $$ = createComparisonNode(">=", $1, $3);
      }
    | BOOL_LITERAL {
          printf("PARSER: Recognized boolean literal: %s\n", $1 ? "true" : "false");
          $$ = createBoolNode($1); // Add boolean literal node (true or false)
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
          $$ = createIDNode($1);
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
          $$ = $2;
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

        if (root == NULL) {
        fprintf(stderr, "Error: AST root node is NULL after parsing.\n");
        return EXIT_FAILURE;
        }
        traverseAST(root, 0);
        printSymbolTable(symTab);
        printf("\n ===SEMANTIC ANALYSIS===\n");
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
