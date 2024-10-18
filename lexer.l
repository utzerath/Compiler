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

"write" {words++; chars += yyleng;
            printf("%s : WRITE\n", yytext);
            yylval.string = strdup(yytext); 
            return WRITE;
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

";"     {chars++;
          printf("%s : SEMICOLON\n", yytext);
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