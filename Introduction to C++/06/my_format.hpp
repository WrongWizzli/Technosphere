#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <type_traits>
#include <limits>


namespace My_Tools {

    class FormatWrongIndex : virtual public std::exception {
        std::string message;
        public:
        FormatWrongIndex() = delete;
        FormatWrongIndex(const std::string &message) throw(): std::exception(), message(message) {}
        const char *what() const noexcept override {
            return message.c_str();
        }
    };

    struct Unpacker {

        public:
        std::vector<std::string> params;

        template <typename... Args>
        Unpacker(Args&&... args) {
            unpack_next(std::forward<Args>(args)...);
        }
        Unpacker(){}

        const std::string& operator[](const int i) {
            return params[i];
        }

        size_t size() const {
            return params.size();
        }

        private:
        template <typename T>
        void unpack_next(T&& val) {
            params.push_back(std::to_string(val));
        }

        void unpack_next(const char *val) {
            params.push_back(val);
        }

        template <typename... Args>
        void unpack_next(const char *val, Args&&... args) {
            params.push_back(val);
            unpack_next(std::forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        void unpack_next(T&& val, Args&&... args) {
            params.push_back(std::to_string(val));
            unpack_next(std::forward<Args>(args)...);
        }
    };

    template <typename T>
    int32_t try_template(T it, T end_it) {

        int64_t num = 0;
        bool empty = true;

        it++;
        while (it != end_it) {
            if (std::isdigit(*it)) {
                empty = false;
                num = num * 10 + *it - '0';
                if (num > std::numeric_limits<int>::max()) {
                    return -1;
                }
            } else if (*it == '}') {
                break;
            } else {
                return -1;
            }
            it++;
        }
        if (empty) {
            return -1;
        }
        return num;
    }

    template <typename... Args>
    std::string format(const std::string &templ, Args&&... args) {
        Unpacker u(std::forward<Args>(args)...);
        std::string formatted;
        
        auto it_prev = templ.begin();
        auto it = templ.begin();

        size_t nformat_size = 0;
        while (it != templ.end()) {
            if (*it == '}') {
                throw FormatWrongIndex(std::string("Index is not valid inside brackets or brackets are not paired for input string: \'") + templ + '\'');
            } else if (*it != '{') {
                nformat_size++;
                it++;
            } else {
                formatted.insert(formatted.end(), it_prev, it);
                nformat_size = 0;
                int32_t num = try_template(it, templ.end());
                if (num >= 0 && size_t(num) < u.size()) {
                    formatted.insert(formatted.end(), u[num].begin(), u[num].end());
                    while (*it != '}') {
                        it++;
                    }
                    it++;
                    it_prev = it;
                } else {
                    throw FormatWrongIndex(std::string("Index is not valid inside brackets or brackets are not paired for input string: \'") + templ + '\'');
                }
            }
        }
        if (nformat_size) {
            formatted.insert(formatted.end(), it_prev, it);
        }
        return formatted;
    }
}