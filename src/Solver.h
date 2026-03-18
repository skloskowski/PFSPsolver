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
    void applyMutation();
    void applyCrossover();
    void applySelection();

    // mutations
    void swapMutation();
    void inversionMutation();

    // crossovers
    void OXCrossover();
    void PMXCrossover();
    void CXCrossover();

    // selections
    void tournamentSelection();
    void roulleteSelection();
};


#endif //PFSPSOLVER_SOLVER_H