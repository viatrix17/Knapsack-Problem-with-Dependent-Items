#include "../include/define_include.h"

Result bruteForce(int B, const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies);

Result bfCutsOff(const std::vector<item> items, const std::vector<std::pair<int, int>> dependencies, std::chrono::time_point<std::chrono::high_resolution_clock> startTime, std::chrono::time_point<std::chrono::high_resolution_clock> stopTime);
