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
    for (std::set<std::set<item>>::iterator it = cycles.begin(); it != cycles.end(); it++) {
        std::cout << "numer cyklu: " << 0 << "\n";
        for (std::set<item>::iterator vertice = it->begin(); vertice != it->end(); vertice++) {
            std::cout << vertice->number << " ";
        }
        std::cout << "\n";
    }
    //łączenie cykli
    delete[] visited;
}

void show(graph *G, int len){
    for (int i = 0; i < len; i++) {
        std::cout << i << ": ";
        for (int j = 0; j < G[i].next.size(); j++) {
            std::cout << G[i].next[j]<< " ";
        }
        std::cout << "\t\t" << G[i].weight << " " << G[i].value << "\n";
    }  
}

graph *transformGraph(graph *oldG, std::set<std::set<item>> &cycles) {
    
    graph *newG;
    newG = new graph[N+cycles.size()];
    std::vector<std::pair<bool, int>> visited(N+cycles.size());
    for (int i = 0; i < N+cycles.size(); i++) {
        visited[i].first = 0;
        visited[i].second = -1;
    }
    int sumWeight, sumValue, pom = 0, searchVertice;
    
    for (std::set<std::set<item>>::iterator it = cycles.begin(); it != cycles.end(); it++) {
        for (std::set<item>::iterator vertice = it->begin(); vertice != it->end(); vertice++) { //idzie po wierzcholkach w cyklu
            //std::cout << "vertice in a cycle: " << vertice->number-1 << "\n";
            visited[vertice->number-1].first = 1;
            visited[vertice->number-1].second = pom;
            //jesli jest czescia jakiegos cyklu, to wtedy ma -1 w nowym grafie
            newG[vertice->number-1].next.push_back(-1);
            newG[N+pom].weight += oldG[vertice->number-1].weight;
            newG[N+pom].value += oldG[vertice->number-1].value;
            newG[vertice->number-1].weight = 0;
            newG[vertice->number-1].value = 0;
        }
        pom++;
    }

    for (int i = 0; i < N; i++) {
        if (visited[i].second == -1) {//nie nalezy do cyklu zadnego to przepisz dane
            newG[i].value = oldG[i].value;
            newG[i].weight = oldG[i].weight;
        }
        //std::cout << visited[i].first << " " << visited[i].second << "\n";
    }

    for (int i = 0; i < N; i++) {
        //std::cout << "vertice: " << i << "\n";
        for (int j = 0; j < oldG[i].next.size(); j++) {
        //std::cout << "successors: " << oldG[i].next[j] << " " << visited[oldG[i].next[j]].second << "\n";
            if (visited[oldG[i].next[j]].second == -1 ) { //nie nalezy do zadnego cyklu i jeszcze nie bylo odwiedzone
                //std::cout << "Nie nalezy do zadnego cyklu\n";
                if (visited[oldG[i].next[j]].first == 0) {
                    //std::cout << "isn't a part of any cycle and hasn't been visited yet\n";
                    newG[i].next.push_back(j);
                    visited[oldG[i].next[j]].first = 1;
                }
            }
            else if (visited[oldG[i].next[j]].second != visited[i].second) { //jesli nalezy do cyklu ale jest to cykl inny to podepnij ten cykl
                //std::cout << "is the part of a different cycle: " << visited[j].second << "\n";
                newG[i].next.push_back(visited[j].second);
                visited[j].first = 1;
                visited[visited[j].second].first = 1;
            }
            // else if (visited[oldG[i].next[j]].second == visited[i].second) {
            //     std::cout << "is the part of the same cycle\n";
            // }
            for(int a = 0; a < N+cycles.size(); a++) {
                visited[a].first = 0;
            }
        }
        //std::cout << "\n";
    }
  
    //delete visited;
    return newG;
}


