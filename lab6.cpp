#include <iostream>
#include <limits>


double computeC(double a, double b){
    double result = 0;
    if (a > b) {
         result = (a-b) / (a + b);
    }else if (a == b) {
        result = -a * b;
    } else {
        result = (3 * a - 2) / b;
    }
    return result;
}

//     (a-b)/(a+b), a>b
//x =  -a * b, a = b
//     (3a-2) / b, a < b
double compute(double a, double b) {
    double result;
    int flag = 0;
    const double c3 = 3.0, c2 = 2.0;

    __asm__ __volatile__ (
        "finit\n\t"
        "fld %[a]\n\t"          // st0 = a
        "fld %[b]\n\t"          // st0 = b, st1 = a

        // Сравнение a и b
        "fcomi %%st(0), %%st(1)\n\t"      // Сравнить b (st0) и a (st1)
        "jb greater\n\t"         // a > b (st0 > st1)
        "je equal\n\t"           // a == b

        // Случай a < b
        "fstp %%st(0)\n\t"       // Освобождаем st0 (b)
        "fmul %[c3]\n\t"        // st0 = 3*a
        "fsub %[c2]\n\t"        // st0 = 3a - 2

        "fld %[b]\n\t"          // st0 = b, st1 = (3a-2)
        "ftst\n\t"
        "fnstsw %%ax\n\t"
        "sahf\n\t"
        "jz 1f\n\t"              // Ошибка деления на 0
        "fdivp %%st(1), %%st(0)\n\t" // st0 = (3a-2)/b
        "jmp 3f\n\t"

        "greater:\n\t"           // a > b
        "fsubp %%st(1), %%st(0)\n\t" // st0 = a - b
        "fld %[a]\n\t"          // st0 = a, st1 = (a-b)
        "fadd %[b]\n\t"         // st0 = a + b
        "ftst\n\t"
        "fnstsw %%ax\n\t"
        "sahf\n\t"
        "jz 1f\n\t"              // Ошибка деления на 0
        "fdivp %%st(1), %%st(0)\n\t" // st0 = (a-b)/(a+b)
        "jmp 3f\n\t"

        "equal:\n\t"             // a == b
        "fmulp %%st(1), %%st(0)\n\t" // st0 = a*b
        "fchs\n\t"               // Меняем знак
        "jmp 3f\n\t"

        "1:\n\t"                 // Обработка ошибки деления на 0
        "mov %[flag], 2\n\t"
        "finit\n\t"
        "jmp 2f\n\t"

        "3:\n\t"
        "fstp %[result]\n\t"    // Сохраняем результат
        "mov %[flag], 0\n\t"

        "2:\n\t"
        : [result] "=m" (result), [flag] "=r" (flag)
        : [a] "m" (a), [b] "m" (b),
          [c3] "m" (c3), [c2] "m" (c2)
        : "eax", "st", "st(1)", "st(2)", "cc"
    );

    if (flag == 2) {
        throw std::invalid_argument("Ошибка деления на 0 - result asm");
    }
    return result;
}

void uncorrect() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "I don't understand you, sorry. Please, try again.\n";
}

double checkDouble() {
    double res;
    std::cin >> res;
    while (std::cin.fail() || std::cin.peek() != '\n') {
        uncorrect();
        std::cout << "Give me an integer: ";
        std::cin >> res;
    }
    return res;
}
int main() {
    double a, b;
    std::cout << "Enter a: " << std::endl;
    a = checkDouble();
    std::cout << "Enter b: " << std::endl;
    b = checkDouble();
    std::cout << "Result asm = " << compute(a, b) << std::endl;
    std::cout << "Result C++ = " << computeC(a, b) << std::endl;
}
