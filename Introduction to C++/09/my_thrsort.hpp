#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <fstream>
#include <functional>
#include <string>
#include <future>
#include <ios>

namespace My_Tools {
    void external_sort(const std::string &filename);
}