#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#ifndef DEFINE_INCLUDE
#define DEFINE_INCLUDE

struct item{
    int weight;
    int value;
};

struct Result{
    int value;
    std::vector<bool> arr;

    Result(size_t N) {
        arr.resize(N, false);  // Resize the vector to N elements
    }
};

#endif