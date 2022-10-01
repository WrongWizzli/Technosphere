#include <iostream>
#include <thread>
#include <future>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <fstream>
std::mutex mtx;

void func1() {
    mtx.lock();
    std::cout << "Dog\n";
    mtx.unlock();
}

void func2(std::ifstream &f) {
    mtx.lock();
    std::cout << " Cat\n";
    mtx.unlock();
}

int main(void) {
    std::ifstream f("result.txt");
    std::thread t1(func1);
    std::thread t2(func2, std::ref(f));
    t1.join();
    t2.join();
}