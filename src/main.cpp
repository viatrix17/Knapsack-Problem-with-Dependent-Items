#include "../include/define_include.h"
#include "../include/bf.h"
#include "../include/greedyKnapsack.h"
#include "../include/antColonyOpt.h"

int N, B;

void showResult(Result result, std::string title) {
    std::cout << title << "\n";
    std::cout << "Max. value: "<< result.value << "\n";
    std::cout << "Chosen items: ";
    if (std::accumulate(result.arr.begin(), result.arr.end(),0) == 0) {
        std::cout << "No items can be chosen.\n";
    }
    else {
        for (int i = 0; i < N; i++) {
            if (result.arr[i] == 1) {
                std::cout << i+1 << " ";
            }
        }
    }
    std::cout << "\n";

}

void showData(std::vector<item> items, std::vector<std::pair<int, int>> dependencies, int B, int M){
     std::cout << N << " " << B << '\n';
    for (int i = 0; i < N; i++) {
        std::cout << items[i].weight << " " << items[i].value << "\n";
    }
    std::cout << "\n" << M << "\n";
    for(int i = 0; i < M; i++) {
        std::cout << dependencies[i].first << " " << dependencies[i].second << "\n";
    }
    std::cout << "\n";
}

int main(int argc, char *argv[]) {

    if(argc == 1) exit(0);
    std::string algorithm = std::string(argv[1]);
    std::fstream data;
    
    data.open(argv[1]);

    int M; //the capacity of the knapsack and the number of dependencies
    data >> N >> B;

    std::vector<item> items(N);
    for (int i = 0; i < N; i++) { //wczytywanie wag i wartosci
        data >> items[i].number >> items[i].weight >> items[i].value;
    }

    //wycztywanie zależności
    data >> M;
    std::vector<std::pair<int, int>> dependencies(M);
    for (int i = 0; i < M; i++) {
        data >> dependencies[i].first >> dependencies[i].second;
    }

    //showData(items, dependencies, B, M);
  
    Result result(N);
    //result = bruteForce(B, items, dependencies);
    //showResult(result, "Brute Force");
    //result = greedyDependentKnapsack(B, items, dependencies);
    //showResult(result, "Greedy Knapsack");
    antAlgorithm(items, dependencies); //bez cykli
    
}

// plik wyglada tak
// liczba przedmiotów   pojemność plecaka
// waga przedmiotu  wartość przedmiotu
// liczba zależności
// A    B -> przedmiot A może być spakowany tylko jeśli przedmiot B jest już spakowany


// 4 10
// (0) 1 3
// (1) 4 5
// (2) 7 2
// (3) 2 5
// 2
// 2 0          2 nie moze byc przed 0
// 3 1


//GRAFY SORTOWANIE TOPOLOGICZNE -> wykrywanie na samym początku czy jest cykl, wtedy nie ma takiego ustawienia, ale to tylko dziala jesli myslimy o ustawieniu wszystkich przedmiotow
//grafy wykrywanie sciezki
