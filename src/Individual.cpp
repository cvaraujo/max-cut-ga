//
// Created by carlos on 26/04/18.
//

#include "../include/Individual.h"

Individual::Individual(Graph *graph, bool initialSolution, bool isDefault) {
    if (graph != nullptr) {
        this->fitness = 0.0;
        this->graph = graph;
        int n = graph->getN();

        chg = vector<int>(n);
        side = vector<bool>(n);
        gene = vector<bool>(n);
        cx = vector<int>(n);

        for (int i = 0; i < graph->getN(); i++) {
            chg[i] = graph->getChg(i), gene[i] = false, side[i] = false, cx[i] = 0;
        }

        if (initialSolution) {
            if (isDefault) defaultSolution();
            else constrainedSolution();
        }
    } else {
        this->fitness = numeric_limits<int>::min();
    }
}

void Individual::constrainedSolution() {
    int i, j, n = graph->getN(), fixed = int(random() % n);
    double value = 0;
    bool hasChange;

    do {
        hasChange = false;

        for (i = 0; i < n; i++) {
            if (i != fixed) {
                if (cx[i] < (graph->getC(i) / 2) && gene[i] == 0) {
                    // gene[i] have to be 1
                    gene[i] = true, side[i] = true, hasChange = true;
                    value += chg[i], chg[i] = -chg[i];
                    for (auto e : graph->graph[i]) {
                        j = e.first;
                        cx[j] += e.second;
                        chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
                    }
                } else if (cx[i] > (graph->getC(i) / 2) && gene[i] == 1) {
                    // gene[i] have to be 0
                    gene[i] = false, side[i] = false, hasChange = true;
                    value += chg[i], chg[i] = -chg[i];
                    for (auto e : graph->graph[i]) {
                        j = e.first;
                        cx[j] -= e.second;
                        chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
                    }
                }
            }
        }
    } while (hasChange);

    this->fitness = value;
//    checkConstraints();
}

void Individual::defaultSolution() {
    int n = graph->getN(), j;
    double value = 0.0;

    for (int i = 0; i < n; i++) {
        if (drand48() <= 0.5) {
            gene[i] = true, side[i] = true;
            value += chg[i], chg[i] = -chg[i];
            for (auto e : graph->graph[i]) {
                j = e.first;
                cx[j] += e.second;
                chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
            }
        }
    }
    this->fitness = value;
}

int Individual::getGene(int i) const {
    return this->gene[i];
}

void Individual::setGene(int i) {
    int j;
    gene[i] = !gene[i], side[i] = !side[i];
    fitness += chg[i], chg[i] = -chg[i];
    for (auto e : graph->graph[i]) {
        j = e.first;
        if (gene[i]) cx[j] += e.second;
        else cx[j] -= e.second;
        chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
    }
}

void Individual::constrainedMutation(int fixed) {
    int i, j, n = graph->getN();
    double value = this->fitness;
    bool hasChange;

    do {
        hasChange = false;

        for (i = 0; i < n; i++) {
            if (i != fixed) {
                if (cx[i] < (graph->getC(i) / 2) && gene[i] == 0) {
                    // gene[i] have to be 1
                    gene[i] = true, side[i] = true, hasChange = true;
                    value += chg[i], chg[i] = -chg[i];
                    for (auto e : graph->graph[i]) {
                        j = e.first;
                        cx[j] += e.second;
                        chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
                    }
                } else if (cx[i] > (graph->getC(i) / 2) && gene[i] == 1) {
                    // gene[i] have to be 0
                    gene[i] = false, side[i] = false, hasChange = true;
                    value += chg[i], chg[i] = -chg[i];
                    for (auto e : graph->graph[i]) {
                        j = e.first;
                        cx[j] -= e.second;
                        chg[j] += e.second * (2 - 4 * (side[i] != side[j]));
                    }
                }
            }
        }
    } while (hasChange);

    this->fitness = value;
}

double Individual::getFitness() const {
    return fitness;
}

int Individual::getSize() {
    return (int) this->gene.size();
}

int Individual::getCx(int i) {
    return cx[i];
}

int Individual::getChg(int i) {
    return chg[i];
}
