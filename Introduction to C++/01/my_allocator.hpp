#ifndef MY_ALLOCATOR_HPP
#define MY_ALLOCATOR_HPP

#include <iostream>

class Allocator {

    private:
    char *allocated_memory = nullptr;
    size_t offset = 0;
    size_t capacity = 0;

    public:
    void MakeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
    ~Allocator();
    char* get_ptr();
};

#endif  //MY_ALLOCATOR_HPP
