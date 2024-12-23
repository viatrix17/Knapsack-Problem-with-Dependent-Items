#include "define_include.h"

Result antAlgorithm(const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies, int ants, int iterations, int alfa, int beta, double evaporationRate, std::chrono::time_point<std::chrono::high_resolution_clock> &startTime, std::chrono::time_point<std::chrono::high_resolution_clock> &stopTime);
