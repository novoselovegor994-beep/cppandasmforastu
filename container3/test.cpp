#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Array.h"
#include "Queue.h"
#include "algorithms.h"
#include "generator.h"

#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <clocale>

using namespace std;

// ---------------- Array ----------------
TEST_CASE("Array: basic operations") {
    Array<int> a;
    CHECK(a.empty());
    CHECK(a.size() == 0);

    a.push_back(10);
    a.push_back(20);
    a.push_back(30);
    CHECK(a.size() == 3);
    CHECK(a[0] == 10);
    CHECK(a[2] == 30);
    CHECK(a.front() == 10);
    CHECK(a.back() == 30);

    a.pop_back();
    CHECK(a.size() == 2);
    CHECK(a.back() == 20);

    a.push_front(5);
    CHECK(a.front() == 5);
    CHECK(a.size() == 3);

    a.pop_front();
    CHECK(a.front() == 10);

    a.insert(1, 99);
    CHECK(a[1] == 99);
    CHECK(a.size() == 3);

    a.erase(1);
    CHECK(a.size() == 2);
    CHECK(a[1] == 20);
}

TEST_CASE("Array: exceptions") {
    Array<int> a;
    CHECK_THROWS_AS(a.pop_back(), out_of_range);
    CHECK_THROWS_AS(a.pop_front(), out_of_range);
    CHECK_THROWS_AS(a.erase(size_t(0)), out_of_range);
    CHECK_THROWS_AS(a.insert(size_t(5), 0), out_of_range);
    CHECK_THROWS_AS(a.at(0), out_of_range);
    CHECK_THROWS_AS(a.front(), out_of_range);
    CHECK_THROWS_AS(a.back(), out_of_range);
}

TEST_CASE("Array: initializer_list, copy and move") {
    Array<int> a = { 1, 2, 3, 4 };
    CHECK(a.size() == 4);

    Array<int> b(a);
    CHECK(b.size() == 4);
    CHECK(b[3] == 4);

    Array<int> c(move(b));
    CHECK(c.size() == 4);
    CHECK(b.empty());

    Array<int> d;
    d = a;
    CHECK(d.size() == 4);
    d = move(c);
    CHECK(d.size() == 4);
    CHECK(c.empty());
}

// ---------------- Date / Lodger ----------------
TEST_CASE("Date: validation") {
    Date d(2020, 5, 17);
    CHECK(d.year() == 2020);
    CHECK(d.month() == 5);
    CHECK(d.day() == 17);

    CHECK_THROWS_AS(Date(1000, 1, 1), invalid_argument);
    CHECK_THROWS_AS(Date(2020, 13, 1), invalid_argument);
    CHECK_THROWS_AS(Date(2020, 1, 32), invalid_argument);
    CHECK_THROWS_AS(Date("123"), invalid_argument);
    CHECK_THROWS_AS(Date(nullptr), invalid_argument);
}

TEST_CASE("Lodger: validation") {
    Lodger l("10000001", 3, 2020, 5, 30.0, 2, 60.0);
    CHECK(memcmp(l.id, "10000001", 8) == 0);
    CHECK(l.familymembers == 3);
    CHECK(l.requiredRooms == 2);
    CHECK(l.requiredArea == doctest::Approx(60.0));

    CHECK_THROWS_AS(Lodger("123", 1, 2020, 1, 10, 1, 10), invalid_argument);
    CHECK_THROWS_AS(Lodger("12345678", -1, 2020, 1, 10, 1, 10), invalid_argument);
    CHECK_THROWS_AS(Lodger("12345678", 1, 2020, 1, -1, 1, 10), invalid_argument);
    CHECK_THROWS_AS(Lodger("12345678", 1, 2020, 1, 10, 1, -1), invalid_argument);
}

// ---------------- Queue ----------------
TEST_CASE("Queue: operations") {
    Queue q("Ivanov", "20240101");
    CHECK(q.empty());

    q.add(Lodger("10000001", 3, 2020, 5, 30.0, 2, 60.0));
    q.add(Lodger("10000002", 5, 2021, 6, 40.0, 3, 90.0));
    q.add(Lodger("10000003", 2, 2022, 7, 20.0, 1, 40.0));
    CHECK(q.size() == 3);

    q.replace(0, Lodger("10000004", 4, 2023, 8, 25.0, 2, 55.0));
    CHECK(memcmp(q[0].id, "10000004", 8) == 0);

    q.remove(0);
    CHECK(q.size() == 2);
    CHECK(memcmp(q[0].id, "10000003", 8) == 0);

    CHECK_THROWS_AS(q.remove(10), out_of_range);
    CHECK_THROWS_AS(q.replace(10, q[0]), out_of_range);
}

