#include "symbolTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Function to create a new symbol table
SymbolTable* createSymbolTable(int size) {
    SymbolTable* newTable = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (!newTable) return NULL;

    newTable->size = size;
    newTable->table = (Symbol**)malloc(sizeof(Symbol*) * size);
    if (!newTable->table) {
        free(newTable);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        newTable->table[i] = NULL;
    }

    return newTable;
}

// Hash function to map a name to an index
unsigned int hash(SymbolTable* table, char* name) {
    unsigned int hashval = 0;
    for (; *name != '\0'; name++) hashval = *name + (hashval << 5) - hashval;
    return hashval % table->size;
}

// Function to add a symbol to the table (for variables or functions)
void addSymbol(SymbolTable* table, char* name, char* type, int isFunction, Param* paramList, SymbolTable* localSymbolTable) {
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    if (!newSymbol) return;

    newSymbol->name = strdup(name);
    newSymbol->type = strdup(type);
    newSymbol->isFunction = isFunction;
    newSymbol->paramList = paramList;
    newSymbol->localSymbolTable = localSymbolTable;
    newSymbol->next = NULL;

    unsigned int hashval = hash(table, name);
    newSymbol->next = table->table[hashval];  
    table->table[hashval] = newSymbol;
}

// Function to look up a name in the table
Symbol* lookupSymbol(SymbolTable* table, char* name) {
    unsigned int hashval = hash(table, name);
    for (Symbol* sym = table->table[hashval]; sym != NULL; sym = sym->next) {
        if (strcmp(name, sym->name) == 0) return sym;
    }
    return NULL;
}

// Function to free the symbol table
void freeSymbolTable(SymbolTable* table) {
    for (int i = 0; i < table->size; i++) {
        Symbol* sym = table->table[i];
        while (sym != NULL) {
            Symbol* nextSym = sym->next;
            free(sym->name);
            free(sym->type);
            freeParamList(sym->paramList);  // Free parameters if function
            freeSymbolTable(sym->localSymbolTable);  // Free local table if function
            free(sym);
            sym = nextSym;
        }
    }
    free(table->table);
    free(table);
}

// Function to print the symbol table
void printSymbolTable(SymbolTable* table) {
    printf("----- SYMBOL TABLE -----\n");
    for (int i = 0; i < table->size; i++) {
        Symbol* sym = table->table[i];
        while (sym != NULL) {
            if (sym->isFunction) {
                printf("Function %s with return type %s\n", sym->name, sym->type);
                printf("Parameters:\n");
                Param* param = sym->paramList;
                while (param != NULL) {
                    printf("\t%s %s\n", param->type, param->name);
                    param = param->next;
                }
                printf("Local Variables:\n");
                printSymbolTable(sym->localSymbolTable);  // Recursively print the local symbol table
            } else {
                printf("Variable %s of type %s\n", sym->name, sym->type);
            }
            sym = sym->next;
        }
    }
    printf("------------------------\n");
}


void addParam(Param** paramList, char* name, char* type) {
    Param* newParam = createParam(name, type);
    newParam->next = *paramList;
    *paramList = newParam;
}

void freeParamList(Param* paramList) {
    while (paramList != NULL) {
        Param* next = paramList->next;
        free(paramList->name);
        free(paramList->type);
        free(paramList);
        paramList = next;
    }
}
Param* createParam(char* type, char* name) {
    Param* param = (Param*)malloc(sizeof(Param));
    if (!param) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    param->type = strdup(type);
    param->name = strdup(name);
    param->next = NULL;
    return param;
}


