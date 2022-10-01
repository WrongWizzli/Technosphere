#include <iostream>
#include <assert.h>
#include "my_pool.hpp"
#include <string>
#include <unistd.h>


using namespace My_Tools;


void end_of_test(const std::string &s, int len_output = 60) {
    std::string output(len_output, '=');
    output.replace((len_output - s.size()) / 2, s.size(), s);
    std::cout << output << std::endl;
}


int lazy_function(int i) {
    usleep(10000);
    std::cout << i << " ";
    return i + 1;
}

int ret_int(int a) {
    return a + a;
}

double ret_double(double a) {
    return a * a;
}

void always_enough_threads() {
    ThreadPool m(10);
    for (int i = 0; i < 5; ++i) {
        m.exec(lazy_function, i);
    }
    end_of_test("Enough Threads successful");
}


void overloaded_threadpool() {
    ThreadPool m(10);
    for (int i = 0; i < 50; ++i) {
        m.exec(lazy_function, i);
    }
    end_of_test("Overloaded Thread Pool successful");
}


void call_after_end_error() {
    ThreadPool m(10);
    for (int i = 0; i < 10; ++i) {
        m.exec(lazy_function, i);
    }
    sleep(1);
    std::cout << std::endl;
    m.close_pool();
    try {
        m.exec(lazy_function, 1);
        assert(false);
    } catch(const EndOfThreadSession_Error &a) {
        assert(a.what() == std::string("You can't ask for more execs of threadpool. End status is active!"));
    }

    try {
        m.exec(lazy_function, 4);
        assert(false);
    } catch(const EndOfThreadSession_Error &a) {
        assert(a.what() == std::string("You can't ask for more execs of threadpool. End status is active!"));
    }

    end_of_test("EnfOfThreadSession exception successful");
}


void correct_returns() {
    ThreadPool m(10);
    auto x1 = m.exec(ret_int, 8);
    auto x2 = m.exec(ret_double, 3.4);

    sleep(1);
    assert(x1.get() == 16);
    assert(x2.get() == 3.4 * 3.4);

    end_of_test("Return value successful");
}


int main(void) {
    always_enough_threads();
    call_after_end_error();
    correct_returns();
    overloaded_threadpool();
    return 0;
}