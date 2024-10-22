

#include "optimizer.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Function to perform dead code elimination
bool deadCodeElimination(TAC** head) {
   bool changed = false;
   TAC* current = *head;
   TAC* prev = NULL;


   while (current != NULL) {
       // Only consider assignments (op == "=") with a valid result
       if (current->op != NULL && strcmp(current->op, "=") == 0 && current->result != NULL) {
           // Check if the variable is used later in the code
           if (!isUsedLater(current->next, current->result)) {
               // Variable is not used; remove this TAC node
               printf("Dead Code Elimination: Removing dead assignment to '%s'\n", current->result);
               changed = true;
               if (prev == NULL) {
                   // Removing the head node
                   *head = current->next;
               } else {
                   prev->next = current->next;
               }
               // Free the current node
               free(current->result);
               free(current->arg1);
               free(current->op);
               free(current->arg2);
               TAC* temp = current;
               current = current->next;
               free(temp);
               continue; // Skip advancing prev
           }
       }
       prev = current;
       current = current->next;
   }


   return changed;
}


// Updated helper function
bool isUsedLater(TAC* head, const char* varName) {
   while (head != NULL) {
       // Check if variable is assigned to again (redefined)
       if (head->op != NULL && strcmp(head->op, "=") == 0 &&
           head->result != NULL && strcmp(head->result, varName) == 0) {
           // Variable is redefined before being used; previous value is dead
           return false;
       }


       // Check if variable is used
       if ((head->arg1 != NULL && strcmp(head->arg1, varName) == 0) ||
           (head->arg2 != NULL && strcmp(head->arg2, varName) == 0) ||
           (head->op != NULL && strcmp(head->op, "write") == 0 &&
            head->arg1 != NULL && strcmp(head->arg1, varName) == 0)) {
           // Variable is used before being redefined
           return true;
       }


       head = head->next;
   }
   return false;
}




void safeStrReplace(char** target, const char* source) {
   if (source != NULL) {
       char* newStr = strdup(source);
       if (newStr == NULL) {
           perror("Failed to allocate memory for string duplication");
           exit(EXIT_FAILURE);
       }
       *target = newStr;  // Assign the new string
   } else {
       *target = NULL;  // Set target to NULL if source is NULL
   }
}
bool constantFolding(TAC** head) {
   TAC* current = *head;
   bool changed = false;


   while (current != NULL) {
       printf("Folding TAC: result='%s', arg1='%s', op='%s', arg2='%s'\n",
              current->result ? current->result : "NULL",
              current->arg1 ? current->arg1 : "NULL",
              current->op ? current->op : "NULL",
              current->arg2 ? current->arg2 : "NULL");


       // Handle integer operations
       if (isConstant(current->arg1) && isConstant(current->arg2)) {
           if (strcmp(current->op, "+") == 0) {
               int result = atoi(current->arg1) + atoi(current->arg2);
               foldOperation(current, result);
               changed = true;
           }
           else if (strcmp(current->op, "-") == 0) {
               int result = atoi(current->arg1) - atoi(current->arg2);
               foldOperation(current, result);
               changed = true;
           }
           else if (strcmp(current->op, "*") == 0) {
               int result = atoi(current->arg1) * atoi(current->arg2);
               foldOperation(current, result);
               changed = true;
           }
           else if (strcmp(current->op, "/") == 0) {
               int right = atoi(current->arg2);
               if (right != 0) {
                   int result = atoi(current->arg1) / right;
                   foldOperation(current, result);
                   changed = true;
               } else {
                   fprintf(stderr, "Error: Division by zero encountered in constant folding.\n");
               }
           }
       }
       // Handle floating-point operations
       else if (isFloatConstant(current->arg1) && isFloatConstant(current->arg2)) {
           float left = atof(current->arg1);
           float right = atof(current->arg2);
           float result = 0;


           if (strcmp(current->op, "add.s") == 0) {
               result = left + right;
           }
           else if (strcmp(current->op, "sub.s") == 0) {
               result = left - right;
           }
           else if (strcmp(current->op, "mul.s") == 0) {
               result = left * right;
           }
           else if (strcmp(current->op, "div.s") == 0 && right != 0.0f) {
               result = left / right;
           } else {
               fprintf(stderr, "Error: Division by zero encountered in constant folding (floats).\n");
           }


           foldFloatOperation(current, result);
           changed = true;
       }


       // Identity operation folding for integers
       if (isConstant(current->arg2)) {
           if (strcmp(current->op, "*") == 0 && strcmp(current->arg2, "1") == 0) {
               safeStrReplace(&current->op, "=");
               safeStrReplace(&current->arg2, NULL);
               changed = true;
           }
           else if (strcmp(current->op, "+") == 0 && strcmp(current->arg2, "0") == 0) {
               safeStrReplace(&current->op, "=");
               safeStrReplace(&current->arg2, NULL);
               changed = true;
           }
           else if (strcmp(current->op, "-") == 0 && strcmp(current->arg2, "0") == 0) {
               safeStrReplace(&current->op, "=");
               safeStrReplace(&current->arg2, NULL);
               changed = true;
           }
           else if (strcmp(current->op, "/") == 0 && strcmp(current->arg2, "1") == 0) {
               safeStrReplace(&current->op, "=");
               safeStrReplace(&current->arg2, NULL);
               changed = true;
           }
       }


       // Identity operation folding for floating-point numbers
       if (isFloatConstant(current->arg2)) {
           if (strcmp(current->op, "mul.s") == 0 && atof(current->arg2) == 1.0f) {
               safeStrReplace(&current->op, "=");
               safeStrReplace(&current->arg2, NULL);
               changed = true;
           }
           else if (strcmp(current->op, "add.s") == 0 && atof(current->arg2) == 0.0f) {
               safeStrReplace(&current->op, "=");
               safeStrReplace(&current->arg2, NULL);
               changed = true;
           }
           else if (strcmp(current->op, "sub.s") == 0 && atof(current->arg2) == 0.0f) {
               safeStrReplace(&current->op, "=");
               safeStrReplace(&current->arg2, NULL);
               changed = true;
           }
           else if (strcmp(current->op, "div.s") == 0 && atof(current->arg2) == 1.0f) {
               safeStrReplace(&current->op, "=");
               safeStrReplace(&current->arg2, NULL);
               changed = true;
           }
       }


       current = current->next;
   }


   return changed;
}






