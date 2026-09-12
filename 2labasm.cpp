#include <iostream>
#include <conio.h>
using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	int y; // переменные
	int chsl = 0, // числитель дроби
		znm = 0; // знаменатель
	int y2, // y^2 дроби
		chast = 0, // частное
		ostatok = 0; // остаток от деления

	int zapros;
	cout << "Введите значение y "; cin >> y;
	__asm {
		mov eax, y //загрузили x в регистр eax
		mov ebx, eax // загрузили y в регистр ebx
		imul ebx, ebx // возвели в квадрат y

		mov ecx, eax // ecx = y
		imul ecx, ecx, 3 // ecx = 3y
		mov edx, ebx // edx = y^2
		imul edx, edx, 2 // edx = 2y^2
		sub ecx, edx //ecx = 3y-2y^2
		mov chsl, ecx // сохранили числитель

		mov eax, ebx //eax = y^2
		imul eax, eax, 4 // умножаем на 4
		
		mov edx, y // edx = y
		imul edx, edx, 12 // умножаем на 12
		
		sub eax, edx // вычитаем
		add eax, 9 // добавили 9
		mov znm, eax // записали в знаменатель

		mov ebx, eax // сохраням знаменатель в ebx
		mov eax, ecx // eax числитель
		cdq // расширяем до 64 бит для того чтобы записать дробь
		idiv ebx // eax частное ebx остаток

		mov chast, eax // сохранили частное
		mov ostatok, edx // сохранили остаток
	}
	cout << "y = " << y << endl;
	cout << "Результат деления : частное = " << chast << endl;
	cout << "Остаток = " << ostatok << endl;
	int chs1 = 3 * y - 2 * y * y;
	int znmcpp = 4 * y * y - 12 * y + 9;
	int chast_cpp = chs1 / znmcpp;
	int ostatok_cpp = chs1 % znmcpp;
	cout << "Результаты на cpp: ";
	cout << "частное = " << chast_cpp << endl;
	cout << "остаток = " << ostatok_cpp << endl;

	cout << "\nСовпадение результатов: ";
	if (chast == chast_cpp && ostatok == ostatok_cpp) {
		cout << "Ассемблер и C++ дали одинаковый результат" << endl;
	}
	else {
		cout << "НЕТ есть ошибки" << endl;
	}
	_getch();
	return 0;



}
