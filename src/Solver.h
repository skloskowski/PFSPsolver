#ifndef PFSPSOLVER_SOLVER_H
#define PFSPSOLVER_SOLVER_H

#include "GAConfig.h"
#include "PFSPInstance.h"


class Solver {
public:
    std::vector<int>  solveGreedy(const PFSPInstance& instance);
    std::vector<int>  solveRandom(const PFSPInstance& instance, int maxReps);
    std::vector<int>  solveSimulatedAnnealing(const PFSPInstance &instance, int iterations, double temp, double cooling);
    std::vector<int>  solveGeneticAlgorithm(const PFSPInstance& instance, GAConfig config);
private:

    // apply helper
    void applyMutation(std::vector<int>& perm, MutationType type);
    std::pair<std::vector<int>, std::vector<int>> applyCrossover(const std::vector<int>& p1,const std::vector<int>& p2, CrossoverType type);
    int applySelection(const std::vector<int>& fitness, SelectionType type);

    // crossovers
    std::pair<std::vector<int>, std::vector<int>> OXCrossover(const std::vector<int>& p1, const std::vector<int>& p2);
    std::pair<std::vector<int>, std::vector<int>> PMXCrossover(const std::vector<int>& p1, const std::vector<int>& p2);
    std::pair<std::vector<int>, std::vector<int>> CXCrossover(const std::vector<int>& p1, const std::vector<int>& p2);

    // selections
    int tournamentSelection(const std::vector<int>& fitness);
    int rouletteSelection(const std::vector<int>& fitness);
};


#endif //PFSPSOLVER_SOLVER_H