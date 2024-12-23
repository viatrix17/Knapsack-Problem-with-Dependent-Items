#include "../include/define_include.h"
#include "../include/graphsOperations.h"

extern int B;
extern int N; //przechod po grafie wszerz, jak sie mie miesci, to nie idz dalej, tylko trzeba wygenerowca wszyskie mozliwe bfsy a nie w porzadku leksykograficznym
//mozna zrobic odcinanie juz na poziomie generacji,ze jak w ktoryms momencie przekracza wage 

std::vector<bool> checkPermutation(int n, int capacity, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies, std::vector<int> subset, bool & check) {

    bool valid;
    check = false;
    int tempWeight;
    std::vector<bool> solution(N);
    for(long unsigned int i = 0; i < dependencies.size(); i++) { //sprawdzic wszystkie warunki
        auto pos1 = std::find(subset.begin(), subset.end(), dependencies[i].second-1); //second - to co przed; first - to co po
        auto pos2 = std::find(subset.begin(), subset.end(), dependencies[i].first-1);  //3 nie moze byc przed 1 i 4 przed 2 
        std::cout << "positions: " << *pos1 << " " << *pos2 << "\n";
        if (pos1 > pos2 && pos1 != subset.end() && pos2 != subset.end()) {    //jezeli jest na odwrot i oba są            
            valid = false;
            break;
        }
        if (pos1 == subset.end() && pos2 != subset.end()) { //jezeli pierwszego w ogole nie ma w permutacji, to drugie nie moze byc
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
            std::cout << "Valid permutation: ";
            for (int idx : subset) {
                std::cout << idx-1 << " ";
                solution[idx-1] = 1;
            }
            std::cout << "\n";
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
        std::cout << indices[i] << " ";
    }
    std::cout << "\n";
    for (int length = 1; length <= N; ++length) {
        std::vector<bool> v(indices.size());
        std::fill(v.begin(), v.begin() + length, true);

        do {
            std::vector<int> subset;
            std::cout << "subset:\n";
            for (long unsigned int i = 0; i < indices.size(); ++i) {
                if (v[i]) {
                    subset.push_back(indices[i]);
                    std::cout << subset[i] << " ";
                }
                std::cout << "\n";
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

void tryAddingItem(int currentItem, int capacity, int profit, int &maxProfit, std::vector<std::vector<int>> prevG, std::vector<bool> visited, std::vector<bool> added, const std::vector<item> items, Result &result) {
    //std::cout << "Try Adding " << currentItem << "\t" << items[currentItem].weight << "\n" << "current capacity: " << capacity << "\n";
    //wchodzi do rekurencyji i jest odwiedzony, wychodzi to tak jak gdybysmy go nie wlozyli
    // for (int i = 0; i < N; i++) {
    //     if (added[i]) {
    //         std::cout << i+1 << " ";
    //     }
    // }
    if (items[currentItem].weight <= capacity && allPrevsAdded(prevG[currentItem], added) && !visited[currentItem]) {
        visited[currentItem] = 1;
        //jesli wejdzie, to jest dodany
        added[currentItem] = 1;
        if (profit + items[currentItem].value > maxProfit) {
            maxProfit = profit + items[currentItem].value;
            result.arr = added;
        }
        for (int i = 0; i < N; i++) {
            tryAddingItem(i, capacity - items[currentItem].weight, profit + items[currentItem].value, maxProfit, prevG, visited, added, items, result);
        }
    }
    visited[currentItem] = 0;
}

Result bfCutsOff(const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies, std::chrono::time_point<std::chrono::high_resolution_clock> &startTime, std::chrono::time_point<std::chrono::high_resolution_clock> &stopTime) {
    
    Result result(N);
    std::vector<std::vector<int>> prevG(N);
    
    graph *G; 
    G = new graph[N];
    createGraph(G, prevG, items, dependencies);

    // for(int i = 0; i < N;i++) {
    //     std::cout << i << ": ";
    //     for(int j =0; j < prevG[i].size(); j++) {
    //         std::cout << prevG[i][j] << " " ;
    //     }
    //     std::cout << "\n";
    // }
    std::vector<bool> visited(N);
    std::vector<bool> added(N);

    int capacity, profit, maxProfit = 0;
    startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        capacity = B;
        profit = 0;
        for (int j = 0; j < N; j++) {
            visited[j] = 0;
        }
        tryAddingItem(i, capacity, profit, maxProfit, prevG, visited, added, items, result);
        // std::cout << "ok\n";
        // for(int j = 0; j < N; j++) {
        //     if(result.arr[j]) {
        //         std::cout << j+1 << " ";
        //     }
        // }
        // std::cout << "\n";
    }
    result.value = maxProfit;
    delete[] G;
    stopTime = std::chrono::high_resolution_clock::now();
    //rekurencyjnie robic permutacje i sprawdzac czy cos wejdzie, jak nie miesci sie w plecaku to koniec sciezki
    return result;
}

