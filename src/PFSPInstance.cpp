#include "PFSPInstance.h"
#include <fstream>
#include <iostream>

PFSPInstance::PFSPInstance() {
    jobs = 0;
    machines = 0;
    seed = 0;
    upperBound = 0;
    lowerBound = 0;
}

bool PFSPInstance::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "Error opening file: " << filename << "\n";
        return false;
    }

    std::string line;

    std::getline(file, line);
    file >> jobs >> machines >> seed >> upperBound >> lowerBound;

    std::getline(file, line);
    std::getline(file, line);

    processingTimes.resize(machines, std::vector<int>(jobs));

    for (int m = 0; m < machines; m++)
    {
        for (int j = 0; j < jobs; j++)
        {
            file >> processingTimes[m][j];
        }
    }

    return true;
}

int PFSPInstance::getJobs() const {
    return jobs;
}

int PFSPInstance::getMachines() const {
    return machines;
}

int PFSPInstance::getProcessingTime(int machine, int job) const {
    return processingTimes[machine][job];
}

void PFSPInstance::printInstance() const {

    for (int m = 0; m < machines; m++)
    {
        std::cout << "Machine " << m << ": ";
        for (int j = 0; j < jobs; j++)
        {
            std::cout << processingTimes[m][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << std::endl;
}

void PFSPInstance::printInstanceInfo() const {

    std::cout << "Jobs: " << jobs << std::endl;
    std::cout << "Machines: " << machines << std::endl;
    std::cout << "Lower Bound: " << lowerBound << std::endl;
    std::cout << "Upper Bound: " << upperBound << std::endl << std::endl;
}

int PFSPInstance::computeMakespan(const std::vector<int>& perm) const {
    int m = machines;
    int n = perm.size();

    std::vector machineTime(m, 0);

    for(int idx=0; idx<n; idx++)
    {
        int job = perm[idx];
        for(int machine=0; machine<m; machine++)
        {
            if(machine == 0)
                machineTime[machine] += processingTimes[machine][job];
            else
                machineTime[machine] = std::max(machineTime[machine], machineTime[machine-1]) + processingTimes[machine][job];
        }
    }

    return machineTime[m-1];
}
