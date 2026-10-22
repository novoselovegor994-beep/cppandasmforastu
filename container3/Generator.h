#pragma once
#include "Lodger.h"
#include <random>
#include <string>
using namespace std;

inline Lodger randomLodger() {
    static mt19937 gen(random_device{}());
    static uniform_int_distribution<int>    fam_d(1, 8);
    static uniform_int_distribution<int>    year_d(2000, 2024);
    static uniform_int_distribution<int>    month_d(1, 12);
    static uniform_real_distribution<double> occ_d(5.0, 80.0);
    static uniform_int_distribution<int>    rooms_d(1, 5);
    static uniform_real_distribution<double> req_d(20.0, 200.0);
    static uniform_int_distribution<int>    digit_d('0', '9');

    char id[9]{};
    for (int i = 0; i < 8; ++i) id[i] = static_cast<char>(digit_d(gen));

    return Lodger(id, fam_d(gen), year_d(gen), month_d(gen),
        occ_d(gen), rooms_d(gen), req_d(gen));
}