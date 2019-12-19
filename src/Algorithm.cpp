//
// Created by carlos on 26/04/18.
//

#include "../include/Algorithm.h"


void Algorithm::defaultEvolvePopulation(Population *population, int newInd) {
    population->setIndividual(0, population->getFittest());

    // Crossover to make new individuals
    for (int i = 0; i < newInd; i++)
        population->appendIndividual(defaultCrossover(tournament(population), tournament(population)));

    for (int i = 1; i < population->getSize(); i++)
        defaultMutation(population->getIndividual(i));

    population->getOnly(population->getSize() - newInd);
}

void Algorithm::constrainedEvolvePopulation(class Population *population, int newInd) {
    population->setIndividual(0, population->getFittest());

    // Crossover to make new individuals
    for (int i = 0; i < newInd; i++)
        population->appendIndividual(defaultCrossover(tournament(population), tournament(population)));

    for (int i = 1; i < population->getSize(); i++)
        constrainedMutation(population->getIndividual(i));

    population->getOnly(population->getSize() - newInd);
}

void Algorithm::hybridEvolvePopulation(class Population *population, int newInd) {
    population->setIndividual(0, population->getFittest());

    // Crossover to make new individuals
    for (int i = 0; i < newInd; i++)
        population->appendIndividual(defaultCrossover(tournament(population), tournament(population)));

    for (int i = 1; i < population->getSize(); i++)
        defaultMutation(population->getIndividual(i));

    for (int i = 1; i < population->getSize(); i++)
        if (drand48() < mutationRate / 2)
            constrainedMutation(population->getIndividual(i));

    population->getOnly(population->getSize() - newInd);
}

Individual *Algorithm::defaultCrossover(Individual *individual1, Individual *individual2) {
    auto *child = new Individual(*individual1);

    for (int i = 0; i < individual1->getSize(); i++) {
        if (child->getGene(i) == individual2->getGene(i)) continue;
        if (drand48() >= uniformRate) child->setGene(i);
    }
    return child;
}

void Algorithm::defaultMutation(Individual *individual) {
    for (int i = 0; i < individual->getSize(); i++)
        if (drand48() <= mutationRate) individual->setGene(i);
}

void Algorithm::constrainedMutation(Individual *individual) {
    int fixed = -1, n = individual->getSize();
    bool fixVertex;

    fixVertex = drand48() < 0.5;
    if (fixVertex) fixed = int(random() % n);

    individual->constrainedMutation(fixed);
}

Individual *Algorithm::tournament(Population *population) {
    auto *tournament = new Population(tournamentSize);

    for (int i = 0; i < population->getSize() && tournament->getSize() < tournamentSize; i++)
        if (drand48() <= 0.4) tournament->appendIndividual(population->getIndividual(i));

    if (!tournament->getSize())
        return population->getIndividual(population->getSize() - 1);

    return tournament->getFittest();
}
