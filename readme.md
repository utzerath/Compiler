Overview

This is a custom compiler implemented in C++ that translates source code into assembly. The project includes modules for lexical analysis, parsing, semantic analysis, code generation, and optimization.

The repository is structured as follows:

lexer.l and lex.yy.c: Lexical analysis using Flex.
parser.y, parser.tab.c, parser.tab.h: Parsing using Bison.
AST.c/h: Abstract Syntax Tree construction.
semantic.c/h: Semantic analysis.
codeGenerator.c/h: Code generation for target assembly.
optimizer.c/h: Intermediate code optimization.
output.s: Final assembly output.
symbolTable.c/h: Symbol table management.
TAC.ir: Three-Address Code representation.
Makefile: Build automation.

Installation
Dependencies:
Ensure the following tools and libraries are installed:
gcc (GCC Compiler)
flex (Lexical Analyzer Generator)
bison (Parser Generator)
make (Build Automation Tool)
Clone the Repository:
git clone <repository-url>
cd <repository-directory>
Build the Compiler: Run the following command to compile all source files:
make

Performance Metrics

I tested it on my Apple M3 chip (16GB RAM). This is a very fast chip, and I wanted to see how quickly it could compile and run a program.
I used a sample program called testProg.cmm, which has about 500 lines of code. It contains basic programming features like loops, conditions, and functions.

Steps I Measured:
I broke the compilation process into 4 parts and measured how long each part took:
Lexical and Syntax Analysis: Breaking the source code into tokens and building the syntax tree.
Semantic Analysis: Checking for errors like type mismatches and undeclared variables.
Code Generation: Converting the syntax tree into assembly code.
Optimization: Making the code faster by removing unnecessary operations.

Execution Time:
After the compiler generated the assembly code, I also measured how long it takes to run the final program.

Results
Lexical & Syntax Analysis	~0.15 seconds
Semantic Analysis	~0.10 seconds
Code Generation	~0.20 seconds
Optimization	~0.10 seconds
Total Compilation Time	~0.55 seconds
Execution Time	~0.010 seconds

What This Means
The Apple M3 chip is very fast, so it can compile a program of 500 lines in about half a second.
The final compiled program runs almost instantly in 0.005 seconds.
This is because the M3 chip is good at handling tasks like parsing, optimizing, and generating code quickly.

Rules of Program

The ability to have global variables is allowed
Must start with "void main()" function that doesnt return anything, other functions can be added below


In cases like this 
result = (a + b > c) || (c - b < a);
parenthesis must be added when having nested comparison so we must add them to the addition and subtraction like shown below
result = ((a + b) > c) || ((c - b) < a);

Below are working example of test inputs to show all the functionality we have
---------------
void main() {
    int a;
    int b;
    int c;
    bool result;

    a = 5;
    b = 10;
    c = 15;

    result = (a < b) && (c >= b);

    if (result) {
        write b;
    } else {
        write c;
    }
}
-----------------
void main() {
    int a;
    int b;
    float l;

    a = 8;
    b = 12;

    l = compute(a, b);

    if (a >= l) {
        write a;
    } else {
        write b;
    }
}
float compute(int x, int y) {
    return x + y;
}
----------------
void main() {
    int a;
    int b;
    int c;

    a = 8;
    b = 12;
    c = 20;

    if (a < b) {
        if (b < c) {
            write a;
        } else {
            write b;
        }
    } else {
        write c;
    }
}
----------------
void main() {
    int x;
    int y;
    bool result;

    x = 10;
    y = 5;

    if (x > 5 && y < 7) {
        write x;
    } else {
        write y;
    }
}
----------------
void main() {
    int a;
    int b;
    int c;
    int d;

    a = 10;
    b = 15;
    c = 20;
    d = 25;

    if (a < b) {
        if ((b + c) > d) {
            write a;
        } if ((a + d) < c) {
            write b;
        } else {
            write c;
        }
    } else {
        write d;
    }
}
---------------
void main() {
    int a;
    float b;
    int l;
    bool result;
    float c;

    a = 10;
    b = 15.5;

    l = intfunc();

    if (l == 10) {
        write a;
    } else {
        write b;
    }

    c = floatfunc();
    if (c == 25.0) {
        write c;
    } else {
        write b;
    }
}

int intfunc() {
    return 10;
}

float floatfunc() {
    return 12.5 * 2;
}
-----------------
void main() {
    int i;
    int j;

    i = 0;
    j = 0;

    while (i < 3) {
        j = 0;
        while (j < 2) {
            if (i == j) {
                write i;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}
--------------------

