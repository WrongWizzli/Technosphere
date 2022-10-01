#pragma once

#include <iostream>
#include <string>
#include <type_traits>

namespace My_Tools {

    template <typename T>
    class Iterator {
        private:
        T* data;
        int32_t size;
        int32_t cur_pos;

        public:
        Iterator(T* data, int32_t sz, int32_t pos): data(data), size(sz), cur_pos(pos) {}

        T& operator*() {
            if (cur_pos < 0 || cur_pos >= size) {
                throw std::out_of_range("Iterator Out Of Range!");
            }
            return data[cur_pos];
        }

        Iterator<T> operator++() {
            return Iterator(data, size, ++cur_pos);
        }

        Iterator<T> operator++(int) {
            return Iterator(data, size, cur_pos++);
        }

        Iterator<T> operator--() {
            return Iterator(data, size, --cur_pos);
        }

        Iterator<T> operator--(int) {
            return Iterator(data, size, cur_pos--);
        }

        bool operator==(const Iterator &a) {
            return data == a.data && size == a.size && cur_pos == a.cur_pos;
        }

        bool operator!=(const Iterator &a) {
            return data != a.data || size != a.size || cur_pos != a.cur_pos;
        }

        ~Iterator(){}
    };


    template <typename T>
    class Reverse_Iterator {
        private:
        T* data;
        int32_t size;
        int32_t cur_pos;

        public:
        Reverse_Iterator(T* data, int32_t sz, int32_t pos): data(data), size(sz), cur_pos(pos) {}

        T& operator*() {
            if (cur_pos < 0 || cur_pos >= size) {
                throw std::out_of_range("Reverse Iterator Out of Range!");
            }
            return data[cur_pos];
        }

        Reverse_Iterator<T> operator++() {
            return Reverse_Iterator(data, size, --cur_pos);
        }

        Reverse_Iterator<T> operator++(int) {
            return Reverse_Iterator(data, size, cur_pos--);
        }

        Reverse_Iterator<T> operator--() {
            return Reverse_Iterator(data, size, ++cur_pos);
        }

        Reverse_Iterator<T> operator--(int) {
            return Reverse_Iterator(data, size, cur_pos++);
        }

        bool operator==(const Reverse_Iterator &a) {
            return data == a.data && size == a.size && cur_pos == a.cur_pos;
        }

        bool operator!=(const Reverse_Iterator &a) {
            return data != a.data || size != a.size || cur_pos != a.cur_pos;
        }

        ~Reverse_Iterator(){}
    };


    template <typename T>
    class Vec_Allocator {
        private:
        T* data = nullptr;
        int32_t max_size = -1;

        void move_data(Vec_Allocator &&a) {
            data = a.data;
            max_size = a.max_size;
            a.max_size = -1;
            a.data = nullptr;
        }

        public:
        Vec_Allocator() {}

        Vec_Allocator(const int32_t max_size) {
            data = new T[max_size];
            this->max_size = max_size;
        }

        Vec_Allocator(int32_t max_size, const Vec_Allocator &c) {
            data = new T[max_size];
            int32_t lim = std::max(max_size, c.max_size());
            this->max_size = max_size;
            for (int32_t i = 0; i < lim; ++i) {
                data[i] = c[i];
            }
        }

        Vec_Allocator(const Vec_Allocator &c) {
            delete[] data;
            max_size = c.max_size;
            data = new T[max_size];
            for (int32_t i = 0; i < max_size; ++i) {
                data[i] = c[i];
            }
        }

        Vec_Allocator(const Vec_Allocator &&c) {
            data = c.data;
            max_size = c.max_size;
            c.data = nullptr;
            c.max_size = -1;
        }

        T* get_data() const {
            return data;
        }

        int32_t capacity() const {
            return max_size;
        }

        T& operator[](const int32_t i) const {
            return data[i];
        }

        Vec_Allocator& operator=(const Vec_Allocator &a) {
            if (this == &a) {
                return *this;
            }
            if (data != nullptr) {
                delete[] data;
            }
            data = new T[a.max_size];
            max_size = a.max_size;
            for (int i = 0; i < max_size; ++i) {
                data[i] = a.data[i];
            }
            return *this;
        }

        Vec_Allocator& operator=(Vec_Allocator &&a) {
            if (data != nullptr) {
                delete[] data;
            }
            data = a.data;
            max_size = a.max_size;
            a.data = nullptr;
            a.max_size = -1;
            return *this;
        }

        ~Vec_Allocator() {
            if (data != nullptr) {
                delete[] data;
            }
        }
    };


