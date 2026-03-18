#include <iostream>
#include <vector>
#include <random>
#include "PFSPInstance.h"
#include "Solver.h"

int main()
{
    PFSPInstance instance;
    if(!instance.loadFromFile("../PFSP/tai20_5_0.fsp")) return 1;

    instance.printInstanceInfo();
    if (instance.getJobs() <= 20 && instance.getMachines() <= 20) instance.printInstance();

    // load config for GA
    GAConfig config = loadConfig("../PFSP/config.txt");

    Solver solver;

    //std::vector<int> result = solver.solveRandom(instance, 5000);
    //std::vector<int> result = solver.solveGreedy(instance);
    //std::vector<int> result = solver.solveSimulatedAnnealing(instance, 5000, 1000, 0.995);
    std::vector<int> result = solver.solveGeneticAlgorithm(instance, config);

    printConfig(config);

    std::cout << "Best perm: ";
    for(int job : result)
        std::cout << job << " ";
    std::cout << "\n";

    std::cout << "Makespan = " << instance.computeMakespan(result) << std::endl;

    return 0;
}
