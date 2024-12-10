%option noyywrap
%option yylineno

%{
#include <stdio.h>
#include <string.h>

#define YY_DECL int yylex()
#include "parser.tab.h"

int words = 0;
int chars = 0;
int lines = 0;

%}

letter      [a-zA-Z]
digit       [0-9]
ID          {letter}({letter}|{digit})*
delim       [ \t\r]
INT         {digit}+
FLOAT       ({digit}+"."{digit}*([eE][+-]?{digit}+)?)
ws          {delim}+

%%

"/*"    {
            int c;
            while((c = input()) != 0) {
                if(c == '*') {
                    if((c = input()) == '/')
                        break;
                    else
                        unput(c);
                }
            }
        }
"if"    {words++; chars += yyleng;
            printf("%s : IF\n", yytext);
            yylval.string = strdup(yytext); 
            return IF;
        }

"else"  {words++; chars += yyleng;
            printf("%s : ELSE\n", yytext);
            yylval.string = strdup(yytext); 
            return ELSE;
        }

"&&"    {chars += yyleng;
            printf("%s : AND\n", yytext);
            yylval.operator = strdup(yytext); 
            return AND;
        }

"||"    {chars += yyleng;
            printf("%s : OR\n", yytext);
            yylval.operator = strdup(yytext); 
            return OR;
        }
"bool"  {words++; chars += yyleng;
            printf("%s : BOOL\n", yytext);
            yylval.string = strdup(yytext); 
            return TYPE_BOOL;
        }

"true"  {chars += yyleng;
            printf("%s : TRUE\n", yytext);
            yylval.number = 1;  // Using 1 to represent true
            return BOOL_LITERAL;
        }

"false" {chars += yyleng;
            printf("%s : FALSE\n", yytext);
            yylval.number = 0;  // Using 0 to represent false
            return BOOL_LITERAL;
        }
"!"     {chars++;
            printf("%s : NOT\n", yytext);
            yylval.operator = strdup(yytext); 
            return NOT;
        }
"=="    {chars += yyleng;
            printf("%s : EQUALS\n", yytext);
            yylval.operator = strdup(yytext); 
            return EQUALS;
        }

"!="    {chars += yyleng;
            printf("%s : NOT_EQUALS\n", yytext);
            yylval.operator = strdup(yytext); 
            return NOT_EQUALS;
        }

"<"     {chars++;
            printf("%s : LESS_THAN\n", yytext);
            yylval.operator = strdup(yytext); 
            return LESS_THAN;
        }

">"     {chars++;
            printf("%s : GREATER_THAN\n", yytext);
            yylval.operator = strdup(yytext); 
            return GREATER_THAN;
        }

"<="    {chars += yyleng;
            printf("%s : LESS_EQUAL\n", yytext);
            yylval.operator = strdup(yytext); 
            return LESS_EQUAL;
        }

">="    {chars += yyleng;
            printf("%s : GREATER_EQUAL\n", yytext);
            yylval.operator = strdup(yytext); 
            return GREATER_EQUAL;
        }
        
"int"   {words++; chars += yyleng;
            printf("%s : TYPE\n", yytext);
            yylval.string = strdup(yytext); 
            return TYPE;
        }

"float" {words++; chars += yyleng;
            printf("%s : TYPE\n", yytext);
            yylval.string = strdup(yytext); 
            return TYPE;
        }

"void" {words++; chars += yyleng;
            printf("%s : VOID\n", yytext);
            yylval.string = strdup(yytext); 
            return VOID;
        }

"return" {words++; chars += yyleng;
            printf("%s : RETURN\n", yytext);
            yylval.string = strdup(yytext); 
            return RETURN;
        }

"while" {
    words++; chars += yyleng;
    printf("%s : WHILE\n", yytext);
    yylval.string = strdup(yytext); 
    return WHILE;
}


"write" {words++; chars += yyleng;
            printf("%s : WRITE\n", yytext);
            yylval.string = strdup(yytext); 
            return WRITE;
        }
"main" { 
    printf("%s : MAIN\n", yytext);
    yylval.string = strdup(yytext); 
    return MAIN;
}


{ID}        {words++; chars += yyleng;
              printf("%s : IDENTIFIER\n", yytext);
              yylval.string = strdup(yytext); // Convert to string
              return ID;
            }

{INT}       {words++; chars += yyleng;
              printf("%s : NUMBER\n", yytext);
              yylval.number = atoi(yytext); // Convert to int
              return NUMBER;
            }

{FLOAT}     {words++; chars += yyleng;
              printf("%s : FLOAT_LITERAL\n", yytext);
              yylval.float_number = atof(yytext); // Convert to float
              return FLOAT_LITERAL;
            }

","     {chars++;
          printf("%s : COMMA\n", yytext);
          yylval.character = yytext[0];
          return COMMA;
        }

";"     {chars++;
          printf("%s : SEMICOLON\n", yytext);
          printf("Lexer: Found SEMICOLON -> %s : SEMICOLON at line %d\n", yytext, yylineno);
          yylval.character = yytext[0]; 
          return SEMICOLON;
        }

"="     {chars++;
          printf("%s : EQ\n", yytext);
          yylval.operator = strdup(yytext); 
          return EQ;
        }

"+"     {chars++;
          printf("%s : PLUS\n", yytext);
          yylval.operator = strdup(yytext); 
          return PLUS;
        }

"-"     {chars++;
          printf("%s : MINUS\n", yytext);
          yylval.operator = strdup(yytext); 
          return MINUS;
        }

"*"     {chars++;
          printf("%s : MULTIPLY\n", yytext);
          yylval.operator = strdup(yytext); 
          return MULTIPLY;
        }

"/"     {chars++;
          printf("%s : DIVIDE\n", yytext);
          yylval.operator = strdup(yytext); 
          return DIVIDE;
        }

"("     {chars++;
          printf("%s : OPEN_PAREN\n", yytext);
          yylval.character = yytext[0]; 
          return OPEN_PAREN;
        }

")"     {chars++;
          printf("%s : CLOSE_PAREN\n", yytext);
          yylval.character = yytext[0]; 
          return CLOSE_PAREN;
        }

"{"     {chars++;
          printf("%s : OPEN_BRACE\n", yytext);
          yylval.character = yytext[0]; 
          return OPEN_BRACE;
        }

"}"     {chars++;
          printf("%s : CLOSE_BRACE\n", yytext);
          yylval.character = yytext[0]; 
          return CLOSE_BRACE;
        }

"["     {chars++;
          printf("%s : OPEN_BRACKET\n", yytext);
          yylval.character = yytext[0]; 
          return OPEN_BRACKET;
        }

"]"     {chars++;
          printf("%s : CLOSE_BRACKET\n", yytext);
          yylval.character = yytext[0]; 
          return CLOSE_BRACKET;
        }
        

{ws}        {chars += yyleng;}
\n        {lines++; chars = 0;}
.           {chars++; printf("Unrecognized symbol '%s' (ASCII %d) at line %d char %d\n", yytext, yytext[0], yylineno, chars);}

%%
