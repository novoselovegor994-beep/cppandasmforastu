#include <iostream>
#include <conio.h>
using namespace std;

int main() {
	setlocale(LC_ALL, "Russian"); // установка русского языка
    
    double y;
	cout << "Введите значение y "; cin >> y; // вводим значение y
    double y2, chisl, znam, resultA = 0, resultC = 0;
    double nine = 9.0;
    __asm
    {
        fld y
        fmul y
        fst y2
        fstp st(0)

        // Числитель: 3y - 2y^2
        fld y
        fadd y
        fadd y          // 3y
        fld y2
        fadd y2         // 2y^2
        fsubr           // 3y - 2y^2
        fstp chisl

        // Знаменатель: 4y^2 - 12y + 9
        fld y2
        fadd y2
        fadd y2
        fadd y2         // 4y^2
        fld y
        fadd y
        fadd y
        fadd y
        fadd y
        fadd y
        fadd y
        fadd y
        fadd y
        fadd y
        fadd y
        fadd y          // 12y
        fsubr           // 4y^2 - 12y
        fld nine
        fadd            // 4y^2 - 12y + 9
        fstp znam

        // Деление
        fld chisl
        fdiv znam
        fstp resultA
	}
    cout << "Новоселов Егор. Лабораторная 3" << endl;
    cout << "Результат на языке  Assembler: " << resultA << endl;
    resultC = (3 * y - 2 * y * y) / (4 * y * y - 12 * y + 9);
    cout << "Результат на языее C++ :" << resultC << endl;
    
	_getch();
	return 0;



}