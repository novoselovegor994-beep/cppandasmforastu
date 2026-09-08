#include <iostream>
#include <clocale> // подключаем библиотеку для изменения языка
using namespace std; // сокращение standart

int main() {
	setlocale(LC_ALL, "Russian"); //установка русского языка
	int a = 5;
	int b = 4;
	int c = 3;
	int d = 8;
	int Y;
	__asm {
		mov eax, d // eax = d	
		sub eax, b // eax = d - b
		sub eax, a // eax = d - b - a
		add eax, c // eax = d - b - a + c
		mov Y, eax // сохраняем результат в Y
	}
	printf("Результат на ассемблере: d - b - a + c = %d = Y\n", Y); // вывод результата на asm с помощью printf
	int Y_cpp = d - b - a + c; //переменная которая считает на cpp наше выражение
	printf("Результат на c++: d - b - a + c = %d = Y\n", Y_cpp); // вывод результата на cpp с помощью printf
	system("pause");
	return 0;
}