void foldOperation(TAC* current, int result) {
   char resultStr[20];
   sprintf(resultStr, "%d", result);
   safeStrReplace(&current->arg1, resultStr);  // Replace with result string
   safeStrReplace(&current->op, "=");          // Properly replace 'op'
   safeStrReplace(&current->arg2, NULL);       // Clear arg2
}


void foldFloatOperation(TAC* current, float result) {
   char resultStr[30];
   snprintf(resultStr, sizeof(resultStr), "%.6f", result);  // Control precision to 6 decimal places
   printf("Folding float: %s %s %s -> %s\n", current->arg1, current->op, current->arg2, resultStr);
   safeStrReplace(&current->arg1, resultStr);  // Replace arg1 with result string
   safeStrReplace(&current->op, "=");          // Replace operation with assignment
   safeStrReplace(&current->arg2, NULL);       // Clear arg2
}


// Utility function to check if a string is a valid float constant
bool isFloatConstant(const char* str) {
   if (str == NULL || *str == '\0') {
       return false;
   }


   char* endptr;
   strtod(str, &endptr);
   return (*endptr == '\0');  // Check if entire string is a valid float
}






bool constantPropagation(TAC** head) {
   TAC* current = *head;
   bool changed = false;


   // Define a structure for variable mappings
   typedef struct VarValue {
       char* varName;
       char* value;
       struct VarValue* next;
   } VarValue;


   VarValue* varTable = NULL;


   while (current != NULL) {
       // Skip 'write' operations entirely
       if (current->op != NULL && strcmp(current->op, "write") == 0) {
           current = current->next;
           continue;
       }
      
       // Handle array accesses
       if (current->op != NULL && strcmp(current->op, "[]") == 0) {
           // Propagate constants for array indices
           if (isConstant(current->arg2)) {
               safeStrReplace(&current->arg2, current->arg2);
               changed = true;
           }
       }


       // Debug: Print TAC info before processing
       printf("Processing TAC: result='%s', arg1='%s', op='%s', arg2='%s'\n",
              current->result ? current->result : "NULL",
              current->arg1 ? current->arg1 : "NULL",
              current->op ? current->op : "NULL",
              current->arg2 ? current->arg2 : "NULL");


       // If the operation is '=', handle assignments
       if (current->op != NULL && strcmp(current->op, "=") == 0) {
           // Attempt to replace arg1 if it's a variable with a known constant value
           if (current->arg1 != NULL && !isConstant(current->arg1)) {
               VarValue* var = varTable;
               while (var != NULL) {
                   if (strcmp(current->arg1, var->varName) == 0) {
                       printf("Propagating constant: Replacing '%s' with '%s' in arg1\n", current->arg1, var->value);
                       safeStrReplace(&current->arg1, var->value);
                       changed = true;
                       break;
                   }
                   var = var->next;
               }
           }
           else{
              
           }


           // Now, check if arg1 is a constant after substitution
           if (isConstant(current->arg1)) {
               // Handle assignment to a constant
               if (current->result != NULL && strlen(current->result) > 0) {
                   // Update or add the mapping
                   VarValue* var = varTable;
                   bool found = false;
                   while (var != NULL) {
                       if (strcmp(var->varName, current->result) == 0) {
                           // Update existing entry
                           free(var->value);
                           var->value = strdup(current->arg1);
                           if (var->value == NULL) {
                               perror("Failed to duplicate string in varTable");
                               exit(EXIT_FAILURE);
                           }
                           found = true;
                           printf("Updated mapping: %s = %s\n", var->varName, var->value);
                           break;
                       }
                       var = var->next;
                   }
                   if (!found) {
                       // Add new entry to varTable
                       VarValue* newVar = malloc(sizeof(VarValue));
                       if (newVar == NULL) {
                           perror("Failed to allocate memory for VarValue");
                           exit(EXIT_FAILURE);
                       }
                       newVar->varName = strdup(current->result);
                       newVar->value = strdup(current->arg1);
                       if (newVar->varName == NULL || newVar->value == NULL) {
                           perror("Failed to duplicate string for new VarValue");
                           exit(EXIT_FAILURE);
                       }
                       newVar->next = varTable;
                       varTable = newVar;
                       printf("Added mapping: %s = %s\n", newVar->varName, newVar->value);
                   }
               } else {
                   // Invalid result, skip mapping
                   printf("Skipping mapping for instruction with invalid result.\n");
               }
           } else {
               // Handle assignment to a non-constant (invalidate any existing mapping)
               if (current->result != NULL && strlen(current->result) > 0) {
                   // Remove existing mapping if any
                   VarValue** indirect = &varTable;
                   while (*indirect) {
                       if (strcmp((*indirect)->varName, current->result) == 0) {
                           VarValue* temp = *indirect;
                           *indirect = (*indirect)->next;
                           free(temp->varName);
                           free(temp->value);
                           free(temp);
                           printf("Removed mapping for variable: %s (assigned non-constant)\n", current->result);
                           break;
                       }
                       indirect = &(*indirect)->next;
                   }
               }
           }
       } else {
           // For operations other than '=', attempt to replace arg1 and arg2 with constants if possible


           // Replace arg1 if it's a variable with a known constant value
           if (current->arg1 != NULL && !isConstant(current->arg1)) {
               VarValue* var = varTable;
               while (var != NULL) {
                   if (strcmp(current->arg1, var->varName) == 0) {
                       printf("Propagating constant: Replacing '%s' with '%s' in arg1\n", current->arg1, var->value);
                       safeStrReplace(&current->arg1, var->value);
                       changed = true;
                       break;
                   }
                   var = var->next;
               }
           }


           // Replace arg2 if it's a variable with a known constant value
           if (current->arg2 != NULL && !isConstant(current->arg2)) {
               VarValue* var = varTable;
               while (var != NULL) {
                   if (strcmp(current->arg2, var->varName) == 0) {
                       printf("Propagating constant: Replacing '%s' with '%s' in arg2\n", current->arg2, var->value);
                       safeStrReplace(&current->arg2, var->value);
                       changed = true;
                       break;
                   }
                   var = var->next;
               }
           }
       }


       current = current->next;
   }


   // Free the varTable
   VarValue* var = varTable;
   while (var != NULL) {
       VarValue* temp = var;
       var = var->next;
       free(temp->varName);
       free(temp->value);
       free(temp);
   }


   return changed;
}




