//
// Created by carlos on 26/04/18.
//

#ifndef PUREGENETICALGORITHM_POPULATION_H
#define PUREGENETICALGORITHM_POPULATION_H

#include "Includes.h"
#include "Individual.h"

class Population {
    vector<Individual *> population;
    int populationSize;

public:
    Population(int size);

    void reset(Graph *graph, int size);

    void initPopDefault(Graph *graph);

    void initPopConstrained(Graph *graph);

    void initPopMixed(Graph *graph, int numConst);

    Individual *getIndividual(int index);

    void setIndividual(int index, Individual *individual);

    void appendIndividual(Individual *individual);

    Individual *getFittest();

    void sortPopulation();

    void getOnly(int popSize, int newInd);

    void setSize(int size);

    int getSize();
};


#endif //PUREGENETICALGORITHM_POPULATION_H
