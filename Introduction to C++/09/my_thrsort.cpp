#include "my_thrsort.hpp"


constexpr uint64_t MAX_SEG_SIZE = 10;       //Для тестов это значение маленькое. Для больших файлов должно стоять большое значение.
std::mutex read_mutex, write_mutex;
std::atomic <int64_t> parts_counter;
bool eof_reached = false;


void read_data(std::ifstream &f) {
    while (!eof_reached) {
        std::vector<uint64_t> a;
        uint64_t x;

        for (int32_t i = 0; i < MAX_SEG_SIZE; ++i) {
            if (f >> x) {
                a.push_back(x);
            } else if (!i) {
                eof_reached = true;
                break;
            } else {
                break;
            }
        }
        if (!eof_reached) {
            parts_counter++;
        }
        if (!eof_reached) { 
            std::sort(a.begin(), a.end());

            write_mutex.lock();

            std::ofstream out("result.txt", std::ios_base::trunc);
            for (int i = 0; i < a.size(); ++i) {
                out << a[i] << " ";
            }
            out.close();

            read_mutex.unlock();
        }
    }
}


void merge_files(bool &file_no) {
    std::ifstream npart;
    std::ofstream out;
    std::ifstream collected_part;
    std::vector<uint64_t> a;
    uint64_t m1;

    read_mutex.lock();

    parts_counter--;
    if (file_no) {
        npart.open("part_sorted1.txt");
        out.open("part_sorted2.txt", std::ios_base::trunc);
    } else {
        npart.open("part_sorted2.txt");
        out.open("part_sorted1.txt", std::ios_base::trunc);
    }
    collected_part.open("result.txt");
    while (collected_part >> m1) {
        a.push_back(m1);
    }
    collected_part.close();

    write_mutex.unlock();

    int32_t ind = 0;
    while (npart >> m1) {
        if (ind < a.size()) {
            while (ind < a.size() && a[ind] < m1) {
                out << a[ind++] << " ";
            }
            out << m1 << " ";
        } else {
            out << m1 << " ";
        }
    }
    for (int32_t i = ind; i < a.size(); ++i) {
        out << a[i] << " ";
    }
    file_no = !file_no;

    out.close();
    npart.close();
}


void slow_merge() {
    bool file_no = false;

    while(!eof_reached) {
        merge_files(file_no);
    }
    if (parts_counter != 0) {
        merge_files(file_no);
    }

    std::ofstream sorted_array("sorted_array.txt", std::ios_base::trunc);
    std::ifstream final_array;
    uint64_t m1;

    if (!file_no) {
        final_array.open("part_sorted2.txt");
    } else {
        final_array.open("part_sorted1.txt");
    }
    while (final_array >> m1) {
        sorted_array << m1 << " ";
    }
}


void My_Tools::external_sort(const std::string &filename) {
    std::ifstream f;
    f.open(filename);

    std::ofstream tmp("part_sorted1.txt", std::ios_base::trunc);
    std::ofstream tmp2("part_sorted2.txt", std::ios_base::trunc);
    std::ofstream tmp3("result.txt", std::ios_base::trunc);
    
    tmp.close(), tmp2.close(), tmp3.close();

    parts_counter = 0;

    read_mutex.lock();
    
    std::thread t1(slow_merge);
    std::thread t2(read_data, std::ref(f));

    t2.join();
    t1.join();

    read_mutex.unlock();
    write_mutex.unlock();
    eof_reached = false;
}