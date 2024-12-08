#include "../include/define_include.h"
#include "../include/graphsOperations.h"

extern int N;

Result greedyDependentKnapsack(int capacity, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies, int type) {
    
    std::cout << "Weszło - początek.\n";

    Result result(N);
    graph *G, *GCycle;
    G = new graph[N]; 
    std::vector<bool> solution(N);
    std::vector<int> perm;
    std::set<std::set<item>> cycles; //numer nowego wierzcholka to N+i
    createGraph(G, items, dependencies);
    std::cout << "Graph was created\n";
    for (int i = 0; i < N; i++) {
        std::cout << i << ": ";
        for (int j = 0; j < G[i].next.size(); j++) {
            std::cout << G[i].next[j] << " ";
        }
        std::cout << "\n";
    }
    //sprawdzic czy zmieni sie kolejnosc items po tym; to mnie nie interesuje bo sortuję graf ale i tak lepiej srpawdzic 
    isCyclic(G, items, cycles);
    std::cout << "Wyszukiwanie cykli zakończone\n";

    int pom = 0;
    int finalValue = 0;
    if (!cycles.empty()) { //jeśli ma cykl to traktuj cykl jako jeden przedmiot, bo nie da sie go rozdzielic
        std::cout << "Graf jest cykliczny.\n";
        GCycle = transformGraph(G, cycles);  //musi byc gdzies przechowana informacja o tym, jakie przedmioty są w cyklu
        std::cout << "po transformacji:\n";
        show(GCycle, N+cycles.size());
        perm = topologicalSort(N+cycles.size(), GCycle, items);
        for (long unsigned int i = 0; i < perm.size(); i++) {
            if (perm[i] >= N) {
                std::cout << "Cykl!\n";
                if (GCycle[perm[i]-1].weight <= capacity) {
                    capacity -= GCycle[perm[i]-1].weight;
                    finalValue += GCycle[perm[i]-1].value;
                    pom = 0;
                    for (auto it = cycles.begin(); it != cycles.end(); it++) {
                        if (pom == perm[i]-1) {
                            for (auto vertice = it->begin(); vertice != it->end(); vertice++) {
                                solution[vertice->number-1] = 1;
                            }
                            break;
                        }
                        pom++;
                    }
                }
            } 
            else {
                std::cout << "Nie-cykl!\n";
                if (items[perm[i]-1].weight <= capacity) {
                    capacity -= items[perm[i]-1].weight;
                    finalValue += items[perm[i]-1].value;
                    solution[items[perm[i]-1].number-1] = 1;
                }

            }

        }
    }
    else { 
        std::cout << "Graf jest acykliczny\n";
        perm = topologicalSort(N, G, items); 
        //std::cout << perm.size() << "\n";
        for (long unsigned int i = 0; i < perm.size(); i++) {
            std::cout << items[perm[i]-1].number << " " << items[perm[i]-1].weight << " " << items[perm[i]-1].value << "\n";
            if (items[perm[i]-1].weight <= capacity) {
                //std::cout << "wchodzi"
                capacity -= items[perm[i]-1].weight;
                finalValue += items[perm[i]-1].value;
                solution[items[perm[i]-1].number-1] = 1;
            }
        }
    }

    std::cout << "Greedy Algorithm Done\n";

    //usuwanie Listy
    //delete G; 
    //delete GCycle;
    result.arr = solution; 
    result.value = finalValue;
    return result;
}
