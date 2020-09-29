//
// Created by carlos on 26/04/18.
//

#ifndef PUREGENETICALGORITHM_GENE_H
#define PUREGENETICALGORITHM_GENE_H

#include "Graph.h"

class Individual {
    vector<int> chg, cx;
    vector<bool> side, gene;
    double fitness;


public:
    Graph *graph;
    
    Individual(Graph *graph, bool initialSolution, bool isDefault);

    void defaultSolution();

    void constrainedSolution();

    int getGene(int i) const;

    void setGene(int i);

    double getFitness() const;

    void constrainedMutation(int fixed);

    int getSize();

    int getCx(int i);

    int getChg(int i);

};


#endif //PUREGENETICALGORITHM_GENE_H
