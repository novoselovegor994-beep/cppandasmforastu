#include <iostream>
#include <iomanip>
#include <clocale>
#include <utility>      // Для std::swap

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Разработчик: Новоселов Егор" << endl;;
    cout << "Лабораторная работа: 7" << endl;
    cout << "Вариант: 8\n\n";

    // Исходные значения вводим как double.
    double inputX, inputY;

    cout << "Введите x и y через пробел: ";
    if (!(cin >> inputX >> inputY))
    {
        cout << "Ошибка ввода.\n";
        return 1;
    }

    // =========================================================
    // 1. Решение для типа float
    // =========================================================

    // Преобразуем введенные значения к типу float.
    float xFloat = static_cast<float>(inputX);
    float yFloat = static_cast<float>(inputY);

    // Сохраняем те же значения для проверки на C++.
    float xFloatCpp = xFloat;
    float yFloatCpp = yFloat;

    __asm
    {
        // Регистры сопроцессора организованы в стек.
        // ST(0), или просто ST, — вершина стека.
        // ST(1) — следующий элемент стека.

        fld yFloat          // Загрузить y: ST(0) = y.
        fld xFloat          // Загрузить x: ST(0) = x, ST(1) = y.

        fcomi st, st(1)     // Сравнить x с y и установить флаги CPU.
                            // Значения из стека не удаляются.

                            jae float_save      // Если x >= y, порядок уже правильный.
                                                // Переходим к сохранению.

                                                fxch st(1)          // Если x < y, поменять ST(0) и ST(1).
                                                                    // Теперь ST(0) = y (большее),
                                                                    // ST(1) = x (меньшее).

                                                                float_save:
        fstp xFloat         // Записать большее в xFloat
                            // и удалить вершину стека.
                            // Меньшее теперь находится в ST(0).

            fstp yFloat         // Записать меньшее в yFloat
                                // и удалить его из стека.
                                // Стек возвращен в исходное состояние.
    }

    // Проверка обычным кодом C++.
    if (xFloatCpp < yFloatCpp)
    {
        swap(xFloatCpp, yFloatCpp);
    }

    cout << "\nРезультаты для float:\n";
    cout << setprecision(9);

    cout << "Ассемблер: x = " << xFloat
        << ", y = " << yFloat << '\n';

    cout << "C++:       x = " << xFloatCpp
        << ", y = " << yFloatCpp << '\n';

    cout << "Проверка: "
        << ((xFloat == xFloatCpp && yFloat == yFloatCpp)
            ? "результаты совпадают"
            : "результаты НЕ совпадают")
        << '\n';

    // =========================================================
    // 2. Решение для типа double
    // =========================================================

    double xDouble = inputX;
    double yDouble = inputY;

    // Копии исходных значений для проверки на C++.
    double xDoubleCpp = xDouble;
    double yDoubleCpp = yDouble;

    __asm
    {
        fld yDouble         // ST(0) = y.
        fld xDouble         // ST(0) = x, ST(1) = y.

        fcomi st, st(1)     // Сравнить x и y.

        jae double_save     // Если x >= y, обмен не нужен.

        fxch st(1)          // Иначе поменять значения местами:
                            // ST(0) = большее, ST(1) = меньшее.

                            double_save:
        fstp xDouble        // Сохранить большее в xDouble, снять со стека.
            fstp yDouble        // Сохранить меньшее в yDouble, снять со стека.
    }

    // Проверка обычным кодом C++.
    if (xDoubleCpp < yDoubleCpp)
    {
        swap(xDoubleCpp, yDoubleCpp);
    }

    cout << "\nРезультаты для double:\n";
    cout << setprecision(17);

    cout << "Ассемблер: x = " << xDouble
        << ", y = " << yDouble << '\n';

    cout << "C++:       x = " << xDoubleCpp
        << ", y = " << yDoubleCpp << '\n';

    cout << "Проверка: "
        << ((xDouble == xDoubleCpp && yDouble == yDoubleCpp)
            ? "результаты совпадают"
            : "результаты НЕ совпадают")
        << '\n';

    // Ожидание Enter перед закрытием окна.
    cout << "\nНажмите Enter для выхода...";
    cin.ignore(10000, '\n');
    cin.get();

    return 0;
}