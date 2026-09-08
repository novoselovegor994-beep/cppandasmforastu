#include <iostream>
#include <conio.h>
using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int y; // переменные
	int chsl = 0, // числитель дроби
		znm = 0; // знаменатель
	int y2, // y^2 дроби
		result = 0; // результат на асемблере
	int zapros;
		cout << "Введите значение y"; cin >> y;
	__asm {
		mov eax, y //загрузили x в регистр eax
		mov ebx, eax // загрузили y в регистр ebx
		imul ebx, ebx // возвели в квадрат y
		
		mov ecx, eax // ecx = y
		imul ecx, ecx 3 // ecx = 3y
		mov edx, ebx // edx = y^2
		imul edx, edx 2 // edx = 2y^2
		sub ecx, edx //ecx = 3y-2y^2
		mov chsl, ecx // сохранили числитель



	}



	

}