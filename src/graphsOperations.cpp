#include "../include/define_include.h"


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
bool dfsFindCycle(graph *L, int V, int W, std::stack<int> &S, bool *visited)
{
    //std::cout<< "W=" << W << "\n";
    visited[W] = true;

    S.push(W); //biezacy wierzcholek

    //for (std::vector<int>::iterator it = L[W].next.begin(); it != L[W].next.end(); it++)
    //std::cout << "neighbours: ";
    for (int i = 0; i < L[W].next.size(); i++) {
        //std::cout << L[W].next[i] << " ";
            if (L[W].next[i] == V) { //doszlo do poczatkowego V - pocz, W - current
                return true;
            }
            if (!visited[L[W].next[i]] && dfsFindCycle(L, V, L[W].next[i], S, visited)) {
                return true;
            }
    }
    S.pop();
    return false;
}

void isCyclic(graph *L, const std::vector<item> items, std::set<std::set<item>> &cycles) {

    std::cout << "Sprawdzanie czy jest cykl:\n";
    std::stack <int> S, T;
    std::set<item> sSet;
    bool *visited;
    visited = new bool[N];
    for (int i = 0; i < N; i++) {
        //std::cout << "V start:" << i << "\n";
        for (int j = 0; j < N; j++) {
            visited[j] = 0;
        }
        
        if(dfsFindCycle(L,i,i,S,visited)) { 
            //std::cout << "jest\n";
            T.push(i);
            while (!S.empty()) {
                T.push(S.top());
                S.pop();
            }
            while (!T.empty()) {
                //std::cout << "v: " << T.top() << " ";
                sSet.insert(items[T.top()]);
                T.pop();
            }
            //std::cout << "\n";
            cycles.insert(sSet);
        }
    }
    for (int j = 0; j < N; j++) {
        visited[j] = 0;
    }
    // for (std::set<std::set<item>>::iterator it = cycles.begin(); it != cycles.end(); it++) {
    //     std::cout << "numer cyklu: " << 0 << "\n";
    //     for (std::set<item>::iterator vertice = it->begin(); vertice != it->end(); vertice++) {
    //         std::cout << vertice->number << " ";
    //     }
    //     std::cout << "\n";
    // }
    //łączenie cykli
    delete[] visited;
}

void show(graph *G, int len){
    for (int i = 0; i < len; i++) {
        std::cout << i << ": ";
        for (int j = 0; j < G[i].next.size(); j++) {
            std::cout << G[i].next[j]<< " ";
        }
        std::cout << "\n";
    }  
}

graph *transformGraph(graph *oldG, std::set<std::set<item>> &cycles) {
    
    graph *newG;
    newG = new graph[2*N];
    bool *visited; 
    visited = new bool[N+cycles.size()];
    for (int i = 0; i < N; i++) {
        visited[i] = 0;
    }
    int sumWeight, sumValue, a = 0;
    
    for (std::set<std::set<item>>::iterator it = cycles.begin(); it != cycles.end(); it++) {
        sumWeight = 0;
        sumValue = 0;
        for (std::set<item>::iterator vertice = it->begin(); vertice != it->end(); vertice++) {
            sumWeight += vertice->weight;
            sumValue += vertice->value;
            visited[vertice->number-1] = 1;
            for (int i = 0; i < N; i++) {
                for (long unsigned int j = 0; j < oldG[i].next.size(); j++) {
                    if (oldG[i].next[j] == vertice->number) { //szukanie poprzedników nienalezacych do cyklu dodac
                        newG[N+a].next.push_back(vertice->number);
                    }
                }
                //te ktore są w cyklach to -1
            }
            newG[vertice->number-1].next.push_back(-1);
        }
        newG[N+a].weight = sumWeight;
        newG[N+a].value = sumValue;
        //std::cout << newG[N+a].weight << " " << newG[N+a].value << "\n";
        a++;
    }
    
    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            newG[i] = oldG[i];
        }
    }
    //show(oldG, N);
    //std::cout << "new:\n";
    //show(newG, N+cycles.size());
    return newG;
}


std::vector<int> topologicalSort (int size, graph *G, const std::vector<item> items) { //jak zroebic zeby nie zmienialo wartosci wskaznik hmm??
    
    std::cout << "Sortowanie: " << size << "\n";

    std::vector <int> sortedGraph;

    int *ins, *visited;
    ins = new int[size]; visited = new int[size];

    for(int i = 0; i < size; i++){
        ins[i] = 0;
        visited[i] = 0;
    }
    int inCount = 0;
    graph *gCopy;
    gCopy = G;
    //std::cout << "Kopia listy zrobiona:\n";
    //show(gCopy, size);
    std::cout << "\n";

    for (int i = 0; i < size; i++) {
        if (gCopy[i].next[0] != -1) {
            for (int j = 0; j < gCopy[i].next.size(); j++) {
                std::cout << gCopy[i].next[j] << "\n";
                if (gCopy[j].next[0] != -1) {
                    ins[gCopy[i].next[j]-1]++;
                    inCount++;
                }
            }
        }
        else {
            ins[i] = -1;
            visited[i] = 1;
        }
    }
    std::cout << "inCount = " << inCount << "\n";
    for (int i = 0; i < size; i++) {
        std::cout << i << " " << ins[i] << "\n";
    }
    std::cout << "\n";
 
    while (inCount >= 0) {
        //ma brac najwiekszy po ilorazie z tych nieodwiedzonych, gdzie ins == 0
        auto indexRemove = std::max_element(items.begin(), items.end(), [&visited, &items, &ins, &gCopy](const item& a, const item& b) {
            int indexA = &a - &items[0];
            int indexB = &b - &items[0];

        // Oblicz iloraz wartości do wagi tylko dla przedmiotów nieodwiedzonych
            
            if (visited[indexA]) {
                //std::cout << "removed " << indexA+1 << " due to being visited and " << indexB+1 << " was picked\n";
                return true; 
            }
            if (ins[indexA] > 0) {
                //std::cout << "removed " << indexA+1 << " due to having ins > 0 and " << indexB+1 << " was picked\n";
                return true;
            }
            
            if (visited[indexB]) {
                //std::cout << "removed " << indexB+1 << " due to being visited and " << indexA+1 << " was picked\n";
                return false;
            }
            if (ins[indexB] > 0) {
                //std::cout << "removed " << indexB+1 << " due to having ins > 0 and " << indexA+1 << " was picked\n";
                return false;
            }
            
            

            return a.value * b.weight < b.value * a.weight;
        });

        if (indexRemove != items.end()) {
            std::cout << "ToRemove: " << indexRemove->number << "\n";
            if(!gCopy[indexRemove->number-1].next.empty() && gCopy[indexRemove->number-1].next[0] != -1) {
                    std::cout << indexRemove->number << ": ";
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
    //show(gCopy, size);
    
    std::cout << "The graph has been sorted successfully (Kahn algorithm):\n";
    for (int i = 0; i < sortedGraph.size(); i++) {
        std::cout << sortedGraph[i] << " ";
    }
    std::cout << "\n";

    delete[] ins; delete[] visited;
    return sortedGraph;  //zwraca permutacje
}

