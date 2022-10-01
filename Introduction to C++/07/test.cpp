#include <iostream>
#include <assert.h>
#include "my_vector.hpp"
#include <string>


using namespace My_Tools;


void end_of_test(const std::string &s, int len_output = 60) {
    std::string output(len_output, '=');
    output.replace((len_output - s.size()) / 2, s.size(), s);
    std::cout << output << std::endl;
}


Vector<int> get_vec() {
    return Vector<int>(3, 3);
}


void Constructors_test() {

    Vector<int> a{1, 2, 3, 4};
    Vector<int> b = Vector<int>{1, 2, 3, 4, 5};

    assert(a.to_string() == "1,2,3,4,");
    assert(b.to_string() == "1,2,3,4,5,");

    Vector<int> c(50);
    assert(c.size() == 0);
    assert(c.empty());

    c = Vector<int>(10, 1);
    assert(!c.empty() && c.size() == 10);
    assert(c.to_string() == "1,1,1,1,1,1,1,1,1,1,");

    Vector<int> m = get_vec();
    assert(m.to_string() == "3,3,3,");
    
    a = b = b;
    assert(a.to_string() == b.to_string());
    
    end_of_test("Constructors test completed!");
}


void Get_Val_operator_test() {

    Vector<double> a{1.4, 2.5, 3.7, 88.88, 99999.99999};
    assert(a[0] == 1.4);
    assert(a[1] == 2.5);
    assert(a[2] == 3.7);
    assert(a[3] == 88.88);
    assert(a[4] == 99999.99999);

    a[3] = -33.33;
    assert(a[3] == -33.33);

    end_of_test("[] operator ok!");

}


void Push_Pop_test() {

    Vector<double> a(10);
    a.push_back(2.2);
    a.push_back(3.3);
    a.push_back(22);
    a.push_back(90.09);
    
    assert(a.size() == 4);
    assert(a.to_string() == "2.200000,3.300000,22.000000,90.090000,");
    assert(a.pop_back() == 90.09);
    assert(a.size() == 3);

    double sum = a.pop_back() + a.pop_back() + a.pop_back();
    assert(a.empty() && sum == 2.2 + 3.3 + 22);

    end_of_test("Push_back/Pop_back methods");
}


void Emplace_test() {

    Vector<std::string> p(3, std::string("Meow"));
    std::cout << "P cap: " << p.capacity() << std::endl;
    assert(p.size() == 3);

    p.emplace_back({"Qwer", "WW"});

    assert(p.size() == 5);
    assert(p.capacity() == 6);
    assert(p[0] == "Meow" && p[1] == "Meow" && p[2] == "Meow" && p[3] == "Qwer" && p[4] == "WW");

    p.emplace_back({"aaa", "bbb", "ccc"});
    std::cout << "P cap: " << p.capacity() << std::endl;

    assert(p[5] == "aaa" && p[6] == "bbb" && p[7] == "ccc");
    assert(p.size() == 8);

    end_of_test("Emplace_back ok!");
}

void Clear_test() {

    Vector<int> a{2,3,4,5,6,7,8,9,9,1,1,1,1};

    assert(!a.empty());
    a.clear();
    assert(a.empty() && a.size() == 0);

    end_of_test("clear test ok!");
}

void Resize_test() {

    Vector<int> a(100000, 100000);

    a.resize(100);
    assert(a.size() == 100);
    for (int32_t i = 0; i < a.size(); ++i) {
        if (a[i] != 100000) {
            assert(false);
        }
    }

    a = Vector<int>(20, 20);
    a.resize(2000);

    assert(a.capacity() == 2000);
    for (int32_t i = 0; i < a.size(); ++i) {
        if (a[i] != 20) {
            assert(false);
        }
    }

    end_of_test("Resize test ok!");
}


void Reserve_test() {

    Vector<int> m(100);

    m.reserve(50);
    assert(m.capacity() == 200);

    m.reserve(2000);
    assert(m.capacity() == 2000);

    end_of_test("Reserve test ok!");
}


void Iter_test() {

    Vector<int> m(10);
    for (int32_t i = 0; i < 10; ++i) {
        m[i] = i;
    }
    auto p0 = m.begin(), pn = m.end();

    int32_t j = 0;
    while(p0 != pn) {
        assert(*p0++ == j++);
    }

    end_of_test("begin/end ok!");
}


void Rev_Iter_test() {

    Vector<int> m(10);
    for (int32_t i = 0; i < 10; ++i) {
        m[i] = i;
    }
    
    auto p0 = m.rbegin(), pn = m.rend();

    int32_t j = 9;
    while(p0 != pn) {
        assert(*p0++ == j--);
    }

    end_of_test("rbegin/rend ok!");
}


int main(void) {
    Constructors_test();
    Get_Val_operator_test();
    Push_Pop_test();
    Emplace_test();
    Clear_test();
    Resize_test();
    Iter_test();
    Rev_Iter_test();
    return 0;
}