#pragma once
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
using namespace std;

template<typename T>
class Array {
public:
    using size_type = size_t;
    using value_type = T;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;

    Array() noexcept : data_(nullptr), size_(0), capacity_(0) {}

    explicit Array(const size_type& n, const value_type& t = value_type())
        : data_(nullptr), size_(0), capacity_(0) {
        if (n == 0) return;
        data_ = new value_type[n];
        for (size_type i = 0; i < n; ++i) data_[i] = t;
        size_ = capacity_ = n;
    }

    Array(const initializer_list<value_type>& init)
        : data_(nullptr), size_(0), capacity_(0) {
        reserve(init.size() ? init.size() : 1);
        for (const auto& v : init) data_[size_++] = v;
    }

    Array(const Array& other) : data_(nullptr), size_(0), capacity_(0) {
        reserve(other.size_);
        for (size_type i = 0; i < other.size_; ++i) data_[i] = other.data_[i];
        size_ = other.size_;
    }

    Array(Array&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    Array& operator=(const Array& other) {
        if (this != &other) { Array tmp(other); swap(tmp); }
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    virtual ~Array() noexcept { delete[] data_; }

    size_type size()     const noexcept { return size_; }
    bool      empty()    const noexcept { return size_ == 0; }
    size_type capacity() const noexcept { return capacity_; }

    iterator       begin()  noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }
    iterator       end()    noexcept { return data_ + size_; }
    const_iterator cend()   const noexcept { return data_ + size_; }

    reference       operator[](size_type i) { return data_[i]; }
    const_reference operator[](size_type i) const { return data_[i]; }

    reference at(size_type i) {
        if (i >= size_) throw out_of_range("Array::at: out of range");
        return data_[i];
    }
    const_reference at(size_type i) const {
        if (i >= size_) throw out_of_range("Array::at: out of range");
        return data_[i];
    }

    reference front() {
        if (empty()) throw out_of_range("Array::front: empty");
        return data_[0];
    }
    const_reference front() const {
        if (empty()) throw out_of_range("Array::front: empty");
        return data_[0];
    }
    reference back() {
        if (empty()) throw out_of_range("Array::back: empty");
        return data_[size_ - 1];
    }
    const_reference back() const {
        if (empty()) throw out_of_range("Array::back: empty");
        return data_[size_ - 1];
    }

    // Безопасный reserve
    void reserve(size_type n) {
        if (n <= capacity_) return;
        value_type* nd = new value_type[n];
        for (size_type i = 0; i < size_; ++i) {
            nd[i] = data_[i]; // Используем копирование, а не move, чтобы избежать сбоев
        }
        delete[] data_;
        data_ = nd;
        capacity_ = n;
    }

    void push_back(const value_type& v) {
        if (size_ == capacity_) {
            size_type new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
            reserve(new_cap);
        }
        data_[size_] = v;
        size_++;
    }

    void pop_back() {
        if (size_ == 0) throw out_of_range("Array::pop_back: empty");
        --size_;
    }

    void push_front(const value_type& v) { insert(size_type(0), v); }

    void pop_front() {
        if (size_ == 0) throw out_of_range("Array::pop_front: empty");
        erase(size_type(0));
    }

    void insert(size_type idx, const value_type& v) {
        if (idx > size_) throw out_of_range("Array::insert: bad index");
        if (size_ == capacity_) {
            size_type new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
            reserve(new_cap);
        }
        for (size_type i = size_; i > idx; --i) data_[i] = data_[i - 1];
        data_[idx] = v;
        ++size_;
    }

    void erase(size_type idx) {
        if (idx >= size_) throw out_of_range("Array::erase: bad index");
        for (size_type i = idx; i + 1 < size_; ++i)
            data_[i] = data_[i + 1];
        --size_;
    }

    void clear() noexcept { size_ = 0; }

    void swap(Array& rhs) noexcept {
        std::swap(data_, rhs.data_);
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
    }

private:
    value_type* data_;
    size_type   size_;
    size_type   capacity_;
};

template<typename T>
void swap(Array<T>& a, Array<T>& b) noexcept { a.swap(b); }