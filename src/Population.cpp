//
// Created by carlos on 26/04/18.
//

#include "../include/Population.h"

Population::Population(int size) {
    this->populationSize = size;
}

void Population::initPopDefault(Graph *graph) {
    for (int i = 0; i < this->populationSize; i++)
        population.push_back(new Individual(graph, true, true));
}

void Population::initPopConstrained(class Graph *graph) {
    for (int i = 0; i < this->populationSize; i++) {
        population.push_back(new Individual(graph, true, false));
    }
}

void Population::initPopMixed(class Graph *graph, int numConst) {
    int numDefault = this->populationSize - numConst;

    for (int i = 0; i < numDefault; i++)
        population.push_back(new Individual(graph, true, true));

    for (int i = 0; i < numConst; i++)
        population.push_back(new Individual(graph, true, false));

}

void Population::appendIndividual(Individual *individual) {
    population.push_back(individual);
}

Individual *Population::getIndividual(int index) {
    return this->population[index];
}

void Population::sortPopulation() {
    sort(this->population.begin(), this->population.end(),
         [](Individual *i1, Individual *i2) { return i1->getFitness() > i2->getFitness(); });
}

void Population::setIndividual(int index, Individual *individual) {
    this->population[index] = individual;
}

Individual *Population::getFittest() {
    Individual *fittest = this->population[0];
    for (int i = 1; i < this->population.size(); i++)
        if (this->population[i] != nullptr && fittest->getFitness() < this->population[i]->getFitness())
            fittest = this->population[i];
    return fittest;
}

void Population::getOnly(int popSize, int newInd) {
    vector<Individual *> aux(population.begin() + (popSize - newInd), population.end());
    int i, countInserted = 0;

    sort(aux.begin(), aux.end(), [](Individual *i1, Individual *i2) { return i1->getFitness() > i2->getFitness(); });

    population.erase(population.begin() + (popSize - 2 * newInd), population.end());

    double lastFitness = 0;

    for (i = 0; i < int(aux.size()); i++) {
        if (aux[i]->getFitness() != lastFitness && aux[i]->getFitness() != aux[0]->getFitness()) {
            countInserted++, lastFitness = aux[i]->getFitness();
            population.push_back(aux[i]);
        }
    }

    for (i = 0; i < (newInd - countInserted); i++) {
        population.push_back(new Individual(population[0]->graph, true, true));
    }

}

int Population::getSize() {
    return int(this->population.size());
}

void Population::setSize(int size) {
    this->populationSize = size;
}

void Population::reset(Graph *graph, int size) {
    this->population.erase(population.begin() + 1, population.begin() + 1 + size);
    for (int i = 0; i < size; i++)
        population.push_back(new Individual(graph, true, true));
}