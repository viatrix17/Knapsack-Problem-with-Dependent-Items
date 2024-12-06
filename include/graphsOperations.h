#include "../include/define_include.h"

std::vector <int> topologicalSort(int size, graph *G, const std::vector<item> items);

void isCyclic(graph *G, std::set<std::set<item>> &cycles);

void createGraph(graph *G, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies);

graph *transformGraph(graph *oldG, std::set<std::set<item>> &cycles);