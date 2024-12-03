#include "../include/define_include.h"

extern int N; //liczba wierzchołków

void createGraph(graph *L, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies) {

}

bool isCyclic(graph *L) {

}

std::vector<int> topologicalSort (graph *L, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies) { //jak zroebic zeby nie zmienialo wartosci wskaznik hmm??
    std::vector <int> sortedGraph;

    int *ins, *visited;
    ins = new int[N]; visited = new int[N];

    for(int i = 0; i < N; i++){
        ins[i] = 0;
        visited[i] = 0;
    }
    //kopia listy idk czy potrzebna, może nie jest potrzebna
    int ins_count = 0;
    graph *L_copy; L_copy = new graph[N];
    for (int i = 0; i < N; i++) {
        for(std::list<int>::iterator it = L[i].next.begin(); it != L[i].next.end(); it++) {
            L_copy[i].next.push_back(*it);
            ins[*it]++;
            ins_count++;
        }
    }

//trzeba dodac zeby wybieralo ten z lepszym ilorazem
    int a, j, check = 1; 
    while (ins_count > 0) {
        for (int b = 0; b < N; b++) { //sprawdzanie cykliczności
            if (ins[b] == 0) {
                check = 0;
            }
        }
        if (check) {
            std::cout << "The graph is cyclic, can't be sorted.\n"; 
            return;
        }
       
        //ma brac najwiekszy po ilorazie z tych nieodwiedzonych, gdzie ins == 0
        auto indexRemove = std::max_element(items.begin(), items.end(), [&visited, &items](const item& a, const item& b) {
            int indexA = &a - &items[0];
            int indexB = &b - &items[0];
        
        // Oblicz iloraz wartości do wagi tylko dla przedmiotów nieodwiedzonych
            if (visited[indexA]) return false;
            if (visited[indexB]) return true;

            return (static_cast<double>(a.value) / a.weight) < (static_cast<double>(b.value) / b.weight);
        });

        if (indexRemove != items.end()) {
            //itemRemove = indexRemove - items.begin(); 
            std::cout << indexRemove->number << "\n";
            while (!L_copy[indexRemove->number-1].next.empty()) {
                j = L_copy[indexRemove->number-1].next.front();
                L_copy[indexRemove->number-1].next.pop_front();
                ins[j]--;
                ins_count--;
            }
            visited[indexRemove->number-1] = 1;
            sortedGraph.push_back(indexRemove->number);
        }
    }

    //dodawanie do wyniku tych które są gdzieś tam osobno niepowiązane z niczym 
    for(int i = 0; i < N; i++){
        if (!visited[i]) {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";
    //std::cout << "The graph has been sorted successfully (Kahn algorithm).\n";
    delete[] ins; delete[] visited;
    return sortedGraph;  //zwraca permutacje
}


// MÓJ

// void Kahn_matrix(int **matrix, int vertices){
//     int ins[vertices], visited[vertices];
//     for(int i = 0; i < vertices; i++){
//         ins[i] = 0;
//         visited[i] = 0;
//     }
//     int ins_count = 0;
//     int matrix_copy[vertices][vertices];
//     for(int i = 0; i < vertices; i++){
//         for(int j = 0; j < vertices; j++){
//             matrix_copy[i][j] = matrix[i][j];
//             if(matrix_copy[i][j] == 1){
//                 ins[j]++; //poczatkowa ilosc wejsc
//                 ins_count++;
//             }
//         }
//     }

//     int a, check = 1;
//     while(ins_count > 0){
//         for(int b = 0; b < vertices; b++) if(ins[b] == 0) check = 0;
//         if(check == 1) {std::cout << "The graph is cyclic, can't be sorted.\n"; return;}
//         for(int i = 0; i < vertices; i++){
//             if(!visited[i] && ins[i] == 0){
//                 a = i;
//                 break;
//             }
//         }
//         std::cout << a << " ";
//         for(int j = 0; j < vertices; j++){
//             if(matrix_copy[a][j] == 1){
//                 matrix_copy[a][j] = 0;
//                 matrix_copy[j][a] = 0;
//                 ins[j]--;
//                 ins_count--;
//             }
//         }
//         visited[a] = 1;
//     }//znalezienie z zerowym wejściem
//     for(int i = 0; i < vertices; i++){
//         if(visited[i] == 0) std::cout << i << " ";
//     }
//     std::cout << "\n";
//     std::cout << "The graph has been sorted successfully (Kahn algorithm).\n";
// }




// WAŁASZEK
//  graf = new SLel * [n];
//   for(i = 0; i < n; i++) graf [i] = NULL;

//   // Odczytujemy definicje krawędzi grafu

//   for(i = 0; i < m; i++)
//   {
//     cin >> v1 >> v2;
//     ps = new SLel;
//     ps->v = v2;
//     ps->next = graf [v1];
//     graf [v1] = ps;
//   }

//   cout << endl;

//   // Wykonujemy sortowanie topologiczne grafu

//   Vind = new int [n];         // Tworzymy tablicę stopni wchodzących
//   for(i = 0; i < n; i++) Vind [i] = 0; // Zerujemy tablicę Vind []

//   for(i = 0; i < n; i++)      // Przeglądamy graf
//     for(ps = graf [i];ps;ps = ps->next) // Przeglądamy sąsiadów każdego wierzchołka
//       Vind [ps->v] ++;        // Wyznaczamy ich stopnie wchodzące

//   L = NULL;
//   for(i = n-1; i >= 0; i--) // Na liście L umieszczamy od 0 do n-1
//   {
//     pd = new DLel;
//     pd->v = i;
//     pd->next = L;
//     if(pd->next) pd->next->prev = pd;
//     pd->prev = NULL;
//     L = pd;
//   }

//   do
//   {
//     test = false;             // Oznaczamy brak usunięcia wierzchołka

//     pd = L;                   // Przeglądamy listę L
//     while(pd)
//       if(Vind [pd->v]) pd = pd->next; // Wierzchołki o niezerowym stopniu wchodzącym pomijamy
//       else
//       {
//         test = true;          // Będzie usunięcie wierzchołka
//         for(ps = graf [pd->v];ps;ps = ps->next) // Przeglądamy listę sąsiadów
//           Vind [ps->v] --;  // Modyfikujemy ich stopnie wchodzące

//         cout << pd->v << " "; // Wypisujemy usuwany wierzchołek
//         rd = pd;              // Zapamiętujemy adres elementu L
//         pd = pd->next;        // Następny wierzchołek na liście lub NULL!

//         // Zapamiętany element rd listy L usuwamy z pamięci

//         if(rd->next) rd->next->prev = rd->prev;
//         if(rd->prev) rd->prev->next = rd->next;
//         else L = pd;
//         delete rd;
//       }
//   } while(test);

//   cout << endl << endl;

//   // Usuwamy zmienne dynamiczne

//   delete [] Vind;

//   for(i = 0; i < n; i++)
//   {
//     ps = graf [i];
//     while(ps)
//     {
//       rs = ps;
//       ps = ps->next;
//       delete rs;
//     }
//   }
//   delete [] graf;

//   pd = L;
//   while(pd)
//   {
//     rd = pd;
//     pd = pd->next;
//     delete rd;
//   }