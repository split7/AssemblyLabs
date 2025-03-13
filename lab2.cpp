#include <iostream>
#include <limits>


//(4*b/c + 1)/(2*c+a*c- b) 1 2 3
void computeC(int a, int b, int c) {
    printf("Result C++ = %d\n", (4 * b / c + 1) / (2 * c + a * c - b));
}

void compute(int a, int b, int c) {
    int result;
    int flag = 0;
    asm (
        "mov eax, esi \n" // eax = esi = b
        "imul eax, 4 \n" // //eax = eax * 4 = b * 4
        "jo error_of \n" //ошибка переполнения
        "cmp edi, 0 \n" // //Сравнение edi = c и 0
        "jz error_zf\n" //Ошибка деления на 0
        "cdq \n" //eax ==> edx:eax
        "idiv edi \n" //eax = eax / edi = 4 * b / c
        "inc eax \n" //eax = eax + 1 = 4 * b / c + 1
        "jo error_of\n" //ошибка переполнения
        "mov ecx, eax \n" //ecx = eax = 4 * b / c + 1
        "mov eax, edi \n" //eax = c
        "sal eax, 1 \n" //eax = c << 1 = 2 * c
        "jo error_of \n" //ошибка переполнения
        "imul ebx, edi \n" //ebx = ebx * edi = a * c
        "jo error_of\n" //ошибка переполнения
        "add eax, ebx \n" //eax = 2 * c + a * c
        "jo error_of \n" //ошибка переполнения
        "sub eax, esi \n" //eax = 2 * c + a * c - b
        "jz error_zf\n" //Если деление на 0, то ошибка
        "jo error_of \n" //ошибка переполнения
        "mov ebx, eax \n" // ebx = eax = 2 * c + a * c - b
        "mov eax, ecx \n" // eax = ecx = 4 * b / c + 1
        "cdq\n" //eax ==> edx:eax
        "idiv ebx \n" // eax = eax / ebx = (4 * b / c + 1)/(2 * c + a * c - b)
        "mov ebx, 0 \n" //ebx = 0, значение флага ошибки
        "jmp exit \n" //Безусловный переход
        "error_of: \n" //метка, если была ошибка переполнения
        "mov ebx, 1 \n" //ebx = 1, значение флага с ошибкой переполнения
        "jmp exit \n" //Безусловный переход
        "error_zf: \n" //метка, если была ошиюка деления на 0
        "mov ebx, 2 \n" //ebx = 2, значение флага с ошибкой деления на 0
        "exit: \n" //Выход
        :"+a"(result), "=b" (flag) //result <- eax, flag <- ebx, выходные параметры
        :"b"(a), "S"(b), "D"(c) // ebx <- a, esi <- b, edi <- c, входные параметры
        :"ecx", "edx"//список задействованных регистров, кроме перечисленных выше
    );
    if (flag == 1) {
        printf("%s\n", "Ошибка переполнения - result asm");
    } else if (flag == 2) {
        printf("%s\n", "Ошибка деления на 0 - result asm");
    } else {
        printf("Result asm = %d\n", result);
    }
}

void uncorrect() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "I don't understand you, sorry. Please, try again.\n";
}

int checkInt() {
    int res;
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
    int a, b, c;
    std::cout << "Enter a: " << std::endl;
    a = checkInt();
    std::cout << "Enter b: " << std::endl;
    b = checkInt();
    std::cout << "Enter c: " << std::endl;
    c = checkInt();
    compute(a, b, c);
    computeC(a, b, c);
}
