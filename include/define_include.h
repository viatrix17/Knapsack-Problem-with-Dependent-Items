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
};

#endif