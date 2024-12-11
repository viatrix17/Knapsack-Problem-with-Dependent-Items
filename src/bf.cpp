#include "../include/define_include.h"

extern int N;

std::vector<bool> checkPermutation(int n, int capacity, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies, std::vector<int> subset, bool & check) {

    bool valid;
    check = false;
    int tempWeight;
    std::vector<bool> solution(N);
    for(long unsigned int i = 0; i < dependencies.size(); i++) { //sprawdzic wszystkie warunki
        auto pos1 = std::find(subset.begin(), subset.end(), dependencies[i].second-1); 
        auto pos2 = std::find(subset.begin(), subset.end(), dependencies[i].first-1);  //3 nie moze byc przed 1 i 4 przed 2
        if (pos1 > pos2 && pos1 != subset.end() && pos2 != subset.end()) {                
            valid = false;
            break;
        }
    }
    if (valid) {
        tempWeight = 0;
        for(int i = 0; i < n; i++) {
            tempWeight += items[subset[i]-1].weight;
        }
                
        if (tempWeight <= capacity) { 
            check = true;
            //std::cout << "Valid permutation: ";
            for (int idx : subset) {
                //std::cout << idx-1 << " ";
                solution[idx-1] = 1;
            }
            //std::cout << "\n";
        }
    }
    
    return solution;
}

Result bruteForce(int capacity, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies) {
    
    Result result(N);
    result.value = 0;
    bool check;
    int tempValue;
    std::vector<int> indices;
    std::vector<bool> tempSolution(N);
    for(int i = 0; i < N; i++) {
        indices.push_back(i);
    }
    for (int length = 1; length <= N; ++length) {
        std::vector<bool> v(indices.size());
        std::fill(v.begin(), v.begin() + length, true);

        do {
            std::vector<int> subset;
            for (long unsigned int i = 0; i < indices.size(); ++i) {
                if (v[i]) {
                    subset.push_back(indices[i]);
                }
            }
            
            // Sortujemy podzbiór, aby uzyskać permutacje w porządku leksykograficznym
            std::sort(subset.begin(), subset.end());
            
            // Generowanie permutacji dla tego podzbioru
            do {
                tempValue = 0;
                tempSolution = checkPermutation(length, capacity, items, dependencies, subset, check);
                if (check) {
                    for (long unsigned int i = 0; i < tempSolution.size(); i++) {
                        if (tempSolution[i] == 1) {
                            tempValue += items[i].value;
                        }
                    }
                    if (tempValue > result.value) {
                        result.value = tempValue;
                        result.arr = tempSolution; 
                    }
                }
            } while (std::next_permutation(subset.begin(), subset.end()));

            
        } while (std::prev_permutation(v.begin(), v.end()));
    }

    return result;
}