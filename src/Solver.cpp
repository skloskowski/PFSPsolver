#include "Solver.h"

#include <algorithm>
#include <random>

std::vector<int> Solver::solveRandom(const PFSPInstance& instance, int maxIterations) {

    // calculate current instance
    int bestMakespan = INT32_MAX;
    int bestLocal = INT32_MAX;
    std::vector<int> bestPerm(instance.getJobs());
    std::vector<int> currentPerm(instance.getJobs());

    for(int i=0; i<instance.getJobs(); i++) currentPerm[i] = i;
    bestPerm = currentPerm;
    bestMakespan = instance.computeMakespan(bestPerm);

    std::random_device rd;
    std::mt19937 g(rd());

    for (int i = 0; i < maxIterations; i++) {
        std::ranges::shuffle(currentPerm, g);
        bestLocal = instance.computeMakespan(currentPerm);

        if (bestMakespan > bestLocal) {
            bestMakespan = bestLocal;
            bestPerm = currentPerm;
        }
    }

    return bestPerm;
}

std::vector<int>  Solver::solveGreedy(const PFSPInstance& instance) {

    int n = instance.getJobs();

    std::vector<int> solution;
    std::vector<bool> used(n, false);

    for(int step = 0; step < n; step++)
    {
        int bestJob = -1;
        int bestMakespan = INT32_MAX;

        for(int j = 0; j < n; j++)
        {
            if(used[j]) continue;

            std::vector<int> temp = solution;
            temp.push_back(j);

            int makespan = instance.computeMakespan(temp);

            if(makespan < bestMakespan)
            {
                bestMakespan = makespan;
                bestJob = j;
            }
        }

        solution.push_back(bestJob);
        used[bestJob] = true;
    }

    return solution;

}

std::vector<int> Solver::solveGeneticAlgorithm(const PFSPInstance& instance, GAConfig config) {

    /*
     * MUTATION: SWAP, INVERSION
     * CROSSOVER: OX, PMX, CX
     * SELECTION: TOURNAMENT, ROULETTE
     * ADDITIONAL: ELITISM
     */

    return {};
}

std::vector<int> Solver::solveSimulatedAnnealing(const PFSPInstance& instance, int iterations, double temp, double cooling) {
    int n = instance.getJobs();
    std::vector<int> solution(n);
    std::vector<int> bestSolution(n);

    for (int i = 0; i < n; i++) solution[i] = i;
    bestSolution = solution;

    int currentMakespan = instance.computeMakespan(bestSolution);
    int bestMakespan = currentMakespan;

    std::random_device rd;
    std::mt19937 g(rd());

    std::uniform_real_distribution<double> prob(0, 1);
    std::uniform_int_distribution<int> dist(0, n-1);

    for (int i = 0; i < iterations; i++) {

        std::vector<int> neighbour = solution;
        // swap two jobs
        // could probably do inversion - have to ask
        std::swap(neighbour[dist(g)], neighbour[dist(g)]);

        int neighbourMakespan = instance.computeMakespan(neighbour);
        int delta = neighbourMakespan - currentMakespan;

        if (delta < 0 || prob(g) < std::exp(-delta / temp)) {

            solution = neighbour;
            currentMakespan = neighbourMakespan;

            if (currentMakespan < bestMakespan) {
                bestMakespan = currentMakespan;
                bestSolution = solution;
            }
        }
    }

    return bestSolution;
}