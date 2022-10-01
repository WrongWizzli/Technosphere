#include <iostream>

void deletea(int* a) {
    a = nullptr;
}

int main(void) {
    int* a = new int[50];
    a[0] = 8;
    deletea(a);
    std::cout << a[0] << std::endl;
}