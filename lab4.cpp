#include <iostream>
#include <limits>


int computeC(int d, const int* mas, int size){
    int result = 0;
    for (int i = 0; i < size; i++) {
        if (mas[i] < 0 && mas[i] <= d) {
            result += mas[i] * mas[i] * mas[i];
        }
    }
    return result;
}

//Найти сумму кубов всех отрицательных элементов массива A={a[i]}, удовлетворяющих условию: a[i] <=d.

int compute(int d, int* mas, int size) {
    int result;
    int flag = 0;
    asm (
        "xor esi, esi \n" //подготовим регистр индекса в массиве
        "xor edi, edi \n" //Cумма

        "cmp ecx, 0 \n" //Сравнение size с 0
        "jecxz exit \n" //завершить если длина массива 0

        "sub esp, 8 \n"
        "mov [esp], edx \n" //В стеке d
        "xor edx, edx \n"

        "begin_loop: \n"
        "mov eax, [ebx + esi*4] \n" //определяем текущий элемент

        "cmp eax, [esp] \n" //Сравниваем текущий элемент с d
        "jg end_loop \n" //Если больше выходим

        "cmp eax, 0 \n" //Сравниваем текущий элемент с 0
        "jge end_loop \n" //Если больше или равно выходим

        "mov edx, eax \n" //edx = eax
        "imul eax, edx \n" //eax = eax * eax
        "jo error_of \n"

        "imul eax, edx \n" //eax = eax * eax * eax
        "jo error_of \n"

        "add edi, eax \n" //edi - сумма
        "jo error_of \n"

        "end_loop: \n"
        "inc esi \n"
        "loop begin_loop \n"

        "mov ebx, 0 \n"
        "jmp exit \n"
        "error_of: \n" //метка, если была ошибка переполнения
        "mov ebx, 1 \n" //ebx = 1, значение флага с ошибкой переполнения
        "jmp exit \n" //Безусловный переход
        "exit: \n" //Выход
        "mov eax, edi \n"
        "add esp, 8 \n"
        :"+a"(result), "=b" (flag) //result <- eax, flag <- ebx, выходные параметры
        :"b"(mas), "d"(d), "c"(size) // ebx <- mas, edx <- d, ecx <- size входные параметры
        : "edi", "esi"//список задействованных регистров, кроме перечисленных выше
    );
    if (flag == 1) {
        throw std::invalid_argument("Ошибка переполнения - result asm");
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
    int size = 2;
    int* mas = new int[size]{-1291, -1292};
    std::cout << "Введите значение d: ";
    int d = checkInt();
    std::cout << "Result asm = " << compute(d, mas, size) << std::endl;
    std::cout << "Result C++ = " << computeC(d, mas, size) << std::endl;
    delete[] mas;
}