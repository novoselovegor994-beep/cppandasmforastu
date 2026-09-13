#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <random>
#include <numeric>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <clocale>
using namespace std;

using Triangle = tuple<int, int>;   // (основание, высота)

// Площадь
double area(const Triangle& t) {
    auto [b, h] = t;
    return 0.5 * b * h;
}

// Периметр (в рамках учебного задания — сумма основания и высоты)
int perimeter(const Triangle& t) {
    auto [b, h] = t;
    return b + h;
}

// Функтор сравнения: сначала площадь, при равенстве — периметр
struct TriangleLess {
    bool operator()(const Triangle& a, const Triangle& b) const {
        double sa = area(a), sb = area(b);
        if (sa != sb) return sa < sb;
        return perimeter(a) < perimeter(b);
    }
};

int main() {
    setlocale(LC_ALL, "");
    srand(static_cast<unsigned>(time(nullptr)));

    // ---------- a) emplace_back / push_back ----------
    vector<Triangle> emplaceVec;
    emplaceVec.reserve(10);
    for (int i = 0; i < 10; ++i)
        emplaceVec.emplace_back(rand() % 20 + 1, rand() % 20 + 1);

    vector<Triangle> pushVec;
    pushVec.reserve(10);
    for (int i = 0; i < 10; ++i)
        pushVec.push_back({ rand() % 20 + 1, rand() % 20 + 1 });

    cout << "=== a) emplace_back ===\n";
    for (auto [b, h] : emplaceVec) cout << "(" << b << "," << h << ") ";
    cout << "\n\n=== a) push_back ===\n";
    for (auto [b, h] : pushVec) cout << "(" << b << "," << h << ") ";
    cout << "\n\n";

    // ---------- b) generate / generate_n ----------
    vector<Triangle> genVec(10);
    generate(genVec.begin(), genVec.end(), [] {
        return make_tuple(rand() % 20 + 1, rand() % 20 + 1);
        });

    vector<Triangle> genNVec;
    generate_n(back_inserter(genNVec), 10, [] {
        return make_tuple(rand() % 20 + 1, rand() % 20 + 1);
        });

    cout << "=== b) generate ===\n";
    for (auto [b, h] : genVec) cout << "(" << b << "," << h << ") ";
    cout << "\n\n=== b) generate_n + back_inserter ===\n";
    for (auto [b, h] : genNVec) cout << "(" << b << "," << h << ") ";
    cout << "\n\n";

    // Работаем дальше с одним вектором
    vector<Triangle> v = genVec;

    // ---------- в) sort с функтором ----------
    sort(v.begin(), v.end(), TriangleLess{});
    cout << "=== в) sort (площадь, при равенстве — периметр) ===\n";
    for (auto [b, h] : v)
        cout << "(" << b << "," << h << ") S=" << area({ b,h })
        << " P=" << perimeter({ b,h }) << "  ";
    cout << "\n\n";

    // ---------- г) unique + erase ----------
    // unique по тому же критерию, что и sort
    auto last = unique(v.begin(), v.end(), [](const Triangle& a, const Triangle& b) {
        return area(a) == area(b) && perimeter(a) == perimeter(b);
        });
    v.erase(last, v.end());
    cout << "=== г) unique + erase ===\n";
    for (auto [b, h] : v) cout << "(" << b << "," << h << ") ";
    cout << "\n\n";

    // ---------- д) shuffle ----------
    mt19937 g(static_cast<unsigned>(time(nullptr)));
    shuffle(v.begin(), v.end(), g);
    cout << "=== д) shuffle ===\n";
    for (auto [b, h] : v) cout << "(" << b << "," << h << ") ";
    cout << "\n\n";

    // ---------- е) count_if с лямбдой ----------
    int A = 10, B = 25;   // диапазон периметра
    int cnt = count_if(v.begin(), v.end(), [A, B](const Triangle& t) {
        int p = perimeter(t);
        return p >= A && p <= B;
        });
    cout << "=== е) count_if (периметр в [" << A << "," << B << "]) ===\n";
    cout << "Количество: " << cnt << "\n\n";

    // ---------- Минимум и максимум ----------
    auto [minIt, maxIt] = minmax_element(v.begin(), v.end(), TriangleLess{});
    cout << "=== min / max ===\n";
    cout << "min: (" << get<0>(*minIt) << "," << get<1>(*minIt) << ") S=" << area(*minIt) << "\n";
    cout << "max: (" << get<0>(*maxIt) << "," << get<1>(*maxIt) << ") S=" << area(*maxIt) << "\n";
    cout << "Среднее по площади: "
        << accumulate(v.begin(), v.end(), 0.0,
            [](double acc, const Triangle& t) { return acc + area(t); }) / v.size()
        << "\n\n";

    // ---------- з) transform / for_each ----------
    double minArea = area(*minIt);
    double maxArea = area(*maxIt);
    double diff = maxArea - minArea;

    cout << "=== з) transform: делим на (max - min) = " << diff << " ===\n";

    // Условие замены: периметр в [A, B] (то же, что в count_if)
    transform(v.begin(), v.end(), v.begin(),
        [A, B, diff](const Triangle& t) {
            int p = perimeter(t);
            if (p >= A && p <= B && diff != 0.0) {
                auto [b, h] = t;
                return make_tuple(
                    static_cast<int>(b / diff),
                    static_cast<int>(h / diff)
                );
            }
            return t;
        });

    for (auto [b, h] : v) cout << "(" << b << "," << h << ") ";
    cout << "\n\n";

    // Демонстрация for_each
    cout << "=== з) for_each: печатаем площадь каждого ===\n";
    for_each(v.begin(), v.end(), [](const Triangle& t) {
        cout << "S=" << area(t) << "  ";
        });
    cout << "\n";

    return 0;
}