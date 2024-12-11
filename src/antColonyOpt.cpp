#include "../include/define_include.h"
#include "../include/graphsOperations.h"

int extern N;
int extern B;

int selectItem(antGraph *G, std::vector<int> verSet) {

    //bierze jakis losowy na razie, potem to bedzie zalezne od feromonow jakos
    //jak wybierze ten 
    if (!verSet.empty()) {
        return verSet[rand() % verSet.size()];
    }
    return -1;
}

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
}

void addToKnapsack(int &capacity, const std::vector<item> items, int I, int &finalValue) {
    if (items[I].weight <= capacity) {
        std::cout << "Przedmiot " << I << " wszedł do plecaka\n";
        capacity -= items[I].weight;
        finalValue += items[I].value;
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
   
    int nextItem;
    for (int i = 0; i < iterations; i++) {
        std::cout << "iteration: " << i <<"\n";
        std::vector<bool> visited(N);
        for(int a = 0; a < ants; a++) {
            finalValue  = 0;
            std::cout << "ant = " << a << "\n";
            capacity = B;
            possibleVertices(prevG, acceptableVertices, visited);
            showVer(acceptableVertices);
            currentItem = selectItem(G, acceptableVertices);
            addToKnapsack(capacity, items, currentItem, finalValue);
            visited[currentItem] = 1;
            //bulshow(visited);
            while (capacity > 0) {
                std::cout << "keeps checking for more items...\n";
                possibleVertices(prevG, acceptableVertices, visited);
                //showVer(acceptableVertices);
                //tworzymy zbior wierzcholków, które nie mają nieodwiedzonych poprzedników
                nextItem = selectItem(G, acceptableVertices);
                if (nextItem == -1) { //nie ma nic do wybrania juz
                    break;
                }
                addToKnapsack(capacity, items, nextItem, finalValue);
                visited[nextItem] = 1;
                //bulshow(visited);
            }

        }
    }
    delete[] G;
    //return result;
}