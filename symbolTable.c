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
    newTable->currentScopeLevel = 0; // Ensure currentScopeLevel is initialized to 0
    
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

void addSymbol(SymbolTable* table, char* name, char* type, int isFunction, SymbolTable* localSymbolTable) {
    // Avoid duplicates in the same scope
    if (lookupSymbol(table, name, 1) != NULL) {
        fprintf(stderr, "Semantic error: Variable %s is already declared in the current scope\n", name);
        return;
    }

    // Create and add the new symbol with the correct scope level
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    newSymbol->name = strdup(name);
    newSymbol->type = strdup(type);
    newSymbol->scopeLevel = table->currentScopeLevel;  // Assign the current scope level
    newSymbol->isFunction = isFunction;
    newSymbol->localSymbolTable = localSymbolTable;
    newSymbol->next = table->table[hash(table, name)];
    table->table[hash(table, name)] = newSymbol;
}


// Enhanced lookupSymbol to check only the current scope or global scope if needed
Symbol* lookupSymbol(SymbolTable* table, char* name, int currentScopeOnly) {
    unsigned int hashval = hash(table, name);
    for (Symbol* sym = table->table[hashval]; sym != NULL; sym = sym->next) {
        if (strcmp(name, sym->name) == 0) {
            if (currentScopeOnly && sym->scopeLevel != table->currentScopeLevel) {
                continue;  // Skip symbols not in the current scope
            }
            return sym;
        }
    }

    // If not found in the current scope and currentScopeOnly is set, return NULL
    if (currentScopeOnly) return NULL;

    // Check global scope if symbol not found in the local scope
    SymbolTable* globalTable = getGlobalSymbolTable();
    if (globalTable != table) {
        hashval = hash(globalTable, name);
        for (Symbol* sym = globalTable->table[hashval]; sym != NULL; sym = sym->next) {
            if (strcmp(name, sym->name) == 0 && sym->scopeLevel == 0) {
                return sym;  // Found in the global scope
            }
        }
    }

    return NULL;  // Symbol not found
}

// Function to free the symbol table
void freeSymbolTable(SymbolTable* table) {
    for (int i = 0; i < table->size; i++) {
        Symbol* sym = table->table[i];
        while (sym != NULL) {
            Symbol* nextSym = sym->next;
            free(sym->name);
            free(sym->type);
            if (sym->isFunction && sym->localSymbolTable) {
                freeSymbolTable(sym->localSymbolTable);  // Free local table for functions
            }
            free(sym);
            sym = nextSym;
        }
    }
    free(table->table);
    free(table);
}

void printSymbolTable(SymbolTable* table) {
    printf("----- SYMBOL TABLE -----\n");
    for (int i = 0; i < table->size; i++) {
        Symbol* sym = table->table[i];
        while (sym != NULL) {
            if (sym->isFunction) {
                printf("Function %s with return type %s at scope level %d\n", sym->name, sym->type, sym->scopeLevel);
                if (sym->localSymbolTable) {
                    printf("Parameters and Local Variables:\n");
                    printSymbolTable(sym->localSymbolTable);  // Print local symbols recursively for functions
                }
            } else {
                printf("Variable %s of type %s at scope level %d\n", sym->name, sym->type, sym->scopeLevel);
            }
            sym = sym->next;
        }
    }
    printf("------------------------\n");
}


// Assuming you have a global symbol table instance elsewhere
SymbolTable* globalSymbolTable = NULL;

// Function to get the global symbol table (singleton)
SymbolTable* getGlobalSymbolTable() {
    if (!globalSymbolTable) {
        globalSymbolTable = createSymbolTable(100);  // Initialize with an arbitrary size
    }
    return globalSymbolTable;
}
void enterScope(SymbolTable* table) {
    table->currentScopeLevel++;
    printf("Entering scope level %d\n", table->currentScopeLevel);  // Debug output
}

void exitScope(SymbolTable* table) {
    if (table->currentScopeLevel > 0) {
        table->currentScopeLevel--;
        printf("Exiting to scope level %d\n", table->currentScopeLevel);  // Debug output
    }
}
