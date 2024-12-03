#include "../include/define_include.h"

std::vector <int> topologicalSort(graph *G, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies);

bool isCyclic(graph *G);

void createGraph(graph *G, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies);