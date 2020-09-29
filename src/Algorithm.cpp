//
// Created by carlos on 26/04/18.
//

#include "../include/Algorithm.h"

void Algorithm::defaultEvolvePopulation(Population *population, int newInd) {
  int selected, toMutation = int((population->getSize()) * 0.3), i;

  // Crossover to make new individuals
  for (i = 0; i < newInd; i++)
    population->appendIndividual(defaultCrossover(tournament(population), tournament(population)));

  //    for (i = 1; i < population->getSize(); i++)
  //        cout << population->getIndividual(i)->getFitness() << ", ";
  //    cout << endl;

  // Optimization
  for (i = 0; i < population->getSize() - 1; i++) {
    localOptimization(population->getIndividual(i));
  }

  for (i = 0; i < population->getSize() - 1; i++)
    cout << population->getIndividual(i)->getFitness() << endl;

  getchar();

  for (i = 0; i < toMutation; i++) {
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
  int selected, toMutation = int((population->getSize()) * 0.3);
  int i;
  // Crossover
  for (i = 0; i < newInd; i++)
    population->appendIndividual(multiParentCrossover(population, 2 + int(random() % 2)));
  //population->appendIndividual(defaultCrossover(tournament(population), tournament(population)));
  // for (int i = population->getSize()  - newInd; i < population->getSize(); i++)
  //   cout << population->getIndividual(i)->getFitness() << ", ";
  // cout << endl;
  // getchar();
  // Optimization
  for (i = population->getSize() - newInd; i < population->getSize(); i++) {
    localOptimization(population->getIndividual(i));
  }
  for (i = 0; i < toMutation; i++) {
    selected = int(random() % population->getSize());
    if (selected != 0) constrainedMutation(population->getIndividual(selected));
  }

  for (i = 0; i < toMutation; i++) {
    selected = int(random() % population->getSize());
    if (selected != 0) defaultMutation(population->getIndividual(selected));
  }

  // Eletism on the best individual
  population->setIndividual(0, population->getFittest());

  // Remove the excess
  population->getOnly(population->getSize(), newInd);
  //getchar();
}

Individual *Algorithm::defaultCrossover(Individual *individual1, Individual *individual2) {
  auto *child = new Individual(
			       (individual1->getFitness() > individual2->getFitness()) ? *individual1 : *individual2);

  for (int i = 0; i < individual1->getSize(); i++) {
    if (child->getGene(i) == individual2->getGene(i)) continue;
    else if (drand48() < uniformRate) child->setGene(i);
  }
  return child;
}

Individual *Algorithm::multiParentCrossover(Population *population, int p) {
  vector<vector<int>> sideZero = vector<vector<int>>(p, vector<int>());
  vector<vector<int>> sideOne = vector<vector<int>>(p, vector<int>());
  int i, j, k, n = population->getIndividual(0)->getSize();
  vector<int> selecteds = vector<int>();
  vector<bool> allocated = vector<bool>(n);

  selecteds.push_back(int(random() % population->getSize()));
  for (k = 1; k < p; k++) {
    j = int(random() % population->getSize());
    //cout << j << " - " << find(selecteds.begin(), selecteds.end(), j) != selecteds.end() << endl;
    if (find(selecteds.begin(), selecteds.end(), j) != selecteds.end())
      selecteds.push_back(j);
    else k--;
  }
  
  for (i = 0; i < n; i++) {
    for (k = 0; k < p; k++) {
      if (population->getIndividual(selecteds[k])->getGene(i)) {
	sideOne[k].push_back(i);
      } else sideZero[k].push_back(i);
    }
  }

  int smallest = 0, smallestZero = 0, sizeSmallest = sideOne[0].size(), sizeSmallestZero = sideZero[0].size();

  for (k = 0; k < p; k++) {
    sort(sideZero[k].begin(), sideZero[k].end());
    sort(sideOne[k].begin(), sideOne[k].end());
    if (sideOne[k].size() < sizeSmallest)
      smallest = k, sizeSmallest = sideOne[k].size();
    if (sideZero[k].size() < sizeSmallestZero)
      smallestZero = k, sizeSmallestZero = sideZero[k].size();
  }

  Individual *ind = new Individual(population->getIndividual(smallest)->graph, false, false);

  bool change;
  for (i = 0; i < sizeSmallest; i++) {
    j = sideOne[smallest][i];
    change = true;
    for (k = 0; k < p; k++) {
      if (sideOne[k][i] != j) {
	change = false; break;
      }
    }
    if (change) ind->setGene(j), allocated[j] = true;
  }

  for (i = 0; i < sizeSmallestZero; i++) {
    j = sideZero[smallestZero][i];
    change = true;
    for (k = 0; k < p; k++) {
      if (sideZero[k][i] != j) {
	change = false; break;
      }
    }
    if (change) allocated[j] = true;
  }

  for (i = 0; i < n; i++) {
    if (!allocated[j])
      if (drand48() < 0.5)
	ind->setGene(i);
  }

  return ind;  
}

void Algorithm::defaultMutation(Individual *individual) {
  for (int i = 0; i < individual->getSize(); i++)
    if (drand48() < mutationRate) individual->setGene(i);
}

void Algorithm::localOptimization(Individual *individual) {
  int j;
  for (int i = 0; i < (4 * individual->getSize()); i++) {
    j = int(random() % individual->getSize());
    if (individual->getChg(j) >= 0) individual->setGene(j);
  }
}

void Algorithm::constrainedMutation(Individual *individual) {
  int fixed = -1, n = individual->getSize();
  bool fixVertex;

  fixVertex = drand48() < 0.5;
  if (fixVertex) fixed = int(random() % n);

  individual->constrainedMutation(fixed);
}

Individual *Algorithm::tournament(Population *population) {
  int selected = 0, toReturn = population->getSize() - 1;
  double best = population->getIndividual(population->getSize() - 1)->getFitness();

  for (int i = 0; i < tournamentSize; i++) {
    selected = int(random() % population->getSize());
    if (best < population->getIndividual(selected)->getFitness()) {
      toReturn = selected;
      best = population->getIndividual(selected)->getFitness();
    }
  }

  return population->getIndividual(toReturn);
}
