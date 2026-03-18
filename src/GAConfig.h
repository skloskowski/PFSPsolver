#ifndef PFSPSOLVER_GACONFIG_H
#define PFSPSOLVER_GACONFIG_H
#include <string>

enum class MutationType {
    Swap,
    Inversion
};

enum class CrossoverType {
    OX,
    PMX,
    CX
};

enum class SelectionType {
    Tournament,
    Roulette
};

struct GAConfig {
    int populationSize;
    int generations;

    double mutationRate;
    double crossoverRate;

    MutationType mutation;
    CrossoverType crossover;
    SelectionType selection;

    bool elitism;
    double elitismRate;
};

GAConfig loadConfig(const std::string& filename);
void printConfig(const GAConfig& config);

std::string toString(MutationType type);
std::string toString(CrossoverType type);
std::string toString(SelectionType type);

#endif //PFSPSOLVER_GACONFIG_H