#pragma once
#include <cstddef>
#include <utility>
using namespace std;

constexpr size_t NOT_FOUND = static_cast<size_t>(-1);

// ---- 1. Последовательный поиск с барьером (неупорядоченный контейнер) ----
template<typename Container, typename Key, typename KeyFn>
size_t sequential_search_barrier(Container& c,
    const Key& key,
    KeyFn keyfn,
    const typename Container::value_type& barrier) {
    const size_t n = c.size();
    c.push_back(barrier);
    size_t i = 0;
    while (!(keyfn(c[i]) == key)) ++i;
    c.pop_back();
    return (i == n) ? NOT_FOUND : i;
}

// ---- 2. Последовательный поиск по упорядоченному контейнеру ----
template<typename Container, typename Key, typename KeyFn>
size_t sequential_search_ordered(const Container& c,
    const Key& key,
    KeyFn keyfn) {
    for (size_t i = 0; i < c.size(); ++i) {
        const auto k = keyfn(c[i]);
        if (k == key) return i;
        if (key < k)  return NOT_FOUND;
    }
    return NOT_FOUND;
}

// ---- 3. Бинарный поиск по упорядоченному контейнеру ----
template<typename Container, typename Key, typename KeyFn>
size_t binary_search_my(const Container& c,
    const Key& key,
    KeyFn keyfn) {
    size_t lo = 0, hi = c.size();
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        const auto k = keyfn(c[mid]);
        if (k < key)      lo = mid + 1;
        else if (key < k) hi = mid;
        else              return mid;
    }
    return NOT_FOUND;
}

// ---- 4. Интерполяционный поиск (числовой ключ) ----
template<typename Container, typename KeyFn>
size_t interpolation_search(const Container& c,
    double key,
    KeyFn keyfn) {
    if (c.empty()) return NOT_FOUND;
    size_t lo = 0, hi = c.size() - 1;
    while (lo <= hi) {
        double a = static_cast<double>(keyfn(c[lo]));
        double b = static_cast<double>(keyfn(c[hi]));
        if (a == b) {
            if (a == key) return lo;
            return NOT_FOUND;
        }
        if (key < a || key > b) return NOT_FOUND;
        double frac = (key - a) / (b - a);
        size_t pos = lo + static_cast<size_t>(frac * (hi - lo));
        if (pos < lo) pos = lo;
        if (pos > hi) pos = hi;
        double v = static_cast<double>(keyfn(c[pos]));
        if (v == key) return pos;
        if (v < key) lo = pos + 1;
        else {
            if (pos == 0) break;
            hi = pos - 1;
        }
    }
    return NOT_FOUND;
}

// ---- 5. Сортировка выбором ----
template<typename Container, typename Less>
void selection_sort(Container& c, Less less) {
    for (size_t i = 0; i + 1 < c.size(); ++i) {
        size_t m = i;
        for (size_t j = i + 1; j < c.size(); ++j)
            if (less(c[j], c[m])) m = j;
        if (m != i) std::swap(c[i], c[m]);
    }
}
