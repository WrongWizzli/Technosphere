#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <assert.h>
#include "my_parser.hpp"


const std::string s1(""), s2("a"), s3("1"), s4("-5\t-05");
const std::string s5("213\tAA1 aa2\n288x"), s6("213 567\t122\n888");
const std::string s7("x7\t7x7\n000009x Y"), s8(" \t\n");

const char *filename = "test.txt";


void log_uint64(uint64_t a) {
    std::ofstream out("test.txt", std::ios::app);
    out << "(u:" << a << ")";
}


void log_string(const std::string &s) {
    std::ofstream out("test.txt", std::ios::app);
    out << "(s:" << s << ")";
}


void log_post_callback() {
    std::ofstream out("test.txt", std::ios::app);
    out << "post";
}


void log_pre_callback() {
    std::ofstream out("test.txt", std::ios::app);
    out << "pre";
}


void test_1() {
    std::cout << "Test 1: only default callbacks on different strings" << std::endl;
    std::cout << "-----------" << std::endl;

    My_Parser::Parser test_parser;

    test_parser.parse(s1);
    test_parser.parse(s2);
    test_parser.parse(s3);
    test_parser.parse(s4);
    test_parser.parse(s5);
    test_parser.parse(s6);
    test_parser.parse(s7);
    test_parser.parse(s8);

    std::cout << "-----------" << std::endl;
    std::cout << "Test 1: OK!" << std::endl;
}


void test_2() {
    std::cout << "Test 2: Changing pre/post callbacks" << std::endl;

    std::ofstream out("test.txt", std::ofstream::trunc);
    const std::string answer("prepostprepostprepostprepostprepostprepostprepostprepost");
    std::string my_result;

    My_Parser::Parser test_parser;
    test_parser.change_start_callback(log_pre_callback);
    test_parser.change_end_callback(log_post_callback);

    test_parser.parse(s1);
    test_parser.parse(s2);
    test_parser.parse(s3);
    test_parser.parse(s4);
    test_parser.parse(s5);
    test_parser.parse(s6);
    test_parser.parse(s7);
    test_parser.parse(s8);

    std::ifstream in("test.txt");
    in >> my_result;
    assert(my_result == answer);

    std::cout << "Test 2: OK!" << std::endl;    
}


void test_3() {
    std::cout << "Test 3: Changing int/str callbacks + nullptr pre/post callbacks" << std::endl;

    std::ofstream out("test.txt", std::ofstream::trunc);
    const std::string answer("(s:a)(u:1)(s:-5)(s:-05)(u:213)(s:AA1)(s:aa2)(s:288x)(u:213)"
                             "(u:567)(u:122)(u:888)(s:x7)(s:7x7)(s:000009x)(s:Y)");
    std::string my_result;

    My_Parser::Parser test_parser;
    test_parser.change_uint64_callback(log_uint64);
    test_parser.change_str_callback(log_string);
    test_parser.change_end_callback(nullptr);
    test_parser.change_start_callback(nullptr);

    test_parser.parse(s1);
    test_parser.parse(s2);
    test_parser.parse(s3);
    test_parser.parse(s4);
    test_parser.parse(s5);
    test_parser.parse(s6);
    test_parser.parse(s7);
    test_parser.parse(s8);

    std::ifstream in("test.txt");
    in >> my_result;
    assert(my_result == answer);

    std::cout << "Test 3: OK!" << std::endl;    
}


void test_4() {
    std::cout << "Test 4: Only str callback" << std::endl;

    std::ofstream out("test.txt", std::ofstream::trunc);
    const std::string answer("(s:a)(s:-5)(s:-05)(s:AA1)(s:aa2)(s:288x)(s:x7)(s:7x7)(s:000009x)(s:Y)");
    std::string my_result;

    My_Parser::Parser test_parser;
    test_parser.change_str_callback(log_string);
    test_parser.change_end_callback(nullptr);
    test_parser.change_start_callback(nullptr);

    test_parser.parse(s1);
    test_parser.parse(s2);
    test_parser.parse(s3);
    test_parser.parse(s4);
    test_parser.parse(s5);
    test_parser.parse(s6);
    test_parser.parse(s7);
    test_parser.parse(s8);

    std::ifstream in("test.txt");
    in >> my_result;
    assert(my_result == answer);
 
    std::cout << "Test 4: OK!" << std::endl;    
}


void test_5() {
    std::cout << "Test 5: Only int callback" << std::endl;

    std::ofstream out("test.txt", std::ofstream::trunc);
    const std::string answer("(u:1)(u:213)(u:213)(u:567)(u:122)(u:888)");
    std::string my_result;

    My_Parser::Parser test_parser;
    test_parser.change_uint64_callback(log_uint64);
    test_parser.change_end_callback(nullptr);
    test_parser.change_start_callback(nullptr);

    test_parser.parse(s1);
    test_parser.parse(s2);
    test_parser.parse(s3);
    test_parser.parse(s4);
    test_parser.parse(s5);
    test_parser.parse(s6);
    test_parser.parse(s7);
    test_parser.parse(s8);

    std::ifstream in("test.txt");
    in >> my_result;
    assert(my_result == answer);
 
    std::cout << "Test 5: OK!" << std::endl;    
}


void test_6() {
    std::cout << "Test 6: Overflow int" << std::endl;
    
    std::ofstream out("test.txt", std::ofstream::trunc);
    const std::string s9("18446744073709551615"), s10("18446744073709551616"), s11("18446744073709551617");
    const std::string answer("(u:18446744073709551615)(s:18446744073709551616)(s:18446744073709551617)");
    std::string my_result;

    My_Parser::Parser test_parser;
    test_parser.change_uint64_callback(log_uint64);
    test_parser.change_str_callback(log_string);
    test_parser.change_end_callback(nullptr);
    test_parser.change_start_callback(nullptr);

    test_parser.parse(s9);
    test_parser.parse(s10);
    test_parser.parse(s11);

    std::ifstream in("test.txt");
    in >> my_result;
    assert(my_result == answer);
 
    std::cout << "Test 6: OK!" << std::endl;  
}


int main(void) {
    std::vector<std::function<void()>> tests{test_1, test_2, test_3, test_4, test_5, test_6};
    for (size_t i = 0; i < tests.size(); ++i) {
        tests[i]();
    }
    return 0;
}