#include "my_parser.hpp"


void My_Parser::default_start_callback() {
    std::cout << "Parsing started..." << std::endl;
}


void My_Parser::default_end_callback() {
    std::cout << "End of parsing." << std::endl;
}


bool My_Parser::is_uint64(const std::string &s) {
    if (s.empty()) {
        return false;
    }
    auto p = s.begin();
    while (p != s.end()) {
        if (!std::isdigit(*p)) {
            return false;
        }
        ++p;
    }
    try {
        std::stoull(s);
    } catch(std::out_of_range &a) {
        return false;
    }
    return true;
}


My_Parser::Parser::Parser() {
    str_callback = nullptr;
    uint64_callback = nullptr;
    start_callback = default_start_callback;
    end_callback = default_end_callback;
}


void My_Parser::Parser::change_str_callback(const std::function<void(const std::string&)> &callback) {
    str_callback = callback;
}


void My_Parser::Parser::change_uint64_callback(const std::function<void(uint64_t)> &callback) {
    uint64_callback = callback;
}


void My_Parser::Parser::change_start_callback(const boarder_func &callback) {
    start_callback = callback;
}


void My_Parser::Parser::change_end_callback(const boarder_func &callback) {
    end_callback = callback;
}


void My_Parser::Parser::parse(const std::string &str) {
    std::istringstream in(str);
    std::string token;
    
    if (start_callback != nullptr) {
        start_callback();
    }
    while (in >> token) {
        if (is_uint64(token)) {
            if (uint64_callback != nullptr) {
                uint64_callback(std::stoull(token));
            }
        } else {
            if (str_callback != nullptr) {
                str_callback(token);
            }
        }
    }
    if (end_callback != nullptr) {
        end_callback();
    }
}