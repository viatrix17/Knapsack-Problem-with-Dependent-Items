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
#include <chrono>

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
    double attractiveness;
};

class pheromoneItemSelector {
    public:
    void start() {
        
    }
    void adjust(antGraph *G, std::vector<int> verSet, int alfa, int beta, double evaporationRate) { 
        if (verSet.empty()) {
            pherRange.clear();
        }  
        else {
            pherRange.resize(verSet.size()); //zeby wybieralo tylko z tych dostepnych
            //verSet to neighbourhood 
            //std::cout << pherRange.size() << "\n";  
            sum = calculateSum(G, verSet, alfa, beta);
            pherRange[0] = (pow(G[verSet[0]].pheromoneLevel, alfa)*pow(G[verSet[0]].attractiveness, beta))/sum; 
            for (size_t i = 1; i < verSet.size(); i++) {
                //std::cout << (pow(G[verSet[i]].pheromoneLevel, alfa)*pow(G[verSet[i]].attractiveness, beta))/sum << "\n";
                pherRange[i] = pherRange[i-1] + (pow(G[verSet[i]].pheromoneLevel, alfa)*pow(G[verSet[i]].attractiveness, beta))/sum;
                total = pherRange.back();
            }
            // std::cout << "Range:\n";
            //     for (int i = 0; i < pherRange.size(); i++) {
            //         std::cout << pherRange[i] << " ";
            //     }
            //     std::cout << "\ntotal = " << total << "\n";
        }
    }
    int selectItem() {
        if(pherRange.size() > 1) {
            //unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
        //std::random_device rd;
        //static std::mt19937 gen(rd());  // Mersenne Twister engine
        //static std::uniform_int_distribution<> dist(0, total-1);
        //double randNum = dist(gen);
        //std::cout << pherRange.front() << " " << pherRange.back() << "\n";
        double randNum = randomFloat(0, pherRange.back());
        //std::cout << "randNum " << randNum << "\n";
        // Use binary search to find the index where randNum fits in cumulative sums
        auto it = std::lower_bound(pherRange.begin(), pherRange.end(), randNum);
        return std::distance(pherRange.begin(), it);  // Index of the selected item
        }
        else {
            int x = 0;
            return x;
        }
    }

    private:
    std::vector<double> pherRange;
    double total;
    double sum, randNum;

    double calculateSum(antGraph *G, std::vector<int> verSet, int alfa, int beta) {
        double result = 0;
        for (int i = 0; i < verSet.size(); i++) {
            //std::cout << G[verSet[i]].pheromoneLevel << " " << G[verSet[i]].attractiveness << "\n";
            result += pow(G[verSet[i]].pheromoneLevel, alfa)*pow(G[verSet[i]].attractiveness, beta);
        }
        return result;
    }

    float randomFloat(float min, float max)
    {
        return ((float)(rand()) / (float)(RAND_MAX)) * (max-min) + min;
    }
};

#endif

//zrobic klasy na grafy