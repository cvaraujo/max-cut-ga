#include <sstream>
#include <chrono>
#include "Graph.h"
#include "../include/Individual.h"
#include "../include/Population.h"
#include "../include/Algorithm.h"

void checkSolution(Graph *graph, Individual *individual) {
    int i, j, objective = 0;
    for (i = 0; i < graph->getN(); i++) {
        for (auto e : graph->graph[i]) {
            j = e.first;
            if (individual->getGene(i) != individual->getGene(j)) {
                objective += e.second;
            }
        }
    }
    cout << "Objective made from solution: " << objective / 2 << endl;
}

double defaultGeneticAlgorithm(Graph *graph, int timeLimit, int populationSize, int newIndividuals) {
    auto population = new Population(populationSize);
    population->initPopDefault(graph);

    for (int i = 0; i < populationSize; i++) cout << population->getIndividual(i)->getFitness() << " ";
    cout << endl;
/*
 * 9663 9543 9609 9583 9622 9536 9556 9519 9653 9524 9525 9583 9512 9508 9529 9735 9674 9659 9497 9575 9600 9594 9536 9714 9687 9653 9525 9580 9491 9490 9589 9587 9537 9623 9506 9471 9514 9584 9647 9616 9554 9602 9591 9596 9631 9549 9592 9674 9562 9568 9557 9599 9571 9621 9668 9563 9558 9587 9589 9538 9567 9646 9578 9645 9745 9576 9532 9569 9588 9653
 * 9663 9543 9609 9583 9622 9536 9556 9519 9653 9524 9525 9583 9512 9508 9529 9735 9674 9659 9497 9575 9600 9594 9536 9714 9687 9653 9525 9580 9491 9490 9589 9587 9537 9623 9506 9471 9514 9584 9647 9616 9554 9602 9591 9596 9631 9549 9592 9674 9562 9568 9557 9599 9571 9621 9668 9563 9558 9587 9589 9538 9567 9646 9578 9645 9745 9576 9532 9569 9588 9653
 *
 *
 *
 * */

    auto *fittest = new Individual(*population->getFittest());
    Individual *auxInd;

    int generations = 0;

    cout << "Initial population with size " << populationSize << endl;
    cout << "Generating " << newIndividuals << " new individuals per generation" << endl;
    cout << "Running by " << timeLimit << " seconds" << endl;
    cout << "The fittest individual at 1th generation have " << fittest->getFitness() << " of fitness" << endl;

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < timeLimit) {
        auxInd = population->getFittest();
        if (fittest->getFitness() < auxInd->getFitness()) {
            fittest = new Individual(*auxInd);
        }

        Algorithm::defaultEvolvePopulation(population, newIndividuals);
        end = chrono::steady_clock::now();
        generations++;
    }

//    checkSolution(graph, fittest);
    cout << "The number of generations was " << generations << endl;
    return fittest->getFitness();
}

double initialPopulationGeneticAlgorithm(Graph *graph, int timeLimit, int populationSize, int newIndividuals) {
    auto population = new Population(populationSize);
    population->initPopConstrained(graph);

    auto *fittest = new Individual(*population->getFittest());
    Individual *auxInd;

    int generations = 0;

    cout << "Initial Constrained population with size " << populationSize << endl;
    cout << "Generating " << newIndividuals << " new individuals per generation" << endl;
    cout << "Running by " << timeLimit << " seconds" << endl;
    cout << "The fittest individual at 1th generation have " << fittest->getFitness() << " of fitness" << endl;

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < timeLimit) {
        auxInd = population->getFittest();
        if (fittest->getFitness() < auxInd->getFitness()) {
            fittest = new Individual(*auxInd);
        }

        Algorithm::defaultEvolvePopulation(population, newIndividuals);
        end = chrono::steady_clock::now();
        generations++;
    }

    cout << "The number of generations was " << generations << endl;
    return fittest->getFitness();
}

double mutationGeneticAlgorithm(Graph *graph, int timeLimit, int populationSize, int newIndividuals) {
    auto population = new Population(populationSize);
    population->initPopDefault(graph);

    auto *fittest = new Individual(*population->getFittest());
    Individual *auxInd;

    int generations = 0;

    cout << "Initial population with size " << populationSize << endl;
    cout << "Generating " << newIndividuals << " new individuals per generation" << endl;
    cout << "Running by " << timeLimit << " seconds" << endl;
    cout << "The fittest individual at 1th generation have " << fittest->getFitness() << " of fitness" << endl;

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < timeLimit) {
        auxInd = population->getFittest();
        if (fittest->getFitness() < auxInd->getFitness()) {
            fittest = new Individual(*auxInd);
        }

        Algorithm::constrainedEvolvePopulation(population, newIndividuals);
        end = chrono::steady_clock::now();
        generations++;
    }

    cout << "The number of generations was " << generations << endl;
    return fittest->getFitness();
}

double hybridGeneticAlgorithm(Graph *graph, int timeLimit, int populationSize, int newIndividuals) {
    auto population = new Population(populationSize);
    population->initPopMixed(graph, 1);

    auto *fittest = new Individual(*population->getFittest());
    Individual *auxInd;

    int generations = 0;

    cout << "Mixed initial population with size " << populationSize << endl;
    cout << "Generating " << newIndividuals << " new individuals per generation" << endl;
    cout << "Running by " << timeLimit << " seconds" << endl;
    cout << "The fittest individual at 1th generation have " << fittest->getFitness() << " of fitness" << endl;

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < timeLimit) {
        auxInd = population->getFittest();
        if (fittest->getFitness() < auxInd->getFitness()) {
            fittest = new Individual(*auxInd);
        }

        Algorithm::hybridEvolvePopulation(population, newIndividuals);
        end = chrono::steady_clock::now();
        generations++;
    }

    cout << "The number of generations was " << generations << endl;
    return fittest->getFitness();
}

int main(int argc, const char *argv[]) {
    if (argc < 7) {
        cout << "Wrong number of parameters" << endl;
        cout << "Usage:.\\GA <variation> <population-size> <new-at-generation> "
                "<time-limit> <instance-file-name> <result-file-name>" << endl;
        cout
                << "--------------------------\nVariations:\ndf = Default GA\nip = Initial Population\nmt = Mutation\nhy = "
                   "Hybrid version\n--------------------------" << endl;
        exit(0);
    } else {
        srand(time(NULL));
        srand48(time(NULL));
        string variation_df = "df", variation_ip = "ip", variation_mt = "mt", variation_hy = "hy", instanceFile, result;
        int time = 0, populationSize = 0, newIndividuals = 0;
        double solution = 0;

        stringstream convertTime(argv[4]), convertPopSize(argv[2]), convertNewInd(argv[3]);

        convertTime >> time;
        convertPopSize >> populationSize;
        convertNewInd >> newIndividuals;

        auto graph = new Graph(argv[5]);

        if (variation_df == argv[1]) {
            solution = defaultGeneticAlgorithm(graph, time, populationSize, newIndividuals);
        } else if (variation_ip == argv[1]) {
            solution = initialPopulationGeneticAlgorithm(graph, time, populationSize, newIndividuals);
        } else if (variation_mt == argv[1]) {
            solution = mutationGeneticAlgorithm(graph, time, populationSize, newIndividuals);
        } else if (variation_hy == argv[1]) {
            solution = hybridGeneticAlgorithm(graph, time, populationSize, newIndividuals);
        } else {
            cout << "Invalid variation of GA" << endl;
            exit(0);
        }
        cout << "The fitness of the fittest individual was " << solution << endl;
    }
    return 0;
}