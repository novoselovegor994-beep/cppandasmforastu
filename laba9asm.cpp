#include <iostream>
#include <iomanip>
#include <cmath>
#include <clocale>
#include <limits>

using namespace std;

// task:   номер формулы, от 1 до 3.
// method: 1 — регистры x87;
//         2 — значения float в обычном стеке;
//         3 — адреса параметров в обычном стеке.
// single: true — параметры и результат float;
//         false — параметры и результат double.
//
// Унифицированные параметры:
// задача 1: p = y;
// задача 2: p = x;
// задача 3: p = a, q = b, r = угол в радианах.
//
// Неиспользуемые параметры передаются как нули.
double calculateAsm(int task, int method, bool single,
    double p, double q, double r)
{
    float pf = static_cast<float>(p);
    float qf = static_cast<float>(q);
    float rf = static_cast<float>(r);

    float resultFloat = 0;
    double resultDouble = 0;

    // Рабочие ячейки принимают параметры из регистров x87.
    // float при загрузке в x87 расширяется без потери значения.
    double u, v, w;

    const double two = 2.0;
    const double three = 3.0;
    const double four = 4.0;
    const double five = 5.0;
    const double nine = 9.0;
    const double twelve = 12.0;

    int isFloat = single ? 1 : 0;

    __asm
    {
        // EBX содержит адрес, по которому подпрограмма
        // должна сохранить результат.
        lea ebx, resultDouble
        cmp isFloat, 0
        je result_address_ready
        lea ebx, resultFloat

        result_address_ready :
        cmp method, 1
            je pass_registers

            cmp method, 2
            je pass_float_stack

            // -------------------------------------------------
            // Способ 3: передача адресов в обычном стеке.
            // Параметры кладутся справа налево: &r, &q, &p.
            // -------------------------------------------------
            cmp isFloat, 0
            je push_double_addresses

            lea eax, rf
            push eax
            lea eax, qf
            push eax
            lea eax, pf
            push eax

            call addresses_float
            add esp, 12         // Убираем три адреса по 4 байта.
            jmp calculations_done

            push_double_addresses :
        lea eax, r
            push eax
            lea eax, q
            push eax
            lea eax, p
            push eax

            call addresses_double
            add esp, 12
            jmp calculations_done

            // -------------------------------------------------
            // Способ 1: передача в регистрах сопроцессора.
            // Перед CALL:
            // ST(0) = p, ST(1) = q, ST(2) = r.
            // -------------------------------------------------
        pass_registers:
        cmp isFloat, 0
            je load_double_registers

            fld rf
            fld qf
            fld pf
            call formula
            jmp calculations_done

            load_double_registers :
        fld r
            fld q
            fld p
            call formula
            jmp calculations_done

            // -------------------------------------------------
            // Способ 2: передача значений float в стеке.
            // PUSH копирует 4 байта представления float.
            // Преобразования float в int здесь нет.
            // -------------------------------------------------
        pass_float_stack:
        push dword ptr rf
            push dword ptr qf
            push dword ptr pf

            call values_float
            add esp, 12         // Освобождаем параметры.
            jmp calculations_done

            // =================================================
            // Подпрограмма приема значений float из стека.
            // CALL помещает адрес возврата в [ESP].
            // Поэтому p расположен в [ESP+4], q в [ESP+8],
            // r в [ESP+12].
            // =================================================
        values_float:
        fld dword ptr[esp + 12] // r
            fld dword ptr[esp + 8]  // q
            fld dword ptr[esp + 4]  // p

            call formula        // Вычисление формулы.
            ret                 // Возврат вызывающему коду.

            // =================================================
            // Подпрограмма приема адресов переменных float.
            // =================================================
        addresses_float:
        mov eax, [esp + 12] // EAX = адрес r.
            fld dword ptr[eax] // Загружаем значение по адресу.

            mov eax, [esp + 8]  // Адрес q.
            fld dword ptr[eax]

            mov eax, [esp + 4]  // Адрес p.
            fld dword ptr[eax]

            call formula
            ret

            // =================================================
            // Подпрограмма приема адресов переменных double.
            // Адрес занимает 4 байта в x86,
            // само значение double — 8 байт.
            // =================================================
        addresses_double:
        mov eax, [esp + 12]
            fld qword ptr[eax]

            mov eax, [esp + 8]
            fld qword ptr[eax]

            mov eax, [esp + 4]
            fld qword ptr[eax]

            call formula
            ret

            // =================================================
            // Общая вычисляющая подпрограмма.
            // При входе:
            // ST(0) = p, ST(1) = q, ST(2) = r.
            // Результат сохраняется по адресу в EBX.
            // =================================================
        formula:
        fstp u              // u = p, снять вершину стека x87.
            fstp v              // v = q.
            fstp w              // w = r. Стек x87 теперь пуст.

            cmp task, 1
            je formula_one

            cmp task, 2
            je formula_two

            // -------------------------------------------------
            // Задача 3:
            // S = (a+b)*(a-b)*tan(alpha)/4.
            // u = a, v = b, w = alpha.
            // -------------------------------------------------
            fld w               // ST(0) = alpha.
            fptan               // ST(0) = 1, ST(1) = tan(alpha).
            fstp st(0)          // Удаляем вспомогательную единицу.
                                // ST(0) = tan(alpha).

            fld u               // ST(0) = a, ST(1) = tan(alpha).
            fadd v              // ST(0) = a+b.

            fld u               // ST(0) = a.
            fsub v              // ST(0) = a-b.

            fmulp st(1), st     // ST(0) = (a+b)*(a-b).
            fmulp st(1), st     // ST(0) = (a+b)*(a-b)*tan(alpha).
            fdiv four           // Делим на 4.

            jmp save_result

            // -------------------------------------------------
            // Задача 1:
            // (3y - 2y^2) / (4y^2 - 12y + 9).
            // -------------------------------------------------
        formula_one:
        fld u               // y
            fmul three          // 3y

            fld u               // y, 3y
            fmul u              // y^2, 3y
            fmul two            // 2y^2, 3y
            fsubp st(1), st     // ST(0) = 3y - 2y^2.

            fld u               // y, числитель
            fmul u              // y^2, числитель
            fmul four           // 4y^2, числитель

            fld u               // y, 4y^2, числитель
            fmul twelve         // 12y, 4y^2, числитель
            fsubp st(1), st     // 4y^2 - 12y, числитель
            fadd nine           // Знаменатель, числитель.

            fdivp st(1), st     // Числитель / знаменатель.
            jmp save_result

            // -------------------------------------------------
            // Задача 2:
            // ln((2-x)/(3x+5)).
            //
            // Используем переход к основанию 2:
            // ln(t) = log2(t) * ln(2).
            // FYL2X вычисляет ST(1)*log2(ST(0)).
            // -------------------------------------------------
        formula_two:
        fldln2              // ST(0) = ln(2).

            fld two             // 2, ln(2)
            fsub u              // 2-x, ln(2)

            fld u               // x, 2-x, ln(2)
            fmul three          // 3x, 2-x, ln(2)
            fadd five           // 3x+5, 2-x, ln(2)

            fdivp st(1), st     // (2-x)/(3x+5), ln(2)
            fyl2x               // ln(2)*log2(дроби) = ln(дроби).

            // -------------------------------------------------
            // Возврат результата по адресу в EBX.
            // FSTP также освобождает вершину стека x87.
            // -------------------------------------------------
        save_result:
        cmp isFloat, 0
            je save_double

            fstp dword ptr[ebx] // Сохранить результат как float.
            ret

            save_double :
        fstp qword ptr[ebx] // Сохранить результат как double.
            ret

            calculations_done :
        // Сюда попадаем после завершения вызовов.
        // Обычный стек и стек x87 восстановлены.
    }

    if (single)
        return resultFloat;

    return resultDouble;
}

