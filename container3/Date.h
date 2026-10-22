#pragma once
#include <cstring>
#include <stdexcept>
#include <cstdio>
using namespace std;

struct Date {
	char data[9]{};
	Date() { memset(data, '0', 8); }

	Date(int y, int m, int d) {
		if (y < 1900 || y > 2100 || m < 1 || m > 12 || d < 1 || d > 31)
			throw  invalid_argument("Некоректные данные");
		snprintf(data, 9, "%04d%02d%02d", y, m, d);
	}
	explicit Date(const char* s) {
		if (!s || strlen(s) != 8)
			throw invalid_argument("Дата должна содрежать 8 символов");
		memcpy(data, s, 8);
	}
	int year() const { return (data[0] - '0') * 1000 + (data[1] - '0') * 100 + (data[2] - '0') * 10 + (data[3] - '0'); } // год. ASCI кодировка
	int month() const { return (data[4] - '0') * 10 + (data[5] - '0'); }
	int day() const { return (data[6] - '0') * 10 + (data[7] - '0'); }

	bool operator<(const Date& o) const { return memcmp(data, o.data, 8) < 0; }
	bool operator==(const Date& o) const { return memcmp(data, o.data, 8) == 0; }
	bool operator>(const Date& o) const { return o < *this; }
};