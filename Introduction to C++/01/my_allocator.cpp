#include "my_allocator.hpp"


void Allocator::MakeAllocator(size_t maxSize) {
    if (allocated_memory != nullptr) {
        delete[] allocated_memory;
    }
    allocated_memory = new char[maxSize];
    offset = 0;
    capacity = maxSize;
}


char* Allocator::alloc(size_t size) {
    if (size <= 0) {
        return nullptr;
    }

    if (allocated_memory != nullptr && size + offset <= capacity) {
        offset += size;
        return allocated_memory + offset - size;
    }
    return nullptr;
}


void Allocator::reset() {
    offset = 0;
}


Allocator::~Allocator() {
    if (allocated_memory != nullptr) {
        delete[] allocated_memory;
    }
}


char* Allocator::get_ptr() {
    return allocated_memory;
}