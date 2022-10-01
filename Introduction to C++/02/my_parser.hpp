#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <sstream>


namespace My_Parser {
    void default_start_callback();
    void default_end_callback();
    bool is_uint64(const std::string &s);

    using boarder_func = std::function<void()>;

    class Parser{

        private:
        boarder_func start_callback;
        std::function<void(uint64_t)> uint64_callback;
        std::function<void(const std::string&)> str_callback;
        boarder_func end_callback;

        public:
        Parser();
        void change_str_callback(const std::function<void(const std::string&)> &callback);
        void change_uint64_callback(const std::function<void(uint64_t)> &callback);
        void change_start_callback(const boarder_func &callback);
        void change_end_callback(const boarder_func &callback);
        void parse(const std::string &str);
    };
}