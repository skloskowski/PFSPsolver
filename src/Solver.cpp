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

    int n = instance.getJobs();
    int popSize = config.populationSize;

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> prob(0.0, 1.0);

    // --- init pop ---
    std::vector<std::vector<int>> population(popSize, std::vector<int>(n));

    for (int i = 0; i < popSize; i++)
    {
        for (int j = 0; j < n; j++)
            population[i][j] = j;

        std::shuffle(population[i].begin(), population[i].end(), gen);
    }

    std::vector<int> fitness(popSize);

    std::vector<int> bestSolution;
    int bestMakespan = INT_MAX;

    // --- main GA loop ---
    for (int genIdx = 0; genIdx < config.generations; genIdx++)
    {
        // eval pop
        for (int i = 0; i < popSize; i++)
        {
            fitness[i] = instance.computeMakespan(population[i]);

            if (fitness[i] < bestMakespan)
            {
                bestMakespan = fitness[i];
                bestSolution = population[i];
            }
        }

        std::vector<std::vector<int>> newPopulation;

        // --- elitism ---
        if (config.elitism)
        {
            int eliteCount = static_cast<int>(config.elitismRate * popSize);

            // sort by fitness
            std::vector<int> indices(popSize);
            for (int i = 0; i < popSize; i++) indices[i] = i;

            std::sort(indices.begin(), indices.end(),
                      [&](int a, int b) { return fitness[a] < fitness[b]; });

            for (int i = 0; i < eliteCount; i++)
                newPopulation.push_back(population[indices[i]]);
        }

        // --- gen new pop ---
        while (newPopulation.size() < popSize)
        {
            int p1 = applySelection(fitness, config.selection);
            int p2 = applySelection(fitness, config.selection);

            auto [child1, child2] = applyCrossover(
                population[p1],
                population[p2],
                config.crossover
            );

            // mutation
            if (prob(gen) < config.mutationRate)
                applyMutation(child1, config.mutation);

            if (prob(gen) < config.mutationRate)
                applyMutation(child2, config.mutation);

            newPopulation.push_back(child1);

            if (newPopulation.size() < popSize)
                newPopulation.push_back(child2);
        }

        population = newPopulation;
    }

    return bestSolution;
}

std::vector<int> Solver::solveSimulatedAnnealing(const PFSPInstance& instance, int iterations, double temp, double cooling) {
    int n = instance.getJobs();
    std::vector<int> solution(n);
    std::vector<int> bestSolution(n);

    for (int i = 0; i < n; i++) solution[i] = i;

    std::mt19937 g(std::random_device{}());
    std::ranges::shuffle(solution, g);

    bestSolution = solution;

    int currentMakespan = instance.computeMakespan(bestSolution);
    int bestMakespan = currentMakespan;

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

        temp *= cooling;
    }

    return bestSolution;
}

void Solver::applyMutation(std::vector<int>& perm, MutationType type) {
    int n = perm.size();
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, n - 1);

    int i = dist(gen);
    int j = dist(gen);

    if (i > j) std::swap(i, j);

    switch(type)
    {
        case MutationType::Swap:
            std::swap(perm[i], perm[j]);
            break;

        case MutationType::Inversion:
            std::reverse(perm.begin() + i, perm.begin() + j + 1);
            break;
    }
}

std::pair<std::vector<int>, std::vector<int>> Solver::applyCrossover(const std::vector<int>& p1, const std::vector<int>& p2, CrossoverType type) {
    switch(type)
    {
        case CrossoverType::OX:
            return OXCrossover(p1, p2);

        case CrossoverType::PMX:
            return PMXCrossover(p1, p2);

        case CrossoverType::CX:
            return CXCrossover(p1, p2);
    }

    return {p1, p2};
}

int Solver::applySelection(const std::vector<int>& fitness,
                           SelectionType type)
{
    switch(type)
    {
        case SelectionType::Tournament:
            return tournamentSelection(fitness);

        case SelectionType::Roulette:
            return rouletteSelection(fitness);
    }

    return 0;
}

std::pair<std::vector<int>, std::vector<int>> Solver::OXCrossover(const std::vector<int> &p1, const std::vector<int> &p2) {
    int n = p1.size();

    std::vector<int> c1(n, -1);
    std::vector<int> c2(n, -1);

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, n - 1);

    int l = dist(gen);
    int r = dist(gen);
    if (l > r) std::swap(l, r);

    for (int i = l; i <= r; i++)
    {
        c1[i] = p1[i];
        c2[i] = p2[i];
    }

    int idx = (r + 1) % n;

    for (int i = 0; i < n; i++)
    {
        int val = p2[(r + 1 + i) % n];

        if (std::find(c1.begin(), c1.end(), val) == c1.end())
        {
            c1[idx] = val;
            idx = (idx + 1) % n;
        }
    }

    idx = (r + 1) % n;

    for (int i = 0; i < n; i++)
    {
        int val = p1[(r + 1 + i) % n];

        if (std::find(c2.begin(), c2.end(), val) == c2.end())
        {
            c2[idx] = val;
            idx = (idx + 1) % n;
        }
    }

    return {c1, c2};

}

std::pair<std::vector<int>, std::vector<int>> Solver::PMXCrossover(const std::vector<int> &p1, const std::vector<int> &p2) {
}

std::pair<std::vector<int>, std::vector<int>> Solver::CXCrossover(const std::vector<int> &p1, const std::vector<int> &p2) {
}

int Solver::tournamentSelection(const std::vector<int> &fitness) {
    std::mt19937 g(std::random_device{}());
    std::uniform_int_distribution<> dist(0, fitness.size() - 1);

    int k = 5;
    int best = dist(g);

    for (int i = 0; i < k; i++) {
        int candidate = dist(g);
        if (fitness[candidate] < fitness[best]) {
            best = candidate;
        }
    }

    return best;
}

int Solver::rouletteSelection(const std::vector<int> &fitness) {

    static std::mt19937 gen(std::random_device{}());

    std::vector<double> prob(fitness.size());
    double sum = 0;

    for(size_t i = 0; i < fitness.size(); i++)
    {
        prob[i] = 1.0 / fitness[i];
        sum += prob[i];
    }

    std::uniform_real_distribution<> dist(0, sum);
    double r = dist(gen);

    double acc = 0;
    for(size_t i = 0; i < prob.size(); i++) {
        acc += prob[i];
        if(acc >= r)
            return i;
    }

    return fitness.size() - 1;
}