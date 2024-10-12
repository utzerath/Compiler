%option noyywrap

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
delim       [ \t\n]
INT         {digit}+
FLOAT       ({digit}+"."{digit}*([eE][+-]?{digit}+)?)
ws          {delim}+

%option yylineno
%%
"/*"    				{
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

"int"	{words++; chars += strlen(yytext);
			printf("%s : TYPE\n", yytext);
			yylval.string = strdup(yytext); 
			return TYPE;
		}

"float"	{words++; chars += strlen(yytext);
			printf("%s : TYPE\n", yytext);
			yylval.string = strdup(yytext); 
			return TYPE;
		}

"write"	{words++; chars += strlen(yytext);
			printf("%s : KEYWORD\n", yytext);
			yylval.string = strdup(yytext); 
			return WRITE;
		}

{ID}	{words++; chars += strlen(yytext);
			  printf("%s : IDENTIFIER\n",yytext);
			  yylval.string = strdup(yytext); 
			  return ID;
			}

{FLOAT}		{words++; chars += strlen(yytext);
			  printf("%s : FLOAT_LITERAL\n",yytext);
			  yylval.float_number = atof(yytext); // Convert to float
			  return FLOAT_LITERAL;
			}
			
{INT}		{words++; chars += strlen(yytext);
			  printf("%s : NUMBER\n",yytext);
			  yylval.number = atoi(yytext); // Convert to int
			  return NUMBER;
			}

";"		{chars++;
		  printf("%s : SEMICOLON\n", yytext);
		  yylval.string = strdup(yytext); 
		  return SEMICOLON;
		}
		
"="		{chars++;
		  printf("%s : EQ\n", yytext);
		  yylval.operator = strdup(yytext); 
		  return EQ;
		}

"+"		{chars++;
		  printf("%s : PLUS\n", yytext);
		  yylval.operator = strdup(yytext); 
		  return PLUS;
		}


"*"		{chars++;
		  printf("%s : MULTIPLY\n", yytext);
		  yylval.operator = strdup(yytext); 
		  return MULTIPLY;
		}

"/"		{chars++;
		  printf("%s : DIVIDE\n", yytext);
		  yylval.operator = strdup(yytext); 
		  return DIVIDE;
		}
		
"("     {chars++;
         printf("%s : OPEN_PAREN\n", yytext);
         yylval.string = strdup(yytext);
         return OPEN_PAREN;
        }

")"     {chars++;
         printf("%s : CLOSE_PAREN\n", yytext);
         yylval.string = strdup(yytext);
         return CLOSE_PAREN;
        }

"{"		{chars++;
		  printf("%s : OPEN_BRACE\n", yytext);
		  yylval.string = strdup(yytext); 
		  return OPEN_BRACE;
		}

"}"		{chars++;
		  printf("%s : CLOSE_BRACE\n", yytext);
		  yylval.string = strdup(yytext); 
		  return CLOSE_BRACE;
		}
		
[\n]	{lines++; chars=0;yylineno++;}
[ \t]	{chars++;}
.		{chars++;
         printf("%s : Unrecognized symbol at line %d char %d\n", yytext,lines,chars);
		}

%%