// Вычисление формулы на C++ для проверки.
// Тип T определяет float или double.
template <typename T>
T calculateCpp(int task, T p, T q, T r)
{
    if (task == 1)
    {
        return (T(3) * p - T(2) * p * p) /
            (T(4) * p * p - T(12) * p + T(9));
    }

    if (task == 2)
    {
        return std::log((T(2) - p) / (T(3) * p + T(5)));
    }

    return (p + q) * (p - q) * std::tan(r) / T(4);
}

// Проверяем область определения после преобразования типа:
// float может округлить исходное double до другого значения.
template <typename T>
bool validParameters(int task, T p, T q, T r)
{
    if (!std::isfinite(p) ||
        !std::isfinite(q) ||
        !std::isfinite(r))
        return false;

    if (task == 1)
    {
        // Знаменатель = (2y-3)^2.
        return p != T(1.5);
    }

    if (task == 2)
    {
        // (2-x)/(3x+5) > 0:
        // -5/3 < x < 2.
        return p < T(2) && T(3)* p + T(5) > T(0);
    }

    const T halfPi = static_cast<T>(
        1.57079632679489661923);

    return p > q && q > T(0) && r > T(0) && r < halfPi;
}

void printComparison(const char* method, double actual,
    double expected, bool single)
{
    cout << method << '\n';
    cout << "  Assembler: " << actual << '\n';
    cout << "  C++:       " << expected << '\n';

    if (!std::isfinite(actual) || !std::isfinite(expected))
    {
        cout << "  Результат вне конечного диапазона типа.\n";
        return;
    }

    double error = std::fabs(actual - expected);

    // При дробных вычислениях возможны небольшие расхождения:
    // x87 хранит промежуточные результаты с расширенной точностью.
    double tolerance = (single ? 1e-5 : 1e-12) *
        (1.0 + std::fabs(expected));

    cout << "  Абсолютная разница: " << error << '\n';
    cout << "  Проверка: "
        << (error <= tolerance
            ? "совпадает в пределах погрешности"
            : "есть расхождение")
        << '\n';
}

