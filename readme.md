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
        } else if ((a + d) < c) {
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

