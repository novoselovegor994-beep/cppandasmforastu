#pragma once
#include "Date.h"
#include <cstring>
#include <stdexcept>

using namespace std;
struct Lodger {
	char id[8]{}; // номер id
	int familymembers = 0; // членов семьи
	int year; // год постанговки в очередь
	int month; // месяц
	double occupiedArea = 0.0; // занимаемая жилая площадь
	int requiredRooms = 0;
	double requiredArea = 0.0;

	Lodger() = default;
	Lodger(const char* _id, int fam, int y, int m,
		double occ, int rooms, double reqArea)
	{
		if (!_id || strlen(_id) != 8)
			throw invalid_argument("Должен быть 8-ми символьным");
		if (fam < 0 || occ < 0 || rooms < 0 || reqArea < 0)
			throw invalid_argument("отрицательные числа");
		memcpy(id, _id, 8);
		familymembers = fam;
		year = y;
		month = m;
		occupiedArea = occ;
		requiredRooms = rooms;
		requiredArea = reqArea;
	}
		
	Date placementDate() const { return Date(year, month, 1); }

	bool operator==(const Lodger& o) const { return memcmp(id, o.id, 8) == 0; }
	bool operator<(const Lodger& o) const { return memcmp(id, o.id, 8) < 0;  }
};