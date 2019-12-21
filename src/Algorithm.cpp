//
// Created by carlos on 26/04/18.
//

#include "../include/Algorithm.h"


void Algorithm::defaultEvolvePopulation(Population *population, int newInd) {
    int selected, toMutation = int((population->getSize()) * 0.2);

    // Crossover to make new individuals
    for (int i = 0; i < newInd; i++)
        population->appendIndividual(defaultCrossover(tournament(population), tournament(population)));

    for (int i = 0; i < toMutation; i++) {
        selected = int(random() % population->getSize());
        if (selected != 0) defaultMutation(population->getIndividual(selected));
    }

    population->setIndividual(0, population->getFittest());
    population->getOnly(population->getSize(), newInd);
}

void Algorithm::constrainedEvolvePopulation(class Population *population, int newInd) {
    int selected, toMutation = int((population->getSize()) * 0.2);

    // Crossover to make new individuals
    for (int i = 0; i < newInd; i++)
        population->appendIndividual(defaultCrossover(tournament(population), tournament(population)));

    for (int i = 0; i < toMutation; i++) {
        selected = int(random() % population->getSize());
        if (selected != 0)
            constrainedMutation(population->getIndividual(selected));
    }

    population->setIndividual(0, population->getFittest());
    population->getOnly(population->getSize(), newInd);
}

void Algorithm::hybridEvolvePopulation(class Population *population, int newInd) {
    int selected, toMutation = int((population->getSize()) * 0.25);

    // Crossover
    for (int i = 0; i < newInd; i++)
        population->appendIndividual(defaultCrossover(tournament(population), tournament(population)));

    // Mutation
    for (int i = 0; i < toMutation; i++) {
        selected = int(random() % population->getSize());
        if (selected != 0) defaultMutation(population->getIndividual(selected));
    }

    // Constrained Mutation
    for (int i = 0; i < toMutation; i++) {
        selected = int(random() % population->getSize());
        if (selected != 0) constrainedMutation(population->getIndividual(selected));
    }

    // Eletism on the best individual
    population->setIndividual(0, population->getFittest());

    // Remove the excess
    population->getOnly(population->getSize(), newInd);

}

Individual *Algorithm::defaultCrossover(Individual *individual1, Individual *individual2) {
    auto *child = new Individual((individual1->getFitness() > individual2->getFitness()) ? *individual1 : *individual2);

    for (int i = 0; i < individual1->getSize(); i++) {
        if (child->getGene(i) == individual2->getGene(i)) continue;
        else if (drand48() < uniformRate) child->setGene(i);
    }
    return child;
}

void Algorithm::defaultMutation(Individual *individual) {
    for (int i = 0; i < individual->getSize(); i++)
        if (individual->getChg(i) >= 0 || drand48() < mutationRate) individual->setGene(i);
}

void Algorithm::constrainedMutation(Individual *individual) {
    int fixed = -1, n = individual->getSize();
    bool fixVertex;

    fixVertex = drand48() < 0.6;
    if (fixVertex) fixed = int(random() % n);

    individual->constrainedMutation(fixed);
}

Individual *Algorithm::tournament(Population *population) {
    auto *individual = new Individual(nullptr, false, false);
    int selected = 0;

    for (int i = 0; i < tournamentSize; i++) {
        selected = int(random() % population->getSize());
        if (individual->getFitness() < population->getIndividual(selected)->getFitness()) {
            individual = population->getIndividual(selected);
        }
    }

    return individual;
}
