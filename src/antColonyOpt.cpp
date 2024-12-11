#include "../include/define_include.h"
#include "../include/graphsOperations.h"

int extern N;
int extern B;

bool allPrevsVisited(std::vector<int> vertice, std::vector<bool> visited) {
    for (int i = 0; i < vertice.size(); i++) {
        //std::cout << visited[vertice[i]] << "\n";
        if (!visited[vertice[i]]) {
            return false;
        }
    }
    return true;
}

void showVer(std::vector<int> ver) {
    std::cout << "Acceptable Vertices:\n";
    for (int i = 0; i < ver.size(); i++) {
        std::cout << ver[i] << "\n";
    }
    std::cout << "\n";
}

void bulshow(std::vector<bool> ver) {
    std::cout << "visited:\n";
    for (int i = 0; i < ver.size(); i++) {
        std::cout << ver[i] << "\n";
    }
    std::cout << "\n";
}

void possibleVertices(std::vector<std::vector<int>> prevG, std::vector<int> &accVer, std::vector<bool> visited) {
    accVer.clear();
    //showVer(accVer);
    std::cout << "Tworzenie zbioru dopuszczalnych wierzchołków:\n";
    for (int i = 0; i < N; i++) { //szukanie czy dany wierzcholek ma nieodwiedzonych poprzedników;
        if(!visited[i] && allPrevsVisited(prevG[i], visited)) {
            accVer.push_back(i);
        }    
    }
    showVer(accVer);
}

void addToKnapsack(int &capacity, const std::vector<item> items, int I, int &finalValue, antGraph *G) {
    if (items[I].weight <= capacity) {
        std::cout << "Przedmiot " << I << " wszedł do plecaka\n";
        capacity -= items[I].weight;
        finalValue += items[I].value;
        G[I].pheromoneLevel += items[I].value;
    }
}

void antAlgorithm(const std::vector<item> items, const std::vector<std::pair<int,int>> dependencies){
    
    int iterations = 1, ants = 1, currentItem = -1, finalValue, capacity;
    Result result(N);
    antGraph *G;
    G = new antGraph[N];
    std::vector<std::vector<int>> prevG(N); 
    std::vector<int> acceptableVertices;
    antGraphCreate(G, prevG, items, dependencies);
    pheromoneItemSelector selector;

   
    int nextItem;
    for (int i = 0; i < iterations; i++) {
        std::cout << "iteration: " << i <<"\n";
        std::vector<bool> visited(N);
        for(int a = 0; a < ants; a++) {
            finalValue  = 0;
            std::cout << "ant = " << a << "\n";
            capacity = B;
            possibleVertices(prevG, acceptableVertices, visited);
            //showVer(acceptableVertices);
            selector.adjust(G, acceptableVertices);
            //showVer(acceptableVertices);
            currentItem = selector.selectItem();
            std::cout << "chosen item: " << acceptableVertices[currentItem] << "\n";
            addToKnapsack(capacity, items, acceptableVertices[currentItem], finalValue, G);
            visited[acceptableVertices[currentItem]] = 1;
            //bulshow(visited);
            while (capacity > 0) {
                std::cout << "keeps checking for more items...\n";
                possibleVertices(prevG, acceptableVertices, visited);
                //showVer(acceptableVertices);
                if (acceptableVertices.empty()) { //nie ma nic do wybrania juz
                    break;
                }
                selector.adjust(G, acceptableVertices);
                nextItem = selector.selectItem();
                addToKnapsack(capacity, items, acceptableVertices[nextItem], finalValue, G);
                visited[acceptableVertices[nextItem]] = 1;
                //bulshow(visited);
            }
            for (int f = 0; f < N; f++) {
                std::cout << G[f].pheromoneLevel << "\n";
            }

        }
       
        for (int f = 0; f < N; f++) {
            G[f].pheromoneLevel *= 0.9;
        }
    }
    delete[] G;
    //return result;
}

//dodac update'owanie feromonów z zyskiem