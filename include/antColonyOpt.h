#include "define_include.h"

Result antAlgorithm(const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies, int ants, int iterations, int alfa, int beta, double evaporationRate);

bool allPrevsAdded(std::vector<int> vertice, std::vector<bool> added);