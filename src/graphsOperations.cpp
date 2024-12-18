#include "../include/define_include.h"


extern int N; //liczba wierzchołków

void antGraphCreate(antGraph *G, std::vector<std::vector<int>> &prevG, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies) {
   // std::cout << "Tworzenie grafów:\nDodawanie feromonów początkowych:\n";
    for (long unsigned int i = 0; i < items.size(); i++) {
        G[i].pheromoneLevel = 1.0;
    }
    //std::cout << "Dodawanie krawędzi:\n";
    for (long unsigned int i = 0; i < dependencies.size(); i++) {
        //std::cout << i << "\t\t" << dependencies[i].second << " " << items[dependencies[i].second-1].number << "\t" << dependencies[i].first << " " << items[dependencies[i].first-1].number << "\n";
        G[dependencies[i].second-1].n.push_back(dependencies[i].first-1);
        prevG[dependencies[i].first-1].push_back(dependencies[i].second-1);
    }
}

void createGraph(graph *G, std::vector<std::vector<int>> &prevG, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies) {

    for (long unsigned int i = 0; i < items.size(); i++) {
         
        G[i].weight = items[i].weight;
        G[i].value = items[i].value;
    }
    for (long unsigned int i = 0; i < dependencies.size(); i++) {
        G[dependencies[i].second-1].next.push_back(dependencies[i].first-1);
        prevG[dependencies[i].first-1].push_back(dependencies[i].second-1);
    }
}

//DFS do cykli
bool dfsFindCycle(graph *L, int V, int W, std::stack<int> &S, bool *visited)
{
    visited[W] = true;
    S.push(W); //biezacy wierzcholek

    for (int i = 0; i < L[W].next.size(); i++) {
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

void deleteSuccessor(graph *oldG, int ver, std::vector<std::pair<bool,int>> &visited) {
    
    int nextVer;
    for (int i = 0; i < oldG[ver].next.size(); i++){
        nextVer = oldG[ver].next[i];
        if (!visited[nextVer].first && visited[nextVer].second == -1) { //jesli jeszcze nie byl odwiedzony, to tam wchodzimy i jesli nie nalezy cyklu
            deleteSuccessor(oldG, nextVer, visited);
        }
    }
        
    oldG[ver].next.clear();
    oldG[ver].next.push_back(-1);
    visited[ver].first = 1; //odwiedzony, juz wiecej nic z nim nie robimy
    oldG[ver].weight = 0;
    oldG[ver].value  = 0;

}

void deleteCycles(graph *oldG, std::vector<std::vector<int>> &prevOld, std::set<std::set<item>> &cycles) {
    
    std::cout << "usuwanie cyklu i jego nastepnikow\n";
    int pom;
    std::vector<std::pair<bool,int>> visited(N);
    for (int i = 0; i < N; i++) {
        visited[i].first = 0;
        visited[i].second = -1;
    }
    for (std::set<std::set<item>>::iterator it = cycles.begin(); it != cycles.end(); it++) {
        for (std::set<item>::iterator vertice = it->begin(); vertice != it->end(); vertice++) { //idzie po wierzcholkach w cyklu
            //std::cout << "vertice in a cycle: " << vertice->number-1 << "\n";
            visited[vertice->number-1].first = 1;
            visited[vertice->number-1].second = pom;
            oldG[vertice->number-1].weight = 0;
            oldG[vertice->number-1].value  = 0;
            
        }
        pom++;
    }

    
    for (std::set<std::set<item>>::iterator it = cycles.begin(); it != cycles.end(); it++) {
        for (std::set<item>::iterator vertice = it->begin(); vertice != it->end(); vertice++){
            //wywalamy wszystkich nastepnikow, ktorzy nie sa w cyklu
            deleteSuccessor(oldG, vertice->number-1, visited);
            
        }
    }

    for (int i = 0; i < N; i++) {
        if (visited[i].second == -1 && !visited[i].first) {//nie nalezy do cyklu zadnego to przepisz dane
            oldG[i].value = oldG[i].value;
            oldG[i].weight = oldG[i].weight;
            for (int j = 0; j < oldG[i].next.size(); j++) {
                if (visited[oldG[i].next[j]].second != -1) { //jesli nastewpnik nalezy do cyklu to go usun 
                    oldG[i].next.erase(oldG[i].next.begin()+j);
                }
            }
        }
    }

    for(int i = 0; i < N; i++) {
        if (!oldG[i].next.empty()) {
            if (oldG[i].next[0] == -1) {
                prevOld[i].clear();
                prevOld[i].push_back(-1);
            }
        }
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
    
    //usuwanie wierzcholkow zeby mialy 0 w wartosciach i nastepnikach -1
    for (std::set<std::set<item>>::iterator it = cycles.begin(); it != cycles.end(); it++) {
        for (std::set<item>::iterator vertice = it->begin(); vertice != it->end(); vertice++) { //idzie po wierzcholkach w cyklu
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
            
            for(int a = 0; a < N+cycles.size(); a++) {
                visited[a].first = 0;
            }
        }
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

    for (int i = 0; i < size; i++) {
        if (!gCopy[i].next.empty()) {
            if (gCopy[i].next[0] != -1) {
                for (int j = 0; j < gCopy[i].next.size(); j++) {
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

    int indexRemove;
    double maxRatio;
    while (std::accumulate(visited.begin(), visited.end(), 0) != size) {
        maxRatio = 0.0;
        indexRemove = -1;
        for (int i = 0; i < size; i++) {
            if (ins[i] == 0 && visited[i] == 0) {
                double ratio = static_cast<double>(gCopy[i].value) / gCopy[i].weight;
                if (ratio > maxRatio) {
                    maxRatio = ratio;
                    indexRemove = i;
                }
            }
        }
        
        if (indexRemove != -1) {
            //std::cout << "ToRemove: " << indexRemove << "\n";
            if(!gCopy[indexRemove].next.empty() && gCopy[indexRemove].next[0] != -1) {
                    for (int j = 0; j < gCopy[indexRemove].next.size(); j++) {
                        ins[gCopy[indexRemove].next[j]]--;
                    }
                gCopy[indexRemove].next.clear();
            }
            visited[indexRemove] = 1;
            sortedGraph.push_back(indexRemove); 
        }
    }
    //show(gCopy, size);
    
    std::cout << "The graph has been sorted successfully (Kahn algorithm):\n";
    // for (int i = 0; i < sortedGraph.size(); i++) {
    //     std::cout << sortedGraph[i] << " ";
    // }
    // std::cout << "\n\n";

    return sortedGraph;  //zwraca permutacje
}

