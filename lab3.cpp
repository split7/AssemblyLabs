#include <iostream>
#include <limits>


int computeC(int a, int b){
    int result = 0;
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
int compute(int a, int b) {
    int result;
    int flag = 0;
    asm (
        "mov eax, ebx \n" //eax = ebx = a
        "cmp ebx, esi \n" //Сравнение ebx и esi: ebx - esi
        "je equals \n" //Если равны
        "jl less \n" //Если меньше
        "sub eax, esi \n" //eax = eax - esi = a - b
        "jo error_of \n" //Ошибка переполнения
        "add ebx, esi \n" //ebx = ebx + esi = a + b
        "jo error_of \n" //Ошибка переполнения
        "jz error_zf \n" //Деление на 0
        "cdq \n"  //eax ==> edx:eax
        "idiv ebx \n" //eax = eax / ebx = (a-b) / (a+b)
        "jmp exit \n"
        "less: \n"
        "imul eax, 3 \n" // eax = eax * 3 = a * 3
        "jo error_of \n" //Ошибка переполнения
        "sub eax, 2 \n" //eax = eax - 2 = a * 3 - 2
        "jo error_of \n" //Ошибка переполнения
        "test esi, 0 \n"
        "jz error_zf \n" //Деление на 0
        "cdq \n" //eax ==> edx:eax
        "idiv esi \n" // eax = eax / esi = (3a-2) / b
        "jmp exit \n"
        "equals: \n"
        "neg ebx \n" //ebx = -ebx
        "mov eax, ebx \n"  //eax = ebx = -a
        "imul eax, esi \n" //eax = eax * esi = -a * b
        "jo error_of \n" //Ошибка переполнения
        "mov ebx, 0 \n"
        "jmp exit \n" //Безусловный переход
        "error_of: \n" //метка, если была ошибка переполнения
        "mov ebx, 1 \n" //ebx = 1, значение флага с ошибкой переполнения
        "jmp exit \n" //Безусловный переход
        "error_zf: \n" //метка, если была ошиюка деления на 0
        "mov ebx, 2 \n" //ebx = 2, значение флага с ошибкой деления на 0
        "exit: \n" //Выход
        :"+a"(result), "=b" (flag) //result <- eax, flag <- ebx, выходные параметры
        :"b"(a), "S"(b) // ebx <- a, esi <- b, входные параметры
        :"edx"//список задействованных регистров, кроме перечисленных выше
    );
    if (flag == 1) {
        throw std::invalid_argument("Ошибка переполнения - result asm");
    } else if (flag == 2) {
        throw std::invalid_argument("Ошибка деления на 0 - result asm");
    }
    return result;
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
int main() {
    int a, b;
    std::cout << "Enter a: " << std::endl;
    a = checkInt();
    std::cout << "Enter b: " << std::endl;
    b = checkInt();
    std::cout << "Result asm = " << compute(a, b) << std::endl;
    std::cout << "Result C++ = " << computeC(a, b) << std::endl;
}
