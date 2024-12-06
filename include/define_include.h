#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stack>
#include <set>

#ifndef DEFINE_INCLUDE
#define DEFINE_INCLUDE

struct item {
    int number;
    int weight;
    int value;
};

struct Result {
    int value;
    std::vector<bool> arr;

    Result(size_t N) {
        arr.resize(N, false);  // Resize the vector to N elements
    }
};

struct graph{
    std::vector <int> next;
    int weight;
    int value;
};

#endif