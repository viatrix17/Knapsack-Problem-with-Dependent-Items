#include "../include/define_include.h"

extern int N;

std::vector<bool> checkPermutation(int n, int capacity, const std::vector<item>& items, const std::vector<std::pair<int, int>>& dependencies, std::vector<int> subset) {

    bool valid; int tempWeight;
    std::vector<bool> solution(N);
    valid = true; 
    for(long unsigned int i = 0; i < dependencies.size(); i++) { //sprawdzic wszystkie warunki
            //std::cout << dependencies[i].first+1 << dependencies[i].second+1 << "\n";
        auto pos1 = std::find(subset.begin(), subset.end(), dependencies[i].second+1); 
        auto pos2 = std::find(subset.begin(), subset.end(), dependencies[i].first+1);  //3 nie moze byc przed 1 i 4 przed 2
        if (pos1 > pos2 && n != 1 && pos1 != subset.end() && pos2 != subset.end()) {                
            valid = false;
        }
    }
    if(valid) {
        tempWeight = 0;
        for(int i = 0; i < n; i++) {
            tempWeight += items[subset[i]-1].weight;
        }
                
        if (tempWeight <= capacity) { 
            std::cout << "Valid permutation: ";
            for (int idx : subset) {
                std::cout << idx << " ";
                solution[idx-1] = 1;
            }
            std::cout << "\n";
        }
    }
    return solution;
}

void bruteForce(int capacity, const std::vector<item>& items, const std::vector<std::pair<int, int>>& dependencies) {
    //najlepiej zeby zwracalo wektor i liczbe
    std::cout << "N = " << N <<"\n";
    Result result(N);
    int tempValue = 0;
    std::vector<int> indices;
    std::vector<bool> tempSolution(N);
    for(int i = 0; i < N; i++) {
        indices.push_back(i+1);
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
                tempSolution = checkPermutation(length, capacity, items, dependencies, subset);
            } while (std::next_permutation(subset.begin(), subset.end()));

            // for(long unsigned int i = 0; tempSolution.size(); i++) {
            //     tempValue += (tempSolution[i] == 1) ? items[i].value : 0;
            // }
            // if(tempValue > result.value) {
            //     result.value = tempValue;
            //     result.arr = tempSolution; 
            // }
        } while (std::prev_permutation(v.begin(), v.end()));
    }
    
    
}


// Result bruteForce(int capacity, const std::vector<item>& items, const std::vector<std::pair<int, int>>& dependencies) {
//     //najlepiej zeby zwracalo wektor i liczbe
//     Result result(N);
//     int tempValue = 0;
//     std::vector<int> indices;
//     std::vector<bool> tempSolution(N);
//     for(int i = 0; i < N; i++) {
//         indices.push_back(i+1);
//     }
//     for (int length = 1; length <= N; ++length) {
//         std::vector<bool> v(indices.size());
//         std::fill(v.begin(), v.begin() + length, true);

//         do {
//             std::vector<int> subset;
//             for (long unsigned int i = 0; i < indices.size(); ++i) {
//                 if (v[i]) {
//                     subset.push_back(indices[i]);
//                 }
//             }
            
//             // Sortujemy podzbiór, aby uzyskać permutacje w porządku leksykograficznym
//             std::sort(subset.begin(), subset.end());
            
//             // Generowanie permutacji dla tego podzbioru
//             //tempSolution = 
//             permute(length, capacity, items, dependencies, subset);
//             for(int i = 0; tempSolution.size(); i++) {
//                 tempValue += (tempSolution[i] == 1) ? items[i].value : 0;
//             }
//             if(tempValue > result.value) {
//                 result.value = tempValue;
//                 result.arr = tempSolution; 
//             }
//         } while (std::prev_permutation(v.begin(), v.end()));
//     }
    
//     return result;
// }