void demonstrate(int task, double p, double q, double r)
{
    cout << "\n--- FLOAT ---\n";
    cout << setprecision(9);

    float pf = static_cast<float>(p);
    float qf = static_cast<float>(q);
    float rf = static_cast<float>(r);

    if (validParameters<float>(task, pf, qf, rf))
    {
        float expected = calculateCpp<float>(task, pf, qf, rf);

        printComparison(
            "1. Параметры в регистрах сопроцессора",
            calculateAsm(task, 1, true, pf, qf, rf),
            expected, true);

        printComparison(
            "2. Значения float в стеке",
            calculateAsm(task, 2, true, pf, qf, rf),
            expected, true);

        printComparison(
            "3. Адреса параметров в стеке",
            calculateAsm(task, 3, true, pf, qf, rf),
            expected, true);
    }
    else
    {
        cout << "Параметры вне области определения для float.\n";
    }

    cout << "\n--- DOUBLE ---\n";
    cout << setprecision(17);

    if (validParameters<double>(task, p, q, r))
    {
        double expected = calculateCpp<double>(task, p, q, r);

        printComparison(
            "1. Параметры в регистрах сопроцессора",
            calculateAsm(task, 1, false, p, q, r),
            expected, false);

        // Способ 2 по условию требуется именно для float.

        printComparison(
            "3. Адреса параметров в стеке",
            calculateAsm(task, 3, false, p, q, r),
            expected, false);
    }
    else
    {
        cout << "Параметры вне области определения для double.\n";
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Разработчик: Новоселов Егор\n";
    cout << "Лабораторная работа: 9\n";
    cout << "Вариант: 8\n\n";

    double y;

    cout << "ЗАДАЧА 1\n";
    cout << "z = (3y - 2y^2)/(4y^2 - 12y + 9)\n";
    cout << "Введите y, y != 1.5: ";

    if (!(cin >> y))
    {
        cout << "Ошибка ввода.\n";
        return 1;
    }

    demonstrate(1, y, 0.0, 0.0);

    double x;

    cout << "\nЗАДАЧА 2\n";
    cout << "z = ln((2-x)/(3x+5))\n";
    cout << "Введите x, -5/3 < x < 2: ";

    if (!(cin >> x))
    {
        cout << "Ошибка ввода.\n";
        return 1;
    }

    demonstrate(2, x, 0.0, 0.0);

    double a, b, alphaDegrees;

    cout << "\nЗАДАЧА 3\n";
    cout << "Площадь равнобедренной трапеции.\n";
    cout << "Введите большее основание a, меньшее b\n";
    cout << "и угол при основании a в градусах.\n";
    cout << "Условия: a > b > 0, 0 < угол < 90: ";

    if (!(cin >> a >> b >> alphaDegrees))
    {
        cout << "Ошибка ввода.\n";
        return 1;
    }

    const double PI = 3.14159265358979323846;

    // Тригонометрические команды x87 принимают радианы.
    double alphaRadians = alphaDegrees * PI / 180.0;

    demonstrate(3, a, b, alphaRadians);

    cout << "\nНажмите Enter для выхода...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return 0;
}