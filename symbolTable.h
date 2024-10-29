#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

// Symbol structure for variables and functions
typedef struct Symbol {
    char* name;                  // Name of the symbol
    char* type;                  // Type of the symbol or return type for functions
    int size;                    // Size (used for arrays)
    int scopeLevel;              // Scope level (0 = global, >0 = local)
    int isFunction;              // Flag to indicate if this symbol is a function
    char* belongingFunction;
    struct SymbolTable* localSymbolTable;  // Local symbol table for function scope
    struct Symbol* next;         // Pointer to the next symbol (for handling collisions in hash table)
} Symbol;

// Symbol table structure to hold symbols
typedef struct SymbolTable {
    int size;                    // Size of the hash table
    Symbol** table;              // Hash table of symbols
    int currentScopeLevel;       // 0 for global scope, >0 for local scopes
} SymbolTable;

// Function declarations
SymbolTable* createSymbolTable(int size);
unsigned int hash(SymbolTable* table, char* name);
void addSymbol(SymbolTable* table, char* name, char* type, int isFunction, SymbolTable* localSymbolTable, char* belongingFunction);
Symbol* lookupSymbol(SymbolTable* table, char* name, int currentScopeOnly);
void freeSymbolTable(SymbolTable* table);
void printSymbolTable(SymbolTable* table);
void enterScope(SymbolTable* table);
void exitScope(SymbolTable* table);

// Global symbol table access
SymbolTable* getGlobalSymbolTable();

#endif
