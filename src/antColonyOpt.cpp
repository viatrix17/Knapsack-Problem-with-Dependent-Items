#include "../include/define_include.h"
#include "../include/graphsOperations.h"

int extern N;
int extern B;

bool allPrevsAdded(std::vector<int> vertice, std::vector<bool> added) {
    for (int i = 0; i < vertice.size(); i++) {
        if (!added[vertice[i]]) {
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

void possibleVertices(std::vector<std::vector<int>> prevG, std::vector<int> &accVer, std::vector<bool> visited, std::vector<bool> added) {
    accVer.clear();
    //showVer(accVer);
    std::cout << "Tworzenie zbioru dopuszczalnych wierzchołków:\n";
    for (int i = 0; i < N; i++) { //jesli dany wierzcholek nie został jeszcze odwiedzony i ma dodanych do plecaka wszystkich poprzedników
        if(!visited[i] && allPrevsAdded(prevG[i], added)) {
            accVer.push_back(i);
        }    
    }
    showVer(accVer);
}

void addToKnapsack(int &capacity, const std::vector<item> items, std::vector<bool> &added, int I, int &finalValue, antGraph *G) {
    if (items[I].weight <= capacity) {
        std::cout << "Przedmiot " << I << " wszedł do plecaka\n";
        capacity -= items[I].weight;
        finalValue += items[I].value;
        G[I].pheromoneLevel += items[I].value;
        added[I] = 1;
    }
}

void antAlgorithm(const std::vector<item> items, const std::vector<std::pair<int,int>> dependencies){
    
    int iterations = 1, ants = 2, currentItem = -1, finalValue, capacity;
    Result result(N);
    
    antGraph *G;
    G = new antGraph[N];
    std::vector<std::vector<int>> prevG(N); 
    std::vector<int> acceptableVertices;
    antGraphCreate(G, prevG, items, dependencies);
    pheromoneItemSelector selector;

    //isCyclic(G, items, cycles);

    int nextItem;
    for (int i = 0; i < iterations; i++) {
        std::cout << "iteration: " << i <<"\n";
        for(int a = 0; a < ants; a++) {
            std::vector<bool> visited(N);
            std::vector<bool> added(N);
            finalValue  = 0;
            std::cout << "ant = " << a << "\n";
            capacity = B;
            possibleVertices(prevG, acceptableVertices, visited, added);
            //showVer(acceptableVertices);
            selector.adjust(G, acceptableVertices);
            //showVer(acceptableVertices);
            currentItem = selector.selectItem();
            std::cout << "chosen item: " << acceptableVertices[currentItem] << "\n";
            addToKnapsack(capacity, items, added, acceptableVertices[currentItem], finalValue, G);
            visited[acceptableVertices[currentItem]] = 1;
            bulshow(visited);
            while (capacity > 0) {
                std::cout << "keeps checking for more items...\n";
                possibleVertices(prevG, acceptableVertices, visited, added);
                //showVer(acceptableVertices);
                if (acceptableVertices.empty()) { //nie ma nic do wybrania juz
                    break;
                }
                selector.adjust(G, acceptableVertices);
                nextItem = selector.selectItem();
                addToKnapsack(capacity, items, added, acceptableVertices[nextItem], finalValue, G);
                
                visited[acceptableVertices[nextItem]] = 1; //odwiedzony nawet jak nie weszło
                bulshow(visited);
            }


            std::cout << "Pheromone Levels:\n";
            for (int f = 0; f < N; f++) {
                std::cout << f << " " << G[f].pheromoneLevel << "\n";
            }
            std::cout << "ant = " << a << "\t finalValue = " << finalValue << "\n";

        }
       
        for (int f = 0; f < N; f++) {
            G[f].pheromoneLevel *= 0.9;
        }
    }
    delete[] G;
    //return result;
}

//ewentualnie jeszcze dac set, do ktorego wrzucane są potencjalne wierzcholki, zeby nie usuwac calosci, tylko usuwac ten ktory został odwiedzony
//dodac klasy, zeby uzywac jednej funkcji, a nie dwóch do tworzenia grafu czy jakos tak, ewentualnie zrobic to w klasie, ze to zmienia dla kazdego