#include "../include/define_include.h"
#include "../include/bf.h"
#include "../include/greedyKnapsack.h"
#include "../include/antColonyOpt.h"

int N, B;

void argsCorrect(int argc, char *argv[]) {
    if (argc <= 0) { 
        perror("Too few arguments to execute program.");
        exit(0);
    }
    if (std::string(argv[1]) == "AM" && argc < 4) {
        perror("Too few arguments to execute AM.\n");
        exit(0);
    }
    if (std::string(argv[1]) == "ABF" && argc < 3) {
        perror("Too few arguments to exectute ABF.");
        exit(0);
    }
    if (std::string(argv[1]) == "AZ" && argc < 3) {
        perror("Too few arguments to exectute AZ.");
        exit(0);    
    }
}

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

    argsCorrect(argc, argv);
    std::string algorithm = std::string(argv[1]);
    std::fstream data;
    
    data.open(argv[2]);

    int M; //the number of dependencies
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
    if (algorithm == "ABF") {
        result = bruteForce(B, items, dependencies);
    }
    if (algorithm == "AZ") {
        result = greedyDependentKnapsack(B, items, dependencies);
    }
    if (algorithm == "AM") {
        srand(time(0));
        std::fstream parameters;
        int ants, iterations, alfa, beta; //the number of dependencies
        double evaporationRate;
        parameters.open(argv[3]);
        parameters >> iterations;
        parameters >> ants;
        parameters >> alfa;
        parameters >> beta;
        parameters >> evaporationRate;
        //std::cout << ants << " " << iterations << " " << alfa << " " << beta << " " << evaporationRate << "\n";
       
        result = antAlgorithm(items, dependencies, ants, iterations, alfa, beta, evaporationRate); //bez cykli
    }
    showResult(result, algorithm);
    data.close();
    return 0;
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


