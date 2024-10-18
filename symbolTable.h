#ifndef SYMBOL_TABLE1_H
#define SYMBOL_TABLE1_H

#include <stdlib.h>
#include <string.h>

// Symbol structure

// Define the Symbol struct
typedef struct Symbol {
    char* name;
    char* type;
    int size;  // Add this field for array size
    struct Symbol* next;
} Symbol;

// Define the SymbolTable struct
typedef struct SymbolTable {
    int size;
    Symbol** table;
} SymbolTable;

// Function declarations
SymbolTable* createSymbolTable(int size);
void addSymbol(SymbolTable* table, char* name, char* type, int size);
Symbol* lookupSymbol(SymbolTable* table, char* name);
void freeSymbolTable(SymbolTable* table);
void printSymbolTable(SymbolTable* table);  // For debugging

#endif // SYMBOL_TABL1_H