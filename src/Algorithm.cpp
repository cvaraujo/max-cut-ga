//
// Created by carlos on 26/04/18.
//

#include "../include/Algorithm.h"

void Algorithm::defaultEvolvePopulation(Population *population, int newInd) {
  int selected, toMutation = int((population->getSize()) * 0.3);
  int i, j, numIns = 0, p = population->getSize()-1;
  double worst = population->getIndividual(p)->getFitness(), newFitness;
  bool push;
  vector<int> used;
  vector<int>::iterator it;

  for (i = 0; i < newInd; i++) {
    Individual *ind = multiParentCrossover(population, 2 + int(random() % 3));
    newFitness = ind->getFitness();

    if (newFitness > worst) {
      push = true;
      for (j = p; j >= 0; j--){
        if (newFitness == population->getIndividual(j)->getFitness()) {
          push = false;
          break;
        }
      }
      if(push) {
        population->appendIndividual(ind);
        numIns++;
      }
    }
  }

  // Optimization
  for (i = 0; i < newInd; i++) {
    selected = 1 + int(random() % (population->getSize()-1));
    it = find(used.begin(), used.end(), selected);

    if(it != used.end()) i--;
    else {
      used.push_back(selected);
      localOptimization(population->getIndividual(selected));
    }
  }

  for (i = 0; i < toMutation; i++) {
    selected = 1 + int(random() % (population->getSize()-1));
    it = find(used.begin(), used.end(), selected);

    if(it != used.end()) i--;
    else {
      used.push_back(selected);
      defaultMutation(population->getIndividual(selected));
    }
  }

  population->getOnly(population->getSize(), numIns);
}

void Algorithm::constrainedEvolvePopulation(class Population *population, int newInd) {

  int selected, toMutation = int((population->getSize()) * 0.3);
  int i, j, numIns = 0, p = population->getSize()-1;
  double worst = population->getIndividual(p)->getFitness(), newFitness;
  bool push;
  vector<int> used;
  vector<int>::iterator it;

  for (i = 0; i < newInd; i++) {
    Individual *ind = multiParentCrossover(population, 2 + int(random() % 3));
    newFitness = ind->getFitness();

    if (newFitness > worst) {
      push = true;
      for (j = p; j >= 0; j--){
        if (newFitness == population->getIndividual(j)->getFitness()) {
          push = false;
          break;
        }
      }
      if(push) {
        population->appendIndividual(ind);
        numIns++;
      }
    }
  }

  for (int i = 0; i < toMutation; i++) {
    selected = 1 + int(random() % (population->getSize()-1));
    it = find(used.begin(), used.end(), selected);

    if(it != used.end()) i--;
    else {
      used.push_back(selected);
      constrainedMutation(population->getIndividual(selected));
    }
  }
  population->getOnly(population->getSize(), newInd);
}

void Algorithm::hybridEvolvePopulation(class Population *population, int newInd) {
  int selected, toMutation = int((population->getSize()) * 0.3);
  int i, j, numIns = 0, p = population->getSize()-1;
  double worst = population->getIndividual(p)->getFitness(), newFitness;
  bool push;
  vector<int> used;

  for (i = 0; i < newInd; i++) {
    Individual *ind = multiParentCrossover(population, 2 + int(random() % 3));
    newFitness = ind->getFitness();

    if (newFitness > worst) {
      push = true;
      for (j = p; j >= 0; j--){
        if (newFitness == population->getIndividual(j)->getFitness()) {
          push = false;
          break;
        }
      }
      if(push) {
        population->appendIndividual(ind);
        numIns++;
      }
    }
  }

  vector<int>::iterator it;

  for (i = 0; i < 2*toMutation; i++) {
    selected = 1 + int(random() % (population->getSize()-1));
    it = find(used.begin(), used.end(), selected);

    if(it != used.end()) i--;
    else {
      used.push_back(selected);
      if(drand48() < 0.5)
        constrainedMutation(population->getIndividual(selected));
      else
        localOptimization(population->getIndividual(selected));
    }
  }

  for (i = 0; i < toMutation; i++) {
    selected = 1 + int(random() % (population->getSize()-1));
    it = find(used.begin(), used.end(), selected);

    if(it != used.end()) i--;
    else {
      used.push_back(selected);
      defaultMutation(population->getIndividual(selected));
    }
  }

  // Remove the excess
  population->getOnly(population->getSize(), numIns);
}

Individual *Algorithm::defaultCrossover(Individual *individual1, Individual *individual2) {
  auto *child = new Individual((individual1->getFitness() > individual2->getFitness()) ? *individual1 : *individual2);

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
  int first = 1 + int(random() % int(population->getSize() * 0.1));

  selecteds.push_back(first);
  selecteds.push_back(population->getSize()-1);

  for (k = first+1; k < p-1; k++)
    selecteds.push_back(k);

  for (i = 0; i < n; i++)
    for (k = 0; k < p; k++)
      if (population->getIndividual(selecteds[k])->getGene(i)) sideOne[k].push_back(i);
      else sideZero[k].push_back(i);

  int smallest = 0, smallestZero = 0, sizeSmallest = sideOne[0].size(), sizeSmallestZero = sideZero[0].size();

  for (k = 0; k < p; k++) {
    sort(sideZero[k].begin(), sideZero[k].end());
    sort(sideOne[k].begin(), sideOne[k].end());
    if (sideOne[k].size() < sizeSmallest) smallest = k, sizeSmallest = sideOne[k].size();
    if (sideZero[k].size() < sizeSmallestZero) smallestZero = k, sizeSmallestZero = sideZero[k].size();
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

  for (i = 0; i < n; i++)
    if (!allocated[j] && drand48() < 0.6) ind->setGene(i);

  return ind;
}

void Algorithm::defaultMutation(Individual *individual) {
  for (int i = 0; i < individual->getSize(); i++)
    if (drand48() < mutationRate) individual->setGene(i);
}

void Algorithm::localOptimization(Individual *individual) {
  int j, i;
  vector<pair<int, int>> order;

  for(i = 0; i < individual->getSize(); i++)
    order.push_back(make_pair(i, individual->getChg(i)));

  sort(order.begin(), order.end(), [] (const pair<int, int> P1, const pair<int, int> P2) {
                                     return P1.second > P2.second;
                                   });

  for (i = 0; i < individual->getSize(); i++) {
    if (individual->getChg(order[i].first) < 0) continue;
    individual->setGene(order[i].first);
  }
}

void Algorithm::constrainedMutation(Individual *individual) {
  int fixed = int(random() % individual->getSize());
  individual->constrainedMutation(fixed);
}

Individual *Algorithm::tournament(Population *population) {
  int selected = 0, toReturn = population->getSize() - 1;
  double best = population->getIndividual(toReturn)->getFitness();

  for (int i = 0; i < tournamentSize; i++) {
    selected = int(random() % population->getSize());
    if (best < population->getIndividual(selected)->getFitness()) {
      toReturn = selected;
      best = population->getIndividual(selected)->getFitness();
    }
  }

  return population->getIndividual(toReturn);
}
