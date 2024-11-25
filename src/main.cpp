#include "../include/define_include.h"
#include "../include/bf.h"

int main(int argc, char *argv[]) {

    std::string algorithm = std::string(argv[1]);
    std::fstream data;
    
    data.open(argv[1]);

    int N,B,M; //ilosc przedmiotow i pojemnosc plecaka
    data >> N >> B;

    std::vector<item> items(N);
    for (int i = 0; i < N; i++) { //wczytywanie wag i wartosci
        data >> items[i].weight >> items[i].value;
        //std::cout << items[i].weight << " " << items[i].value << "\n";
    }

    //wycztywanie zależności
    data >> M;
    std::vector<std::pair<int, int>> dependencies(M);
    for (int i = 0; i < M; i++) {
        data >> dependencies[i].first >> dependencies[i].second;
        //std::cout << dependencies[i].first << " " << dependencies[i].second << "\n";
    }

    std::vector<bool> solution(N); //już jest wypełnione zerami
    for (int i = 0; i < N; i++) {
        std::cout << solution[i] << " ";
    }
    std::cout << "\n\n";
    std::cout << bruteForce(N, B, items, dependencies) << "\n";
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



