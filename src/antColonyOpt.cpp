#include "../include/define_include.h"
#include "../include/graphsOperations.h"

int extern N;
int extern B;

void showVer(std::vector<int> ver) {
    std::cout << "Acceptable Vertices:\n";
    for (int i = 0; i < ver.size(); i++) {
        std::cout << ver[i] << "\n";
    }
    std::cout << "\n";
}

void bulshow(std::vector<bool> ver) {
    //std::cout << "visited:\n";
    for (int i = 0; i < ver.size(); i++) {
        std::cout << ver[i] << "\n";
    }
    std::cout << "\n";
}

void possibleVertices(int capacity, std::vector<item> items, std::vector<std::vector<int>> prevG, std::vector<int> &accVer, std::vector<bool> visited, std::vector<bool> added) {
    accVer.clear();
    //showVer(accVer);
    //std::cout << "Tworzenie zbioru dopuszczalnych wierzchołków:\n";
    for (int i = 0; i < N; i++) { //jesli dany wierzcholek nie został jeszcze odwiedzony i ma dodanych do plecaka wszystkich poprzedników i sie miesci w plecaku
        if(!visited[i] && allPrevsAdded(prevG[i], added) && items[i].weight < capacity) {
            accVer.push_back(i);
        }    
    }
    //showVer(accVer);
}

void addToKnapsack(int &capacity, const std::vector<item> items, std::vector<bool> &added, int I, int &bestValue, antGraph *G) {
    if (items[I].weight <= capacity) {
        //std::cout << "Przedmiot " << I << " wszedł do plecaka. Ma wartosc: " << items[I].value << "\n";
        capacity -= items[I].weight;
        bestValue += items[I].value;
        added[I] = 1;
    }
}

Result antAlgorithm(const std::vector<item> items, const std::vector<std::pair<int,int>> dependencies, int ants, int iterations, int alfa, int beta, double evaporationRate, std::chrono::time_point<std::chrono::high_resolution_clock> startTime, std::chrono::time_point<std::chrono::high_resolution_clock> stopTime){
    
    int bestProfit, currProfit, capacity, finalProfit;
    Result result(N);
    
    antGraph *G;
    G = new antGraph[N];
    
    std::vector<std::vector<int>> prevG(N); 
    std::vector<int> acceptableVertices;
    antGraphCreate(G, prevG, items, dependencies);
    for (int i = 0; i < N; i++) {
        G[i].pheromoneLevel = 1.0;
        G[i].attractiveness = (double)items[i].value/(double)items[i].weight;
    }
    pheromoneItemSelector selector;

    std::vector<bool> added(N);
    std::vector<bool> path(N);
    

    int nextItem;

    startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        bestProfit = 0;
        //std::cout << "iteration: " << i << "\n";
        for(int a = 0; a < ants; a++) {
            std::vector<bool> visited(N);
            for (int j = 0; j < N; j++) {
                added[j] = 0; //nic nie jest dodane jak mrowka zaczyna
            }
            currProfit  = 0;
            //std::cout << "ant = " << a << "\n";
            capacity = B;
            while (capacity > 0) {
                //std::cout << "keeps checking for more items...\n";
                possibleVertices(capacity, items, prevG, acceptableVertices, visited, added);
                //showVer(acceptableVertices);
                if (acceptableVertices.empty()) { //nie ma nic do wybrania juz
                    //std::cout << "Nie ma wiecej potencjalnych przedmiotow\n";
                    break;
                }
                selector.adjust(G, acceptableVertices, alfa, beta, evaporationRate);
                nextItem = selector.selectItem();
                //std::cout << "next item: " << nextItem << " " << acceptableVertices[nextItem] << "\n";
                addToKnapsack(capacity, items, added, acceptableVertices[nextItem], currProfit, G);
                
                visited[acceptableVertices[nextItem]] = 1; //odwiedzony nawet jak nie weszło
                //std::cout << acceptableVertices[nextItem] << "visited? " << visited[acceptableVertices[nextItem]] << "\n";
                //bulshow(visited);
                
            } 
            if (currProfit > bestProfit) {
                path = added;
                bestProfit = currProfit;
            }
            
            //std::cout << "ant = " << a << "\t current profit = " << currProfit << "\t best profit in this iteration: " << bestProfit << "\n";
            //bulshow(path);

        }
        //evaporation mechanism and pheromones update:
        //tylko te ktore byly uwzglednione w rozwiazaniu
        if (i == 0) {
            finalProfit = bestProfit;
            result.arr = path;
        }
        for (int f = 0; f < N; f++) {
            if (path[f]) {
                G[f].pheromoneLevel *= evaporationRate;
                //std::cout << G[f].pheromoneLevel << " " << (1/(1+(double)(finalProfit-bestProfit)/(double)finalProfit)) <<"\n";
                G[f].pheromoneLevel += 1/(1+(double)(finalProfit-bestProfit)/(double)finalProfit); ///yyy czy tu jrst git???
            }
        }
        // std::cout << "Pheromone Levels:\n"; 
        //     for (int f = 0; f < N; f++) {
        //         std::cout << f << " " << G[f].pheromoneLevel << "\n";
        //     }
        //std::cout << bestProfit << " " << finalProfit << "\n";
        if (bestProfit > finalProfit) {
            finalProfit = bestProfit;
            //std::cout << "final: " << finalProfit << "\n";
            result.arr = path;
        }
    }

    //std::cout << finalProfit << "\n";
    //bulshow(path);
    delete[] G;
    result.value = finalProfit;
    stopTime = std::chrono::high_resolution_clock::now();
    return result;
}

//ewentualnie jeszcze dac set, do ktorego wrzucane są potencjalne wierzcholki, zeby nie usuwac calosci, tylko usuwac ten ktory został odwiedzony
//dodac klasy, zeby uzywac jednej funkcji, a nie dwóch do tworzenia grafu czy jakos tak, ewentualnie zrobic to w klasie, ze to zmienia dla kazdego