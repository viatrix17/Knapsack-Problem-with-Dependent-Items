#include "../include/define_include.h"
#include "../include/graphsOperations.h"

extern int N;

Result greedyDependentKnapsack(int capacity, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies, int type) {
    
    Result result(N);
    graph *G;
    std::vector<bool> solution(N);
    std::vector<int> perm;
    createGraph(G, items, dependencies);
    //sprawdzic czy zmieni sie kolejnosc items po tym; to mnie nie interesuje bo sortuję graf ale i tak lepiej srpawdzic 
    //nie sortuje danych, najpierw sprawdzam czy jest cykl

    if (isCyclic(G)) { //jeśli ma cykl to wywal zachłannie wedlug tam tego ilorazu i wtedy sortuj i tak chyba dopoki wszystkich cykli nie zrobisz???

    }
    else { //jeśli nie ma cyklu to sortuj topologicznie 
        perm = topologicalSort(G, items, dependencies); //ma zwracac permutacje
    }

    double finalvalue = 0.0;
 
    for (int i = 0; i < perm.size(); i++) {
         
        if (items[perm[i]-1].weight <= capacity) {
            capacity -= items[perm[i]-1].weight;
            finalvalue += items[perm[i]-1].value;
            solution[items[perm[i]-1].number-1] = 1;
        }
    }

    //usuwanie Listy
    delete[] G;
    return result;
}

//wykrywanie cyklu, czy w ogole w ten sposob mozna posortowac, a jak się nie da to w sumie co?? trzeba wywalic wierzcholek ktory ma dwie krawedzie

//sortowanie topologiczne, przechodzenie po wierzcholkach
//potem posortowanie sortem i dac to waga do wartosci wtfff to nie zadziala
//ma sens tylko jesli zależności nie tworzą cyklu
//pozbywanie sie wierzcholka jakiegos
//trzeba wybrac ktory podzbior sie bardziej oplaca, wiec gdyby to bylo zachlanne to wtedy by sie bralo to co chwilowo ma lepszy ten wspolczynnik wartosc do wagi