void optimizeTAC(TAC** head) {
   bool changed;
   int passCount = 0;


   printf("=== CODE OPTIMIZATION ===\n");


  do {
   changed = false;  // Reset flag before each pass
   printf("=== Optimization Pass %d ===\n", passCount++);


   // Apply constant propagation multiple times until no changes
   while (constantPropagation(head)) {
       changed = true;
       printf("Constant propagation applied\n");
   }


   // Apply constant folding multiple times until no changes
   while (constantFolding(head)) {
       changed = true;
       printf("Constant folding applied\n");
   }


   // Apply dead code elimination
   if (deadCodeElimination(head)) {
       changed = true;
       printf("Dead Code Elimination applied\n");
   }


} while (changed);  // Repeat until no more changes are made






   printOptimizedTAC("TACOptimized.ir", *head);
   printf("Optimized TAC written to TACOptimized.ir\n");
}




// Utility function to check if a string is a valid constant
bool isConstant(const char* str) {
   if (str == NULL || *str == '\0') {
       return false;
   }


   // Optional: Handle negative numbers
   if (*str == '-') {
       ++str;
   }


   // Check if the remaining string is numeric
   while (*str) {
       if (!isdigit((unsigned char)*str)) {
           return false;
       }
       ++str;
   }


   return true;
}




