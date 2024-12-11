#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stack>
#include <set>
#include <numeric>
#include <random>

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

struct graph {
    std::vector <int> next;
    int weight;
    int value;
};

struct antGraph {
    std::vector <int> n;
    double pheromoneLevel;
};

class pheromoneItemSelector {
    public:
   
    void adjust(antGraph *G, std::vector<int> verSet) { 
        if (verSet.empty()) {
            pherRange.clear();
        }  
        else {
            pherRange.resize(verSet.size()); //zeby wybieralo tylko z tych dostepnych
            pherRange[0] = G[verSet[0]].pheromoneLevel;
            for (size_t i = 1; i < verSet.size(); i++) {
                pherRange[i] = pherRange[i-1] + G[verSet[i]].pheromoneLevel;
            }
            total = pherRange.back();
            std::cout << "Range:\n";
            for (int i = 0; i < pherRange.size(); i++) {
                std::cout << pherRange[i] << " ";
            }
            std::cout << "\n";
        }
    }
    int selectItem() {
        std::random_device rd;
        std::mt19937 gen(rd());  // Mersenne Twister engine
        std::uniform_int_distribution<> dist(0, total - 1);
        double randNum = dist(gen);

        // Use binary search to find the index where randNum fits in cumulative sums
        auto it = std::lower_bound(pherRange.begin(), pherRange.end(), randNum + 1);
        return std::distance(pherRange.begin(), it);  // Index of the selected pheromon
    }

    private:
    std::vector<double> pherRange;
    double total;
};

#endif

//zrobic klasy na grafy