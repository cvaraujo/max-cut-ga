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

void Population::initPopConstrained(Graph *graph) {
  for (int i = 0; i < this->populationSize; i++) {
    population.push_back(new Individual(graph, true, false));
  }
}

void Population::initPopMixed(Graph *graph, int numConst) {
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
  sort(population.begin(), population.end(), [](Individual *i1, Individual *i2) { return i1->getFitness() > i2->getFitness(); });

  Individual *ind = new Individual(*population[0]);

  // cout << ind->getFitness() << endl;

  population.erase(population.begin(), population.begin()+newInd-1);
  population.erase(population.begin() + (popSize - newInd), population.end());

  population[0] = ind;

  for(int i = 0; i < newInd; i++)
    population.push_back(new Individual(population[1]->graph, true, true));

  // cout << population[0]->getFitness() << endl;
  // getchar();
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
