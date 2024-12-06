#include "../include/define_include.h"

#include <chrono>
#include <thread>

extern int N; //liczba wierzchołków

void createGraph(graph *L, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies) {

    for (long unsigned int i = 0; i < items.size(); i++) {
        L[i].weight = items[i].weight;
        L[i].value = items[i].value;
    }
    for (long unsigned int i = 0; i < dependencies.size(); i++) {
        //std::cout << dependencies[i].first-1 << " " << dependencies[i].second-1 << "\n";
        L[items[dependencies[i].second-1].number-1].next.push_back(items[dependencies[i].first-1].number-1);
    }
}

//DFS do cykli
bool dfsFindCycle(graph *L, int V, std::stack<int> S, bool *visited)
{
    visited[V] = true;

    S.push(V); //biezacy wierzcholek

    for (std::vector<int>::iterator it = L[V].next.begin(); it != L[V].next.end(); it++)
    for (long unsigned int i = 0; i < L[V].next.size(); i++) {
        
            if (i == V) {
                return true;
            }
            if (!visited[i] && dfsFindCycle(L, i, S, visited)) {
                return true;
            }
    }
    S.pop();
    return false;
}

void isCyclic(graph *L, std::set<std::set<item>> &cycles) {

    std::stack <int> S, T;
   
    bool *visited;
    //informacje o cyklach
    visited = new bool[N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visited[j] = 0;
        }
        if (!dfsFindCycle(L,i,S,visited) && i == N-1) {
            
        }
        else { 
            T.push(i);
            while (!S.empty()) {
                T.push(S.top());
                S.pop();
            }
            while (!T.empty()) {
                
                T.pop();
            }
        }
        
    }
    for (int j = 0; j < N; j++) {
        visited[j] = 0;
    }
    //łączenie cykli
    delete[] visited;
}

graph *transformGraph(graph *oldG, std::set<std::set<item>> &cycles) {
    graph *newG;
    newG = new graph[2*N];
    bool *visited; 
    visited = new bool[N];
    int sumWeight, sumValue, prev, a = 0;
    
    for (std::set<std::set<item>>::iterator it = cycles.begin(); it != cycles.end(); it++) {
        sumWeight = 0;
        sumValue = 0;
        for (std::set<item>::iterator vertice = it->begin(); vertice != it->end(); vertice++) {
            sumWeight += vertice->weight;
            sumValue += vertice->value;
            for (int i = 0; i < N; i++) {
                for (long unsigned int j = 0; j < oldG[i].next.size(); j++) {
                    if (oldG[i].next[j] == vertice->number) { //szukanie poprzedników
                        newG[N+a+1].next.push_back(prev);
                        visited[N+a+1] = 1;
                    }
                }
            }
        }
        newG[N+a+1].weight = sumWeight;
        newG[N+a+1].value = sumValue;
        a++;
    }
    
    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            newG[i] = oldG[i];
        }
    }

    return newG;
}

void show(graph *G){
    for (int i = 0; i < N; i++) {
        std::cout << i << ": ";
        for (int j = 0; j < G[i].next.size(); j++) {
            std::cout << G[i].next[j]<< " ";
        }
        std::cout << "\n";
    }  
}


std::vector<int> topologicalSort (int size, graph *L, const std::vector<item> items) { //jak zroebic zeby nie zmienialo wartosci wskaznik hmm??
    
    std::cout << "Sortowanie:\n";

    std::vector <int> sortedGraph;

    int *ins, *visited;
    ins = new int[size]; visited = new int[size];

    for(int i = 0; i < N; i++){
        ins[i] = 0;
        visited[i] = 0;
    }
   // std::cout << size <<"\n";
    int inCount = size;
    graph *gCopy;
    gCopy = L;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < gCopy[i].next.size(); j++) {
            ins[gCopy[i].next[j]]++;
        }
    }
    //for(int a = 0; a < N; a++) std::cout << ins[a] << " ";
    std::cout << "Kopia listy zrobiona\n";
    //show(gCopy);
 
    while (inCount > 0) {
       // for(int a = 0; a < N; a++) std::cout << ins[a] << " ";
        //std::cout << "\n";
        //ma brac najwiekszy po ilorazie z tych nieodwiedzonych, gdzie ins == 0
        auto indexRemove = std::max_element(items.begin(), items.end(), [&visited, &items, &ins](const item& a, const item& b) {
            int indexA = &a - &items[0];
            int indexB = &b - &items[0];
        
        // Oblicz iloraz wartości do wagi tylko dla przedmiotów nieodwiedzonych
        //std::cout << "indeksy: " << indexA << " " << indexB << "\n";
            if (visited[indexA]) {
               // std::cout << "removed " << indexA+1 << " due to being visited and " << indexB+1 << " was picked\n";
                return true; // || ins[indexA] == 0) return false;
            }
            if (ins[indexA] != 0) {
                //std::cout << "removed " << indexA+1 << " due to having ins > 0 and " << indexB+1 << " was picked\n";
                return true;
            }
            if (visited[indexB]) {
                //std::cout << "removed " << indexB+1 << " due to being visited and " << indexA+1 << " was picked\n";
                return false;
            }
            if (ins[indexB] != 0) {
                //std::cout << "removed " << indexB+1 << " due to having ins > 0 and " << indexA+1 << " was picked\n";
                return false;
            }
            

            return a.value * b.weight < b.value * a.weight;
        });

        if (indexRemove != items.end()) {
           // std::cout << "ToRemove: " << indexRemove->number << "\n";
            if(!gCopy[indexRemove->number-1].next.empty()) {
                    //std::cout << i << ": ";
                    for (int j = 0; j < gCopy[indexRemove->number-1].next.size(); j++) {
                        //std::cout << " " << j << " ";
                        ins[gCopy[indexRemove->number-1].next[j]]--;
                        //std::cout << ins[gCopy[indexRemove->number-1].next[j]] << "\n";
                    }
                gCopy[indexRemove->number-1].next.clear();
            }
            visited[indexRemove->number-1] = 1;
            sortedGraph.push_back(indexRemove->number);
            inCount--;
        }
    }
    //show(gCopy);
    
    std::cout << "The graph has been sorted successfully (Kahn algorithm).\n";
    delete[] ins; delete[] visited;
    return sortedGraph;  //zwraca permutacje
}