TEST_CASE("Task 0: random fill") {
    Queue q("Petrov", "20240501");
    for (int i = 0; i < 50; ++i) q.add(randomLodger());
    CHECK(q.size() == 50);
    for (size_t i = 0; i < q.size(); ++i) {
        CHECK(q[i].familymembers >= 1);
        CHECK(q[i].year >= 2000);
        CHECK(q[i].month >= 1);
        CHECK(q[i].month <= 12);
    }
}

TEST_CASE("Task 1: sequential search with barrier") {
    Queue q("Sidorov", "20240201");
    q.add(Lodger("11111111", 1, 2020, 1, 10.0, 1, 20.0));
    q.add(Lodger("22222222", 2, 2021, 2, 20.0, 2, 40.0));
    q.add(Lodger("33333333", 3, 2022, 3, 30.0, 3, 60.0));

    auto keyfn = [](const Lodger& l) { return string(l.id, l.id + 8); };

    Lodger barrier("22222222", 0, 2000, 1, 0.0, 0, 0.0);
    CHECK(sequential_search_barrier(q.data(), string("22222222"),
        keyfn, barrier) == 1);

    Lodger barrier2("99999999", 0, 2000, 1, 0.0, 0, 0.0);
    CHECK(sequential_search_barrier(q.data(), string("99999999"),
        keyfn, barrier2) == NOT_FOUND);

    CHECK(q.size() == 3);
}

TEST_CASE("Task 2: sequential search ordered") {
    Queue q("Kuznetsov", "20240301");
    q.add(Lodger("A0000001", 1, 2020, 1, 10.0, 1, 10.0));
    q.add(Lodger("B0000002", 2, 2020, 1, 20.0, 2, 30.0));
    q.add(Lodger("C0000003", 3, 2020, 1, 30.0, 3, 50.0));
    q.add(Lodger("D0000004", 4, 2020, 1, 40.0, 4, 70.0));

    auto keyfn = [](const Lodger& l) { return l.requiredArea; };

    CHECK(sequential_search_ordered(q.data(), 30.0, keyfn) == 1);
    CHECK(sequential_search_ordered(q.data(), 50.0, keyfn) == 2);
    CHECK(sequential_search_ordered(q.data(), 99.0, keyfn) == NOT_FOUND);
}

TEST_CASE("Task 3: binary search") {
    Queue q("Morozov", "20240401");
    q.add(Lodger("A0000001", 1, 2020, 1, 10.0, 1, 10.0));
    q.add(Lodger("B0000002", 2, 2020, 1, 20.0, 2, 20.0));
    q.add(Lodger("C0000003", 3, 2020, 1, 30.0, 3, 30.0));
    q.add(Lodger("D0000004", 4, 2020, 1, 40.0, 4, 40.0));
    q.add(Lodger("E0000005", 5, 2020, 1, 50.0, 5, 50.0));

    auto keyfn = [](const Lodger& l) { return l.requiredArea; };
    CHECK(binary_search_my(q.data(), 10.0, keyfn) == 0);
    CHECK(binary_search_my(q.data(), 30.0, keyfn) == 2);
    CHECK(binary_search_my(q.data(), 50.0, keyfn) == 4);
    CHECK(binary_search_my(q.data(), 15.0, keyfn) == NOT_FOUND);
}

TEST_CASE("Task 3: interpolation search") {
    Queue q("Volkov", "20240601");
    for (int i = 1; i <= 10; ++i) {
        char id[9];
        snprintf(id, sizeof(id), "0000000%d", i);
        q.add(Lodger(id, i, 2020, 1, i * 1.0, 1, i * 10.0));
    }

    auto keyfn = [](const Lodger& l) { return l.requiredArea; };
    CHECK(interpolation_search(q.data(), 10.0, keyfn) == 0);
    CHECK(interpolation_search(q.data(), 50.0, keyfn) == 4);
    CHECK(interpolation_search(q.data(), 100.0, keyfn) == 9);
    CHECK(interpolation_search(q.data(), 55.0, keyfn) == NOT_FOUND);
    CHECK(interpolation_search(q.data(), 5.0, keyfn) == NOT_FOUND);
    CHECK(interpolation_search(q.data(), 1000.0, keyfn) == NOT_FOUND);
}

