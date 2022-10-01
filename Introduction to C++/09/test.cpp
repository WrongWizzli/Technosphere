#include <iostream>
#include <assert.h>
#include "my_thrsort.hpp"
#include <string>
#include <unistd.h>


using namespace My_Tools;


void end_of_test(const std::string &s, int len_output = 60) {
    std::string output(len_output, '=');
    output.replace((len_output - s.size()) / 2, s.size(), s);
    std::cout << output << std::endl;
}


void sorted_array_test() {
    std::ofstream out("test.txt", std::ios_base::trunc);
    for (int i = 0; i < 133; ++i) {
        out << i << " ";
    }
    out.close();

    external_sort("test.txt");
    
    std::ifstream in("sorted_array.txt");
    uint64_t m;

    for (int i = 0; i < 133; ++i) {
        in >> m;
        assert(m == i);
    }

    end_of_test("Presorted array ok!");
}


void reverse_sorted_array_test() {
    std::ofstream out("test.txt", std::ios_base::trunc);
    for (int i = 133; i >= 0; --i) {
        out << i << " ";
    }
    out.close();

    external_sort("test.txt");
    
    std::ifstream in("sorted_array.txt");
    uint64_t m;

    for (int i = 0; i <= 133; ++i) {
        in >> m;
        assert(m == i);
    }

    end_of_test("reverse array ok!");
}


void random_array_test() {
    std::vector<uint64_t> a;
    std::ofstream out("test.txt", std::ios_base::trunc);
    for (int i = 133; i >= 0; --i) {
        uint64_t tmp = rand() % 1000;
        out << tmp << " ";
        a.push_back(tmp);
    }
    out.close();

    external_sort("test.txt");
    
    std::ifstream in("sorted_array.txt");
    uint64_t m;
    std::sort(a.begin(), a.end());

    for (int i = 0; i < a.size(); ++i) {
        in >> m;
        assert(a[i] == m);
    }

    end_of_test("random array ok!");
}


int main(void) {                    //Максимально занимаемая память при наших условиях 4 Mb (в сортировке создается 2 массива по N_MAX Мб)
    sorted_array_test();
    reverse_sorted_array_test();
    random_array_test();
    return 0;
}