void printOptimizedTAC(const char* filename, TAC* head) {
   FILE* outputFile = fopen(filename, "w");
   if (outputFile == NULL) {
       perror("Failed to open output file");
       exit(EXIT_FAILURE);
   }


   TAC* current = head;
   while (current != NULL) {
       // Skip empty TAC nodes
       if (current->result == NULL && current->arg1 == NULL && current->op == NULL && current->arg2 == NULL) {
           current = current->next;
           continue;
       }


       // Handle 'write' operation
       if (current->op != NULL && strcmp(current->op, "write") == 0) {
           fprintf(outputFile, "%s %s\n", current->op, current->arg1);  // Write operation is special case
       }
       // Handle standard operations (result = arg1 op arg2)
       else if (current->op != NULL && strcmp(current->op, "=") == 0) {
           fprintf(outputFile, "%s = %s\n", current->result, current->arg1);
       }
       else {
           // Print normal operations
           if (current->result != NULL) {
               fprintf(outputFile, "%s = ", current->result);
           }
           if (current->arg1 != NULL) {
               fprintf(outputFile, "%s", current->arg1);
           }
           if (current->op != NULL && strcmp(current->op, "=") != 0) {
               fprintf(outputFile, " %s", current->op);
           }
           if (current->arg2 != NULL) {
               fprintf(outputFile, " %s", current->arg2);
           }
           fprintf(outputFile, "\n");
       }


       current = current->next;
   }


   fclose(outputFile);
}




// Use this function to create TAC entries with dynamically allocated strings
TAC* createTAC(const char* result, const char* arg1, const char* op, const char* arg2) {
   TAC* tac = (TAC*)malloc(sizeof(TAC));
   if (tac == NULL) {
       perror("Failed to allocate memory for TAC");
       exit(EXIT_FAILURE);
   }
   tac->result = result ? strdup(result) : NULL;
   tac->arg1 = arg1 ? strdup(arg1) : NULL;
   tac->op = op ? strdup(op) : NULL;
   tac->arg2 = arg2 ? strdup(arg2) : NULL;
   tac->next = NULL;
   return tac;
}


void freeTACList(TAC* head) {
   TAC* current = head;
   while (current != NULL) {
       TAC* next = current->next;


       // Free all dynamically allocated strings
       if (current->result) free(current->result);
       if (current->arg1) free(current->arg1);
       if (current->op) free(current->op);
       if (current->arg2) free(current->arg2);


       // Free the TAC node itself
       free(current);


       current = next;
   }
}