TEST_CASE("Task 4: selection sort") {
    Queue q("Belov", "20240701");
    q.add(Lodger("A0000001", 1, 2020, 1, 10.0, 1, 50.0));
    q.add(Lodger("B0000002", 2, 2020, 1, 20.0, 2, 30.0));
    q.add(Lodger("C0000003", 3, 2020, 1, 30.0, 3, 70.0));
    q.add(Lodger("D0000004", 4, 2020, 1, 40.0, 4, 10.0));

    selection_sort(q.data(), [](const Lodger& a, const Lodger& b) {
        return a.requiredArea < b.requiredArea;
        });
    CHECK(q[0].requiredArea == doctest::Approx(10.0));
    CHECK(q[1].requiredArea == doctest::Approx(30.0));
    CHECK(q[2].requiredArea == doctest::Approx(50.0));
    CHECK(q[3].requiredArea == doctest::Approx(70.0));

    auto keyfn = [](const Lodger& l) { return l.requiredArea; };
    CHECK(binary_search_my(q.data(), 50.0, keyfn) == 2);
    CHECK(binary_search_my(q.data(), 70.0, keyfn) == 3);
}

// ---------------- Project 2 ----------------
TEST_CASE("Project 2: std vs custom") {
    constexpr size_t N = 20000;

    Array<Lodger> arr;
    vector<Lodger> vec;

    for (size_t i = 0; i < N; ++i) {
        Lodger l = randomLodger();
        arr.push_back(l);
        vec.push_back(l);
    }
    CHECK(arr.size() == N);
    CHECK(vec.size() == N);

    auto keyfn = [](const Lodger& l) { return l.requiredArea; };

    // --- 1. Barrier search vs std::find ---
    auto t0 = chrono::high_resolution_clock::now();
    size_t r1 = sequential_search_barrier(arr, keyfn(arr[N / 2]), keyfn, arr[N / 2]);
    auto t1 = chrono::high_resolution_clock::now();
    (void)r1;

    auto t2 = chrono::high_resolution_clock::now();
    bool found_vec = false;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (keyfn(vec[i]) == keyfn(vec[N / 2])) { found_vec = true; break; }
    }
    auto t3 = chrono::high_resolution_clock::now();
    (void)found_vec;

    cout << "\n  Barrier search:  custom = "
        << chrono::duration<double, milli>(t1 - t0).count()
        << " ms ; std::find = "
        << chrono::duration<double, milli>(t3 - t2).count()
        << " ms\n";
    CHECK(true);

    // --- 2. Selection sort vs std::sort ---
    Array<Lodger>   copy_arr(arr);
    vector<Lodger>  copy_vec(vec);
    auto less = [](const Lodger& a, const Lodger& b) {
        return a.requiredArea < b.requiredArea;
    };

    auto t4 = chrono::high_resolution_clock::now();
    selection_sort(copy_arr, less);
    auto t5 = chrono::high_resolution_clock::now();

    auto t6 = chrono::high_resolution_clock::now();
    std::sort(copy_vec.begin(), copy_vec.end(), less);
    auto t7 = chrono::high_resolution_clock::now();

    cout << "  Selection sort:  custom = "
        << chrono::duration<double, milli>(t5 - t4).count()
        << " ms ; std::sort = "
        << chrono::duration<double, milli>(t7 - t6).count()
        << " ms\n";

    // --- 3. Binary search vs std::binary_search ---
    double key = keyfn(copy_arr[N / 2]);

    auto t8 = chrono::high_resolution_clock::now();
    size_t r8 = binary_search_my(copy_arr, key, keyfn);
    auto t9 = chrono::high_resolution_clock::now();
    (void)r8;

    auto t10 = chrono::high_resolution_clock::now();
    bool found_std_bin = false;
    size_t lo = 0, hi = copy_vec.size();
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (keyfn(copy_vec[mid]) < key) lo = mid + 1;
        else if (keyfn(copy_vec[mid]) > key) hi = mid;
        else { found_std_bin = true; break; }
    }
    auto t11 = chrono::high_resolution_clock::now();

    CHECK(found_std_bin);

    cout << "  Binary search:   custom = "
        << chrono::duration<double, milli>(t9 - t8).count()
        << " ms ; std::binary_search = "
        << chrono::duration<double, milli>(t11 - t10).count()
        << " ms\n\n";
}