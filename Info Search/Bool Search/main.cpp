//Stable on 00:23 12.10.21
#include <sstream>
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <ios>
#include <map>
#include <vector>
#include <set>
#include <chrono>
#include <ctime>
#include "parse_read.hpp"
#include "searchtools.hpp"

using namespace My_Space;


int main(void) {

    std::vector<std::string> links;
    links.push_back("empty");
    std::map<std::string, VarByte> word_links;
    std::vector<std::string> files = {"1", "2", "3", "4", "5", "6", "7", "8"};
    std::vector<std::string> dots = {".", "..", "..."};
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < files.size(); ++i) {
        std::system("clear");
        std::cout << "Reading #" << i << " file from #" << files.size() << dots[i % dots.size()] << std::endl;

        std::ifstream rd(files[i], std::ios::in | std::ios::binary);
        parse_file(rd, links, word_links);
    }
    auto end = std::chrono::system_clock::now();

    std::system("clear");

    read_request(links, word_links);
    
    
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "elapsed time for parsing files: " << elapsed_seconds.count() << "s\n";
}