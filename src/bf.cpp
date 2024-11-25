#include "../include/define_include.h"

std::vector<bool> permute(int n, int capacity, const std::vector<item>& items, const std::vector<std::pair<int, int>>& dependencies, std::vector<int> subset){

    bool valid; int tempWeight, result = 0;
    std::vector<bool> solution(N); //globalne N
    
    do {
        valid = true; 
        for(int i = 0; i < dependencies.size(); i++) { //sprawdzic wszystkie warunki
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
                    solution[idx-1] = 1;
                }
                std::cout << std::endl;
            }
        }

    } while (std::next_permutation(subset.begin(), subset.end()));

    return solution;
}

Result bruteForce(int n, int capacity, const std::vector<item>& items, const std::vector<std::pair<int, int>>& dependencies) {
    //najlepiej zeby zwracalo wektor i liczbe

    Result result;
    int tempValue = 0;
    std::vector<int> indices;
    std::vector<bool> tempSolution(n);
    for(int i = 0; i < n; i++) {
        indices.push_back(i+1);
    }

    for (int length = 1; length <= n; ++length) {
        std::vector<bool> v(indices.size());
        std::fill(v.begin(), v.begin() + length, true);

        do {
            std::vector<int> subset;
            for (int i = 0; i < indices.size(); ++i) {
                if (v[i]) {
                    subset.push_back(indices[i]);
                }
            }
            
            // Sortujemy podzbiór, aby uzyskać permutacje w porządku leksykograficznym
            std::sort(subset.begin(), subset.end());
            
            // Generowanie permutacji dla tego podzbioru
            tempSolution = permute(length, capacity, items, dependencies, subset);
            for(int i = 0; tempSolution.size(); i++) {
                tempValue += (tempSolution[i] == 1) ? items[i].value : 0;
            }
            if(tempValue > result.value) {
                result.value = tempValue;
                result.arr = tempSolution; 
            }
        } while (std::prev_permutation(v.begin(), v.end()));
    }
    
    return result;
}

// int bruteForc(int n, int capacity, const std::vector<item>& items, const std::vector<std::pair<int, int>>& dependencies, std::vector<bool>& tempSolution) {
    
//     int include_item, exclude_item, result;
//     std::cout << "knapsack_brute_force(" << capacity << "," << n << ")" << std::endl;
//     for(int i = 0; i < 4; i++) {
//         std::cout << tempSolution[i] << " ";
//     }
//     std::cout << "\n";
//     // Base case: if no items or no capacity left, return 0
//     if (n == 0 || capacity == 0) {
//         return 0;
//     }
//     // If the current item can't be included because it exceeds the capacity or doesn't meet the dependecy requirement
//     if (items[n - 1].weight > capacity) {// || !tempSolution[dependencies[n-1].second]) {
//         std::cout << "wychodzi z rek do (" << capacity << " " << n << ")\n";
//         return bruteForce(n-1, capacity, items, dependencies, tempSolution);
//     }

//     tempSolution[n-1] = 1;
//     include_item = items[n-1].value + bruteForce(n-1, capacity - items[n - 1].weight, items, dependencies, tempSolution);
//     tempSolution[n-1] = 0;
//     exclude_item = bruteForce(n-1, capacity, items, dependencies, tempSolution);
//     if(std::max(include_item, exclude_item) == include_item) {
//         tempSolution[n-1] = 1;
//     } 
//     result = std::max(include_item, exclude_item);
//     std::cout << "wychodzi z rek do (" << capacity << " " << n << ")\n";
//     return result;
    
//     // if (result == include_item) {
//     //     std::cout << "Item " << n-1 << " (Value: " << items[n-1].value << ", Weight: " << items[n-1].weight << ") is added to the knapsack.\n";
//     // }
//     // Return the maximum value
//}


// int knapsack_brute_force(int capacity, int n) {
//     cout << "knapsack_brute_force(" << capacity << "," << n << ")" << endl;