std::vector<int> topologicalSort (int size, graph *G, const std::vector<item> items) { //jak zroebic zeby nie zmienialo wartosci wskaznik hmm??
    
    std::cout << "Sortowanie: " << size << "\n";

    std::vector <int> sortedGraph;

    std::vector<int> ins(size);
    std::vector<int> visited(size);
    
    int inCount = 0;
    graph *gCopy;
    gCopy = G;
    std::cout << "Kopia listy zrobiona:\n";
    //show(gCopy, size);
    std::cout << "\n";

    for (int i = 0; i < size; i++) {
        //std::cout << i << " ";
        if (!gCopy[i].next.empty()) {
            if (gCopy[i].next[0] != -1) {
            for (int j = 0; j < gCopy[i].next.size(); j++) {
                //std::cout << gCopy[i].next[j] << "\n";
                if (gCopy[j].next[0] != -1) {
                    ins[gCopy[i].next[j]]++;
                    inCount++;
                }
            }
            }
            else {
                ins[i] = -1;
            visited[i] = 1;
            }
        }
       
    }
    //std::cout << "inCount = " << inCount << "\n\tvisited:\tins:\n";
    // for (int i = 0; i < size; i++) {
    //     std::cout << i << "\t" << visited[i] << "\t\t" << ins[i] << "\n";
    // }
    // std::cout << "\n";
 
    int indexRemove;
    double maxRatio;
    //std::cout << std::accumulate(visited.begin(), visited.end(), 0) << "\n";
    while (std::accumulate(visited.begin(), visited.end(), 0) != size) {
        maxRatio = 0.0;
        indexRemove = -1;
        for (int i = 0; i < size; i++) {
            if (ins[i] == 0 && visited[i] == 0) {
                double ratio = static_cast<double>(gCopy[i].value) / gCopy[i].weight;
                //std::cout << i << "\tRatio = " << ratio << "\n";                
                if (ratio > maxRatio) {
                    maxRatio = ratio;
                    indexRemove = i;
                }
            }
        }
        //ma brac najwiekszy po ilorazie z tych nieodwiedzonych, gdzie ins == 0
        // auto indexRemove = std::max_element(items.begin(), items.end(), [&visited, &items, &ins, &gCopy](const item& a, const item& b) {
        //     int indexA = &a - &items[0];
        //     int indexB = &b - &items[0];

        // // Oblicz iloraz wartości do wagi tylko dla przedmiotów nieodwiedzonych
            
        //     if (visited[indexA]) {
        //         std::cout << "removed " << indexA+1 << " due to being visited and " << indexB+1 << " was picked\n";
        //         return true; 
        //     }
        //     if (ins[indexA] > 0) {
        //         std::cout << "removed " << indexA+1 << " due to having ins > 0 and " << indexB+1 << " was picked\n";
        //         return true;
        //     }
            
        //     if (visited[indexB]) {
        //         std::cout << "removed " << indexB+1 << " due to being visited and " << indexA+1 << " was picked\n";
        //         return false;
        //     }
        //     if (ins[indexB] > 0) {
        //         std::cout << "removed " << indexB+1 << " due to having ins > 0 and " << indexA+1 << " was picked\n";
        //         return false;
        //     } //do poprawki zeby bralo wierzcholki z grafu
            
            

        //     return a.value * b.weight < b.value * a.weight;
        // });
        //std::cout << indexRemove << "\n";
        if (indexRemove != -1) {
            std::cout << "ToRemove: " << indexRemove << "\n";
            if(!gCopy[indexRemove].next.empty() && gCopy[indexRemove].next[0] != -1) {
                    for (int j = 0; j < gCopy[indexRemove].next.size(); j++) {
                        //std::cout << " " << j << " ";
                        ins[gCopy[indexRemove].next[j]]--;
                        //std::cout << ins[gCopy[indexRemove->number-1].next[j]] << "\n";
                    }
                gCopy[indexRemove].next.clear();
            }
            visited[indexRemove] = 1;
            //std::cout << std::accumulate(visited.begin(), visited.end(), 0) << "\n";
            sortedGraph.push_back(indexRemove); //w permutacji niech tez beda indeksy, w wyniku sie to zmieni
            //inCount--;
        }
    }
    //show(gCopy, size);
    
    std::cout << "The graph has been sorted successfully (Kahn algorithm):\n";
    for (int i = 0; i < sortedGraph.size(); i++) {
        std::cout << sortedGraph[i] << " ";
    }
    std::cout << "\n";

    return sortedGraph;  //zwraca permutacje
}

