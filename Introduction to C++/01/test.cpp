#include <iostream>
#include <vector>
#include <assert.h>
#include "my_allocator.hpp"



void test_1() {
    std::cout << "Test 1: Correct allocation" << std::endl;
    std::cout << "..." << std::endl;

    size_t maxSize1 = 100;
    size_t allocSize1 = 10, allocSize2 = 50, allocSize3 = 39;
    Allocator test_object;

    test_object.MakeAllocator(maxSize1);

    char *p1 = test_object.alloc(allocSize1);
    char *p2 = test_object.alloc(allocSize2);
    char *p3 = test_object.alloc(allocSize3);
    char *p0 = test_object.get_ptr();

    assert(p0 != nullptr);
    assert(p0 == p1);
    assert(p0 + allocSize1 == p2);
    assert(p0 + allocSize1 + allocSize2 == p3);

    std::cout << "Test 1 OK!" << std::endl << std::endl;
}


void test_2() {
    std::cout << "Test 2: Allocation of wrong number of bytes" << std::endl;
    std::cout << "..." << std::endl;

    size_t maxSize1 = 100;
    size_t allocSize1 = 150, allocSize2 = 0, allocSize3 = -10, allocSize4 = 50, allocSize5 = 60;
    Allocator test_object;

    test_object.MakeAllocator(maxSize1);

    char *p1 = test_object.alloc(allocSize1);
    char *p2 = test_object.alloc(allocSize2);
    char *p3 = test_object.alloc(allocSize3);
    char *p4 = test_object.alloc(allocSize4);
    char *p5 = test_object.alloc(allocSize5);

    assert(p1 == nullptr);
    assert(p2 == nullptr);
    assert(p3 == nullptr);
    assert(p4 != nullptr);
    assert(p5 == nullptr);

    std::cout << "Test 2 OK!" << std::endl << std::endl;
}


void test_3() {
    std::cout << "Test 3: Correct limit memory allocation + reset method" << std::endl;
    std::cout << "..." << std::endl;

    size_t maxSize1 = 100;
    size_t allocSize1 = 100, allocSize2 = 1, allocSize3 = 99;
    Allocator test_object;

    test_object.MakeAllocator(maxSize1);

    char *p1 = test_object.alloc(allocSize1);

    test_object.reset();
    char *p2 = test_object.alloc(allocSize2);
    char *p3 = test_object.alloc(allocSize3);

    assert(p1 != nullptr);
    assert(p2 != nullptr);
    assert(p3 != nullptr);

    std::cout << "Test 3 OK!" << std::endl << std::endl;
}


void test_4() {
    std::cout << "Test 4: .alloc() without .MakeAllocator()" << std::endl;
    std::cout << "..." << std::endl;

    size_t allocSize1 = 15;

    Allocator test_object;

    char *p1 = test_object.alloc(allocSize1);
    test_object.reset();

    assert(p1 == nullptr);

    std::cout << "Test 4 OK!" << std::endl << std::endl;
}


void test_5() {
    std::cout << "Test 5: Multi allocation with .reset() and reaching the max capacity" << std::endl;
    std::cout << "..." << std::endl;

    size_t maxSize1 = 10;
    Allocator test_object;

    test_object.MakeAllocator(maxSize1);

    for (int i = 0; i < 20; ++i) {
        if (i % 10 == 0) {
            test_object.reset();
        }
        char *p = test_object.alloc(1);
        assert(p != nullptr);
    }

    std::cout << "Test 5 OK!" << std::endl << std::endl;
}


void test_6() {
    std::cout << "Test 6: Double .MakeAllocator()" << std::endl;
    std::cout << "..." << std::endl;

    size_t maxSize1 = 100, maxSize2 = 300;
    size_t allocSize1 = 50, allocSize2 = 60, allocSize3 = 180, allocSize4 = 10, allocSize5 = 10;
    Allocator test_object;

    test_object.MakeAllocator(maxSize1);

    char *p1 = test_object.alloc(allocSize1);
    char *p2 = test_object.alloc(allocSize2);
    char *p3 = test_object.alloc(allocSize3);

    assert(p1 != nullptr);
    assert(p2 == nullptr);
    assert(p3 == nullptr);

    test_object.MakeAllocator(maxSize2);

    p1 = test_object.alloc((allocSize1));
    p2 = test_object.alloc((allocSize2));
    p3 = test_object.alloc((allocSize3));
    char *p4 = test_object.alloc((allocSize4));
    char *p5 = test_object.alloc((allocSize5));

    assert(p1 != nullptr);
    assert(p2 != nullptr);
    assert(p3 != nullptr);
    assert(p4 != nullptr);
    assert(p5 == nullptr);

    std::cout << "Test 6 OK!" << std::endl << std::endl;
}


void test_7() { 
    std::cout << "Test 7: Correct pointer positions" << std::endl;
    std::cout << "..." << std::endl;

    size_t maxSize1 = 5;
    size_t allocSize1 = 1, allocSize2 = 2, allocSize3 = 2, allocSize4 = 3;
    Allocator test_object;

    test_object.MakeAllocator(maxSize1);

    char *p1 = test_object.alloc(allocSize1);
    char *p2 = test_object.alloc(allocSize2);
    char *p3 = test_object.alloc(allocSize3);
    char *p0 = test_object.get_ptr();

    p1[0] = 'T', p2[0] = 'e', p2[1] = 's', p3[0] = 't', p3[1] = '7';
    const char *s1 = "Test7";
    for (size_t i = 0; i < maxSize1; ++i) {
        assert(s1[i] == p0[i]);
    }

    test_object.reset();

    char *p4 = test_object.alloc((allocSize4));

    p4[0] = 't', p4[1] = 's', p4[2] = 'e';
    const char *s2 = "tset7";
    for (size_t i = 0; i < maxSize1; ++i) {
        assert(s2[i] == p0[i]);
    }

    std::cout << "Test 7 OK!" << std::endl << std::endl;
}


int main(void) {
    std::vector<void (*)()> tests{test_1, test_2, test_3, test_4, test_5, test_6, test_7};
    for (size_t i = 0; i < tests.size(); ++i) {
        tests[i]();
    }
    return 0;
}