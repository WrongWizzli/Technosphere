#include <iostream>
#include <fstream>
#include <assert.h>
#include "my_format.hpp"
#include <sstream>
#include <string>
#include <limits>


using namespace My_Tools;


void end_of_test(const std::string &s, int len_output = 60) {
    std::string output(len_output, '=');
    output.replace((len_output - s.size()) / 2, s.size(), s);
    std::cout << output << std::endl;
}


void check_empty_format() {

    auto test = format("qwer", 3, 4, "aaa");
    assert(test == "qwer");

    test = format("", 3, "ksls", 2);
    assert(test == "");

    test = format("");
    assert(test == "");

    test = format("qer");
    assert(test == "qer");

    test = format("5647382901", "kld", "", 3223);
    assert(test == "5647382901");

    end_of_test("String formatting without insertions successful");
}

void check_correct_format() {

    auto test = format("qq{0}{1}{2}", "ald", 5, 9);
    assert(test == "qqald59");

    test = format("q+{0}+{1}+{0}q{2}{2}{1}", "one", "two", "3");
    assert(test == "q+one+two+oneq33two");

    test = format("q\n{1}{0}\t{1}\r{0}", "zero", "one");
    assert(test == "q\nonezero\tone\rzero");

    test = format("{0}{1}", 3, 4);
    assert(test == "34");

    test = format("{2}", 1, 2, "example");
    assert(test == "example");

    test = format("{0}", 1);
    assert(test == "1");

    end_of_test("Correct insertions successful");
}

void check_incorrect_index_format() {
    std::string test;

    try {
        test = format("{{}}", "11");
        assert(false);
    } catch(const FormatWrongIndex &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: '{{}}'");
    }
    try {
        test = format("q{q}{0}{23}{-1}", 3);
        assert(false);
    } catch(const std::exception &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: 'q{q}{0}{23}{-1}'");
    }

    try {
        test = format("q{q}{0}{0}{0}");
        assert(false);
    } catch(const std::exception &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: 'q{q}{0}{0}{0}'");
    }

    try {
        test = format("{akjdsk}", 123, 18, 'a');
        assert(false);
    } catch(const std::exception &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: '{akjdsk}'");
    }

    end_of_test("Incorrect index insertions successful");
}

void check_incorrect_bracket_sequences() {
    std::string test;

    try {
        test = format("{{q}{1}}{{}}", 5, 6, 7, 8);
        assert(false);
    } catch(const std::exception &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: '{{q}{1}}{{}}'");
    }

    try {
        test = format("r{0}{0}{3}{22}{12}{0}}}}", "qw", 1, 2);
        assert(false);
    } catch(const std::exception &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: 'r{0}{0}{3}{22}{12}{0}}}}'");
    }

    try {
        test = format("{0}{", 3);
        assert(false);
    } catch(const std::exception &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: '{0}{'");
    }

    try {
        test = format("}", 123, 18, 'a');
        assert(false);
    } catch(const std::exception &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: '}'");
    }

    try {
        test = format("}}}}{0}}}}}", 123);
        assert(false);
    } catch(const std::exception &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: '}}}}{0}}}}}'");
    }

    end_of_test("Incorrect bracket sequences successful");
}

void check_complex_wrong_format() {
    std::string test;

    try {
        test = format("{1}{9}{2}{3}{4}{5}{{7}}kdok{0}{5{-1}}{8}ui{0}", 0, 1, 2, "#", "$", 5, 6, 7, "qqq", "99", "\n");
        assert(false);
    } catch(const std::exception &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: '{1}{9}{2}{3}{4}{5}{{7}}kdok{0}{5{-1}}{8}ui{0}'");
    }

    try {
        test = format("qq{1}{18446744073709551616}", 1, 2, 3, 4, 5, 6, 7, 8);
        assert(false);
    } catch(const FormatWrongIndex &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: 'qq{1}{18446744073709551616}'");
    }
    try {
        test = format("{4294967296}{0}", 0, 1, 2, 3, 4, 5, 6);
        assert(false);
    } catch(const FormatWrongIndex &m) {
        assert(std::string(m.what()) == "Index is not valid inside brackets or brackets are not paired for input string: '{4294967296}{0}'");
    }
    
    end_of_test("Complex wrong formatting successful");
}

void check_complex_correct_format() {

    auto test = format("{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}{0}{6}", 0, 1, 2, 3, 4, 5, 6);
    assert(test == "060606060606060606060606060606060606");

    test = format("{1}{9}{2}{3}{4}{5}{7}kdok{0}{5}{8}ui{0}", 0, 1, 2, "#", "$", 5, 6, 7, "qqq", "99", "\n");
    assert(test == "1992#$57kdok05qqqui0");

    end_of_test("Complex correct formatting successful");
}

int main(void) {
    check_empty_format();
    check_correct_format();
    check_incorrect_bracket_sequences();
    check_incorrect_index_format();
    check_complex_wrong_format();
    check_complex_correct_format();
    return 0;
}