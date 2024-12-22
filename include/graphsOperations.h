#include "../include/define_include.h"

std::vector <int> topologicalSort(int size, graph *G, const std::vector<item> items);

void isCyclic(graph *G, const std::vector<item> items, std::set<std::set<item>> &cycles);

void createGraph(graph *G, std::vector<std::vector<int>> &prevG, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies);

void antGraphCreate(antGraph *G, std::vector<std::vector<int>> &prevG, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies);

graph *transformGraph(graph *oldG, std::set<std::set<item>> &cycles);

void deleteCycles(graph *oldG, std::vector<std::vector<int>> &prevOld, std::set<std::set<item>> &cycles);

void show(graph *G, int size);

bool allPrevsAdded(std::vector<int> vertice, std::vector<bool> added);