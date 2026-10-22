#pragma once
#include "Array.h"
#include "Lodger.h"
#include <string>
using namespace std;

class Queue {
public:
    using size_type = Array<Lodger>::size_type;
    using value_type = Lodger;
    using iterator = Array<Lodger>::iterator;
    using const_iterator = Array<Lodger>::const_iterator;

    Queue() = default;

    Queue(const char* operatorName, const char* creationDate)
        : operatorName_(operatorName ? operatorName : ""),
        creationDate_(creationDate) {}

    size_type size()     const noexcept { return data_.size(); }
    bool      empty()    const noexcept { return data_.empty(); }
    size_type capacity() const noexcept { return data_.capacity(); }

    iterator       begin()  noexcept { return data_.begin(); }
    const_iterator cbegin() const noexcept { return data_.cbegin(); }
    iterator       end()    noexcept { return data_.end(); }
    const_iterator cend()   const noexcept { return data_.cend(); }

    Lodger& operator[](size_type i) { return data_[i]; }
    const Lodger& operator[](size_type i) const { return data_[i]; }

    const string& operatorName() const noexcept { return operatorName_; }
    const Date& creationDate() const noexcept { return creationDate_; }

    void add(const Lodger& l) { data_.push_back(l); }

    // эффективное удаление в неупорядоченном массиве (swap-with-last, O(1))
    void remove(size_type idx) {
        if (idx >= data_.size()) throw out_of_range("Queue::remove: bad index");
        data_[idx] = data_.back();
        data_.pop_back();
    }

    void replace(size_type idx, const Lodger& l) {
        if (idx >= data_.size()) throw out_of_range("Queue::replace: bad index");
        data_[idx] = l;
    }

    void clear() noexcept { data_.clear(); }
    void reserve(size_type n) { data_.reserve(n); }

    Array<Lodger>& data()       noexcept { return data_; }
    const Array<Lodger>& data() const noexcept { return data_; }

private:
    Array<Lodger> data_;
    string        operatorName_;
    Date          creationDate_;
};