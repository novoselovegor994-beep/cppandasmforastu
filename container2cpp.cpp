#include <iostream>
#include <tuple>
#include <vector>
#include <random>
#include <iomanip>
using namespace std;

int main() {
	mt19937 rng(random_device{}());
	uniform_int_distribution<int> dist(0, 99);

	tuple<int, int> t;
	t = make_tuple(dist(rng), dist(rng));

	cout << get<0>(t) << ", " << get<1>(t) << "\n";
	return 0;	
}
