#include "../include/define_include.h"
#include "../include/bf.h"

int N;

int main(int argc, char *argv[]) {

    std::string algorithm = std::string(argv[1]);
    std::fstream data;
    
    data.open(argv[1]);

    int B, M; //the capacity of the knapsack and the number of dependencies
    data >> N >> B;

    std::vector<item> items(N);
    for (int i = 0; i < N; i++) { //wczytywanie wag i wartosci
        data >> items[i].weight >> items[i].value;
    }

    //wycztywanie zależności
    data >> M;
    std::vector<std::pair<int, int>> dependencies(M);
    for (int i = 0; i < M; i++) {
        data >> dependencies[i].first >> dependencies[i].second;
    }

    std::vector<bool> solution(N); //już jest wypełnione zerami
    for (int i = 0; i < N; i++) {
        std::cout << solution[i] << " ";
    }
    //std::cout << N << " " << B << '\n';
    for (int i = 0; i < N; i++) {
        std::cout << items[i].weight << " " << items[i].value << "\n";
    }
    //std::cout << "\n" << M << "\n";
    for(int i = 0; i < M; i++) {
        std::cout << dependencies[i].first << " " << dependencies[i].second << "\n";
    }
    //std::cout << "\n";
    Result result(N);
    //result = 
    bruteForce(B, items, dependencies);
    // std::cout << "Max. value: "<< result.value << " ";
    // std::cout << "Chosen items:\n";
    // for (int i = 0; i < N; i++) {
    //     if (result.arr[i] == 1) {
    //         std::cout << i+1 << " ";
    //     }
    // }
    // std::cout << "\n";
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
// 2 0
// 3 1



