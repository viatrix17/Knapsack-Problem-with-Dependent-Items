#include "../include/define_include.h"
#include "../include/graphsOperations.h"
#include "../include/antColonyOpt.h"

extern int N;

void cyclic(int &capacity, int &finalValue, graph *G, std::vector<std::vector<int>> prevG, const std::vector<item> items, std::set<std::set<item>> &cycles) {
    
    std::cout << "Graf jest cykliczny.\n";
    graph *GCycle;
    std::vector<bool> solution(N);
    std::vector<int> perm;
        GCycle = transformGraph(G, cycles);  //musi byc gdzies przechowana informacja o tym, jakie przedmioty są w cyklu
        std::cout << "po transformacji:\n";
        show(GCycle, N+cycles.size());
        perm = topologicalSort(N+cycles.size(), GCycle, items);
        std::vector<bool> added(N+cycles.size());
        // std::cout << "permutation:\n";
        // for(int i = 0; i < perm.size(); i++) {
        //     std::cout << perm[i] << " ";
        // }
        int pom = 0;
        std::cout << "\n";
        for (long unsigned int i = 0; i < perm.size(); i++) {
            if (perm[i] >= N) {
                //std::cout << "Cykl!\n";
               // std::cout << perm[i] << " " << GCycle[perm[i]].weight << " " << GCycle[perm[i]].value << " " << capacity << "\n";
                if (GCycle[perm[i]].weight <= capacity && allPrevsAdded(GCycle[perm[i]].next, added)) {
                    //std::cout << "ok\n";
                    capacity -= GCycle[perm[i]].weight;
                    finalValue += GCycle[perm[i]].value;
                    added[perm[i]] = 1;
                    //std::cout << finalValue << "\n";
                    pom = N;
                    for (auto it = cycles.begin(); it != cycles.end(); it++) {
                        if (pom == perm[i]) {
                            for (auto vertice = it->begin(); vertice != it->end(); vertice++) {
                                //std::cout << "dodajemy do rozwiazania: " << vertice->number << " ";
                                solution[vertice->number-1] = 1;
                            }
                            break;
                        }
                        pom++;
                    }
                }
            } 
            else {
                //std::cout << "Nie-cykl!\n";
                if (items[perm[i]].weight <= capacity && allPrevsAdded(GCycle[perm[i]].next, added)) {
                    capacity -= items[perm[i]].weight;
                    finalValue += items[perm[i]].value;
                    solution[perm[i]] = 1;
                    added[perm[i]] = 1;
                }

            }

        }
}


Result greedyDependentKnapsack(int capacity, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies) {
    
    Result result(N);
    graph *G, *GCycle;
    G = new graph[N]; 
    std::vector<std::vector<int>> prevG(N);
    std::vector<bool> solution(N);
    std::vector<int> perm;
    std::set<std::set<item>> cycles; //numer nowego wierzcholka to N+i
    createGraph(G, prevG, items, dependencies);
    std::cout << "Graph was created\n";
     
    isCyclic(G, items, cycles);
    std::cout << "Wyszukiwanie cykli zakończone\n";

    int pom = 0;
    int finalValue = 0;
    if (!cycles.empty()) {
        std::cout << "Graf jest cykliczny.\n";
        //zmienia graf tak ze wywala te wierzcholki z cykli i ich nastepnikow
        deleteCycles(G, prevG, cycles);
    }
    else { 
        std::cout << "Graf jest acykliczny\n";
    }
    perm = topologicalSort(N, G, items); 
    std::vector<bool> added(N);
    for (long unsigned int i = 0; i < perm.size(); i++) {
        if (items[perm[i]].weight <= capacity && allPrevsAdded(prevG[perm[i]], added)) {
            capacity -= items[perm[i]].weight;
            finalValue += items[perm[i]].value;
            solution[perm[i]] = 1;
            added[perm[i]] = 1;
        }
    }
    

    //std::cout << "Greedy Algorithm Done.\n" << finalValue << "\n";
    
    result.arr = solution; 
    result.value = finalValue;
    return result;
}
