#include <iostream>
#include <tuple>
#include <vector>
#include <random>
#include <iomanip>
#include <algorithm>
#include <cmath>
using namespace std;

int main() {
	setlocale(LC_ALL, "");
	mt19937 rng(random_device{}());
	uniform_int_distribution<int> dist(20, 50);

	tuple<int, int, int> t1 = make_tuple(dist(rng), dist(rng), dist(rng));
	tuple<int, int, int> t2 = make_tuple(dist(rng), dist(rng), dist(rng));
	tuple<int, int, int> t3 = make_tuple(dist(rng), dist(rng), dist(rng));
	tuple<int, int, int> t4 = make_tuple(dist(rng), dist(rng), dist(rng));

	vector<tuple<int, int, int>> vec;
	vec.push_back(t1);
	vec.push_back(t2);
	vec.push_back(t3);
	vec.push_back(t4);
	sort(vec.begin(), vec.end());
	for (const auto& t : vec) {
		int a = get<0>(t);
		int b = get<1>(t);
		int c = get<2>(t);
		cout << "(" << a << ", " << b << ", " << c << ") -> ";
		int P = a + b + c;
		if (a == b || a == c || b == c) {
			cout << "Есть равные стороны, периметр = " << P << endl;
		}
		else {
			double p = (a + b + c) / 2.0;
			int P = (a + b + c);
			double S = sqrt(p * (p - a) * (p - b) * (p - c));
			cout << "все стороны разные, S = " << fixed << setprecision(2) << S << "," << " Периметр равен = " << P << "\n";
		}

		int max_side = max({ a, b, c });
		int min_side = min({ a, b, c });

		double result = (double)P / (max_side - min_side);
		cout << "4.1 результат деления периметра на разность максимума и минимума = " << fixed << setprecision(2) << result << endl;
	}
	return 0;
}
