#include <iostream>
#include <limits>


//(4*b/c + 1)/(2*c+a*c- b) 1 2 3
void computeC(double a, double b, double c) {
    printf("Result C++ = %f\n", (4 * b / c + 1) / (2 * c + a * c - b));
}

void compute(double a, double b, double c) {
    double result;
    int flag = 0;
    const double c4 = 4.0, c2 = 2.0, c1 = 1.0;

    __asm__ __volatile__ (
        "finit\n\t"
        // Вычисляем числитель (4*b/c + 1)
        "fld %[b]\n\t"            // st0 = b
        "fmul %[c4]\n\t"          // st0 = 4*b
        "fld %[c]\n\t"            // st0 = c, st1 = 4*b
        "ftst\n\t"
        "fnstsw %%ax\n\t"
        "sahf\n\t"
        "jz 1f\n\t"               // Ошибка деления на 0

        // Вычисление 4*b/c
        "fdivp %%st(1), %%st(0)\n\t"  // st0 = (4*b)/c

        // Добавляем 1 к результату
        "fadd %[c1]\n\t"          // st0 = 4*b/c + 1 (числитель)

        // Вычисляем знаменатель (2*c + a*c - b)
        "fld %[c]\n\t"            // st0 = c, st1 = числитель
        "fmul %[c2]\n\t"          // st0 = 2*c
        "fld %[a]\n\t"            // st0 = a, st1 = 2*c
        "fmul %[c]\n\t"           // st0 = a*c
        "faddp\n\t"               // st0 = 2*c + a*c
        "fsub %[b]\n\t"           // st0 = 2*c + a*c - b (знаменатель)

        // Проверка знаменателя на 0
        "ftst\n\t"
        "fnstsw %%ax\n\t"
        "sahf\n\t"
        "jz 1f\n\t"               // Ошибка деления на 0

        // Деление числителя (st1) на знаменатель (st0)
        "fdivp %%st(1), %%st(0)\n\t"  // st0 = числитель / знаменатель
        "fstp %[result]\n\t"      // Сохраняем результат
        "mov %[flag], 0\n\t"
        "jmp 2f\n\t"

        "1:\n\t"                  // Обработка ошибки
        "mov %[flag], 2\n\t"
        "finit\n\t"               // Сброс FPU

        "2:\n\t"
        : [result] "=m" (result),
          [flag] "=r" (flag)
        : [a] "m" (a),
          [b] "m" (b),
          [c] "m" (c),
          [c4] "m" (c4),
          [c2] "m" (c2),
          [c1] "m" (c1)
        : "eax", "st", "st(1)", "st(2)", "st(3)", "st(4)", "cc"
    );

    if (flag == 2) {
        printf("Ошибка деления на 0 - result asm\n");
    } else {
        printf("Result asm = %f\n", result);
    }
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

//(4*b/c + 1)/(2*c+a*c- b) 2 2 2 = 0
//1 1 1 = 2
//1 1 0 - ошибка деления на 0
//2 4 2 = 2
//computeC(210000000, 1, 2222);
int main() {
    double a, b, c;
    std::cout << "Enter a: " << std::endl;
    a = checkDouble();
    std::cout << "Enter b: " << std::endl;
    b = checkDouble();
    std::cout << "Enter c: " << std::endl;
    c = checkDouble();
    compute(a, b, c);
    computeC(a, b, c);
}