//     // Base case: if no items or no capacity
//     if (n == 0 || capacity == 0) {
//         return 0;
//     }

//     // If the current item's weight is greater than the remaining capacity, skip it
//     if (weights[n-1] > capacity) {
//         return knapsack_brute_force(capacity, n-1);
//     }

//     // Otherwise, consider both including and excluding the current item
//     int include_item = values[n-1] + knapsack_brute_force(capacity - weights[n-1], n-1);
//     int exclude_item = knapsack_brute_force(capacity, n-1);
    
//     // Return the maximum value of including or excluding the current item
//     return max(include_item, exclude_item);
// }


// // bool isValidCombination(const vector<int>& combination, const vector<pair<int, int>>& dependencies) {
// //     // Sprawdzamy zależności: jeśli przedmiot A jest zapakowany, to przedmiot B musi być zapakowany
// //     for (const auto& dep : dependencies) {
// //         int A = dep.first;  // Przedmiot A
// //         int B = dep.second; // Przedmiot B
// //         if (combination[A] == 1 && combination[B] == 0) {
// //             return false;  // Zależność nie jest spełniona
// //         }
// //     }
// //     return true;
// // }

// // int bruteForceKnapsack(int W, const vector<Item>& items, const vector<pair<int, int>>& dependencies) {
// //     int n = items.size();
// //     int maxValue = 0;
    
// //     // Przechodzimy przez wszystkie możliwe kombinacje przedmiotów
// //     // Można zapakować lub nie zapakować każdy przedmiot, czyli mamy 2^n kombinacji
// //     for (int i = 0; i < (1 << n); ++i) {
// //         vector<int> combination(n, 0);
        
// //         // Sprawdzamy, które przedmioty są zapakowane w tej kombinacji
// //         for (int j = 0; j < n; ++j) {
// //             if (i & (1 << j)) {
// //                 combination[j] = 1; // Przedmiot j jest zapakowany
// //             }
// //         }
        
// //         // Sprawdzamy, czy suma wag zmieści się w plecaku i czy zależności są spełnione
// //         int totalWeight = 0;
// //         int totalValue = 0;
// //         if (isValidCombination(combination, dependencies)) {
// //             for (int j = 0; j < n; ++j) {
// //                 if (combination[j] == 1) {
// //                     totalWeight += items[j].weight;
// //                     totalValue += items[j].value;
// //                 }
// //             }
            
// //             // Sprawdzamy, czy waga mieści się w plecaku
// //             if (totalWeight <= W) {
// //                 maxValue = max(maxValue, totalValue); // Aktualizujemy maksymalną wartość
// //             }
// //         }
// //     }
    
// //     return maxValue;
// // }

// // int main() {
// //     int n, W;
    
// //     cout << "Podaj liczbe przedmiotow: ";
// //     cin >> n;
// //     cout << "Podaj pojemnosc plecaka: ";
// //     cin >> W;
    
// //     vector<Item> items(n);
// //     cout << "Podaj wagi i wartosci przedmiotow:" << endl;
// //     for (int i = 0; i < n; ++i) {
// //         cin >> items[i].weight >> items[i].value;
// //     }
    
// //     int m;
// //     cout << "Podaj liczbe zaleznosci: ";
// //     cin >> m;
    
// //     vector<pair<int, int>> dependencies(m);
// //     cout << "Podaj zaleznosci (A B), gdzie przedmiot A moze byc zapakowany tylko, jesli B jest zapakowany:" << endl;
// //     for (int i = 0; i < m; ++i) {
// //         cin ;
// //     }
    
// //     // Rozwiazujemy problem plecakowy brute-force z zaleznosciami
// //     int result = bruteForceKnapsack(W, items, dependencies);
    
// //     cout << "Maksymalna wartosc, jaka mozna zapakowac do plecaka: " << result << endl;
    
// //     return 0;
// // }
