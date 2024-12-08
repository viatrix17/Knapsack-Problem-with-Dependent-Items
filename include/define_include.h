#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stack>
#include <set>
#include <numeric>

#ifndef DEFINE_INCLUDE
#define DEFINE_INCLUDE

struct item {
    int number;
    int weight;
    int value;

    bool operator<(const item& other) const {
        if (number == other.number) {
            if (weight == other.weight) {
                return value < other.value;
            }
            return weight < other.weight;
        }
        return number < other.number;
    }
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