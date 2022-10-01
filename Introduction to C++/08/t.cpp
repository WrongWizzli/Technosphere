#include <iostream>
#include <thread>
#include <future>
#include <unistd.h>
#include <vector>
#include <mutex>
std::mutex a;
int output_random_num(int i) {
    std::lock_guard<std::mutex> lock(a);
    std::cout << i << std::endl;
    return i;
}



int main(void) {
    std::vector<std::future<void>> threads(30);
    for (int i = 0; i < 50; ++i) {
        //threads[i] = std
    }

    for (int i = 0; i < 50; ++i) {
        //a[i].join();
    }
}