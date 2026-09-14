#include <iostream>
#include <tuple>
#include <vector>
#include <random>
#include <iomanip>
using namespace std;

int main() {

	mt19937 rng(random_device{}());
	uniform_int_distribution<int> dist(0, 99);

	tuple<int, int> t1 = make_tuple(dist(rng), dist(rng));
	tuple<int, int> t2 = make_tuple(dist(rng), dist(rng));
	tuple<int, int> t3 = make_tuple(dist(rng), dist(rng));
	tuple<int, int> t4 = make_tuple(dist(rng), dist(rng));

	vector<tuple<int, int>> vec;
	vec.push_back(t1);
	vec.push_back(t2);
	vec.push_back(t3);
	vec.push_back(t4);
	for (const auto& t : vec) {
		cout << "(" << get<0>(t) << ", " << get<1>(t) << ")\n";
	}

	// генерация двух чисел в кортеже
	

	return 0;	
}
