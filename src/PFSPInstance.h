#ifndef PFSPSOLVER_PFSPINSTANCE_H
#define PFSPSOLVER_PFSPINSTANCE_H

#include <vector>
#include <string>

class PFSPInstance
{
private:
    int jobs;
    int machines;
    int seed;
    int upperBound;
    int lowerBound;

    std::vector<std::vector<int>> processingTimes;

public:
    PFSPInstance();

    bool loadFromFile(const std::string& filename);

    int getJobs() const;
    int getMachines() const;
    int getProcessingTime(int machine, int job) const;

    void printInstance() const;

    void printInstanceInfo() const;

    int computeMakespan(const std::vector<int>& perm) const;
};


#endif //PFSPSOLVER_PFSPINSTANCE_H