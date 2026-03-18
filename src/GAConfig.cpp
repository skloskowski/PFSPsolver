#include "GAConfig.h"

#include <fstream>
#include <iostream>

GAConfig loadConfig(const std::string& filename)
{
    GAConfig cfg{};
    std::ifstream file(filename);
    std::string line;

    while(std::getline(file, line))
    {
        auto pos = line.find('=');
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos+1);

        if(key == "populationSize") cfg.populationSize = std::stoi(value);
        else if(key == "generations") cfg.generations = std::stoi(value);
        else if(key == "mutationRate") cfg.mutationRate = std::stod(value);
        else if(key == "crossoverRate") cfg.crossoverRate = std::stod(value);
        else if(key == "elitismRate") cfg.elitismRate = std::stod(value);

        else if(key == "mutation")
        {
            if(value == "swap") cfg.mutation = MutationType::Swap;
            else cfg.mutation = MutationType::Inversion;
        }

        else if(key == "crossover")
        {
            if(value == "ox") cfg.crossover = CrossoverType::OX;
            else if(value == "pmx") cfg.crossover = CrossoverType::PMX;
            else cfg.crossover = CrossoverType::CX;
        }

        else if(key == "selection")
        {
            if(value == "tournament") cfg.selection = SelectionType::Tournament;
            else cfg.selection = SelectionType::Roulette;
        }

        else if(key == "elitism")
        {
            cfg.elitism = (value == "true");
        }
    }

    return cfg;
}

std::string toString(MutationType type)
{
    switch(type)
    {
        case MutationType::Swap: return "Swap";
        case MutationType::Inversion: return "Inversion";
    }
    return "Unknown";
}

std::string toString(CrossoverType type)
{
    switch(type)
    {
        case CrossoverType::OX: return "OX";
        case CrossoverType::PMX: return "PMX";
        case CrossoverType::CX: return "CX";
    }
    return "Unknown";
}

std::string toString(SelectionType type)
{
    switch(type)
    {
        case SelectionType::Tournament: return "Tournament";
        case SelectionType::Roulette: return "Roulette";
    }
    return "Unknown";
}

void printConfig(const GAConfig& cfg)
{
    std::cout << "Population:\t" << cfg.populationSize << "\n";
    std::cout << "Generations:\t" << cfg.generations << "\n";
    std::cout << "Selection:\t" << toString(cfg.selection) << "\n";
    std::cout << "Mutation:\t" << toString(cfg.mutation) << "\tRate: " << cfg.mutationRate << "\n";
    std::cout << "Crossover:\t" << toString(cfg.crossover) << "\tRate: " << cfg.crossoverRate << "\n";
    std::cout << "Elitism:\t" << (cfg.elitism ? "True" : "False") << "\tRate: " << cfg.elitismRate << "\n";

}