    template <typename T, class Alloc= Vec_Allocator<T>>
    class Vector {
        private:
        const int32_t LARGE_SIZE = 1e6;
        const int64_t SIZE_LIMIT = 1e7;
        Alloc data;
        int32_t Size;

        void allocate_more_memory() {
            int32_t max_size = data.capacity();
            if (max_size > LARGE_SIZE) {
                max_size += LARGE_SIZE;
            } else {
                max_size *= 2;
            }
            Alloc tmp(max_size);
            for (int32_t i = 0; i < Size; ++i) {
                tmp[i] = data[i];
            }
            data = tmp;
        }

        public:

        explicit Vector(int32_t count) {
            int32_t max_size;
            if (count < LARGE_SIZE) {
                max_size = 2 * count;
            } else {
                max_size = count + LARGE_SIZE;
            }
            data = Alloc(max_size);
            Size = 0;
        }

        explicit Vector(int32_t count, const T& val) {
            int32_t max_size;
            if (count < LARGE_SIZE) {
                max_size = 2 * count;
            } else {
                max_size = count + LARGE_SIZE;
            }
            Size = count;
            data = Alloc(max_size);
            for (int32_t i = 0; i < count; ++i) {
                data[i] = val;
            }
        }

        explicit Vector(std::initializer_list<T> init) {
            auto p0 = init.begin(), pn = init.end();
            int32_t max_size;
            if (init.size() < LARGE_SIZE) {
                max_size = 2 * init.size();
            } else {
                max_size = init.size() + LARGE_SIZE;
            }
            int32_t i = 0;
            data = Alloc(max_size);
            Size = init.size();
            while (p0 != pn) {
                data[i++] = *p0++;
            }
        }

        explicit Vector() {
            data = Alloc(1e2);
            Size = 0;
        }

        Vector(const Vector &c) {
            data = c.data;
            Size = c.Size;
        }

        Vector(Vector &&c) {
            data = c.data;
            Size = c.Size;
        }

        Vector& operator=(const Vector &c) {
            if (this == &c) {
                return *this;
            }
            data = c.data;
            Size = c.Size;
            return *this;
        }

        Vector& operator=(Vector &&c) {
            data = c.data;
            Size = c.Size;
            return *this;
        }

        T operator[](const int32_t i) const {
            if (i < 0 || i >= Size) {
                throw std::out_of_range("Vector index out of range!");
            }
            return data[i];
        }

        T& operator[](const int32_t i) {
            if (i < 0 || i >= data.capacity()) {
                throw std::out_of_range("Vector index out of range!");
            }
            return data[i];
        }

        void push_back(const T& val) {
            if (Size == data.capacity()) {
                allocate_more_memory();
            }
            data[Size++] = val;
        }

        T pop_back() {
            return data[--Size];
        }

        void emplace_back(std::initializer_list<T> l) {
            auto p0 = l.begin(), pn = l.end();
            while (Size + l.size() >= data.capacity()) {
                allocate_more_memory();
            }
            while (p0 != pn) {
                data[Size++] = *p0++;
            }
        }

        bool empty() const {
            return Size == 0;
        }

        int32_t size() const {
            return Size;
        }

        void clear() {
            Size = 0;
        }

        int32_t capacity() const {
            return data.capacity();
        }

        void resize(const int32_t new_size) {
            if (new_size < 0 || new_size >= SIZE_LIMIT) {
                throw std::bad_alloc();
            }
            Alloc tmp(new_size);
            Size = std::min(new_size, Size);
            for (int32_t i = 0; i < Size; ++i) {
                tmp[i] = data[i];
            }
            data = tmp;
        }

        void reserve(const int32_t new_size) {
            if (new_size < 0 || new_size >= SIZE_LIMIT) {
                throw std::bad_alloc();
            }
            if (new_size > data.capacity()) {
                Alloc tmp(new_size);
                for (int32_t i = 0; i < Size; ++i) {
                    tmp[i] = data[i];
                }
                data = tmp;
            }
        }

        Iterator<T> begin() const {
            return Iterator<T>(data.get_data(), Size, 0);
        }

        Iterator<T> end() const {
            return Iterator<T>(data.get_data(), Size, Size);
        }

        Reverse_Iterator<T> rbegin() const {
            return Reverse_Iterator<T>(data.get_data(), Size, Size-1);
        }

        Reverse_Iterator<T> rend() const {
            return Reverse_Iterator<T>(data.get_data(), Size, -1);
        }

        std::string to_string() const {
            std::string a;
            for (int32_t i = 0; i < Size; ++i) {
                a += std::to_string(data[i]);
                a += ',';
            }
            return a;
        }

        ~Vector() {}
    };
}

#include "my_vector.cpp"