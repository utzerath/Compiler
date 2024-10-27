#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct Param {
    char* name;    // Parameter name
    char* type;    // Parameter type
    struct Param* next;  // Pointer to the next parameter (for linked list)
} Param;

typedef struct Symbol {
    char* name;           // Name of the symbol
    char* type;           // Type of the symbol or return type for functions
    int size;             // Size (used for arrays)
    int isFunction;       // 1 if it's a function, 0 otherwise
    Param* paramList;     // List of parameters if it's a function
    struct SymbolTable* localSymbolTable;  // Local symbol table for function scope
    struct Symbol* next;  // Pointer to the next symbol (for handling collisions)
} Symbol;

typedef struct SymbolTable {
    int size;
    Symbol** table;  // Hash table of symbols
} SymbolTable;

// Function declarations
SymbolTable* createSymbolTable(int size);
unsigned int hash(SymbolTable* table, char* name);
void addSymbol(SymbolTable* table, char* name, char* type, int isFunction, Param* paramList, SymbolTable* localSymbolTable);
Symbol* lookupSymbol(SymbolTable* table, char* name);
void freeSymbolTable(SymbolTable* table);
void printSymbolTable(SymbolTable* table);

// Parameter handling
Param* createParam(char* name, char* type);
void addParam(Param** paramList, char* name, char* type);
void freeParamList(Param* paramList);

#endif
