#include <sstream>
#include <chrono>
#include <fstream>
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

double defaultGeneticAlgorithm(Graph *graph, int timeLimit, int populationSize, int newIndividuals,
                               const string solutionPath) {
    ofstream solution;
    solution.open(solutionPath);

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();

    auto population = new Population(populationSize);
    population->initPopDefault(graph);

    auto *fittest = population->getFittest();

    int generations = 0;

    cout << "Initial population with size " << populationSize << endl;
    cout << "Generating " << newIndividuals << " new individuals per generation" << endl;
    cout << "Running by " << timeLimit << " seconds" << endl;
    cout << "The fittest individual at 1th generation have " << fittest->getFitness() << " of fitness" << endl;

    solution << populationSize << " " << newIndividuals << " " << timeLimit << endl;
    solution << fittest->getFitness() << endl;

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < timeLimit) {
        Algorithm::defaultEvolvePopulation(population, newIndividuals);

        fittest = population->getFittest();
        generations++;
        end = chrono::steady_clock::now();
    }

    cout << "The number of generations was " << generations << endl;
    solution << generations << endl;
    solution << fittest->getFitness() << endl;
    return fittest->getFitness();
}

double initialPopulationGeneticAlgorithm(Graph *graph, int timeLimit, int populationSize, int newIndividuals,
                                         const string solutionPath) {
    ofstream solution;
    solution.open(solutionPath);

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();

    auto population = new Population(populationSize);
    population->initPopConstrained(graph);

    int generations = 0;

    cout << "Initial Constrained population with size " << populationSize << endl;
    cout << "Generating " << newIndividuals << " new individuals per generation" << endl;
    cout << "Running by " << timeLimit << " seconds" << endl;
    cout << "The fittest individual at 1th generation have " << population->getFittest()->getFitness() << " of fitness"
         << endl;

    solution << populationSize << " " << newIndividuals << " " << timeLimit << endl;
    solution << population->getFittest()->getFitness() << endl;

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < timeLimit) {
        Algorithm::defaultEvolvePopulation(population, newIndividuals);
        generations++;
        end = chrono::steady_clock::now();
    }

    cout << "The number of generations was " << generations << endl;
    solution << generations << endl;
    solution << population->getFittest()->getFitness() << endl;
    return population->getFittest()->getFitness();
}

double
mutationGeneticAlgorithm(Graph *graph, int timeLimit, int populationSize, int newIndividuals, string solutionPath) {
    ofstream solution;
    solution.open(solutionPath);

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();

    auto population = new Population(populationSize);
    population->initPopDefault(graph);

    int generations = 0;


    solution << populationSize << " " << newIndividuals << " " << timeLimit << endl;
    solution << population->getFittest()->getFitness() << endl;

    cout << "Initial population with size " << populationSize << endl;
    cout << "Generating " << newIndividuals << " new individuals per generation" << endl;
    cout << "Running by " << timeLimit << " seconds" << endl;
    cout << "The fittest individual at 1th generation have " << population->getFittest()->getFitness() << " of fitness"
         << endl;

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < timeLimit) {
        Algorithm::constrainedEvolvePopulation(population, newIndividuals);
        generations++;
        end = chrono::steady_clock::now();
    }

    cout << "The number of generations was " << generations << endl;
    solution << generations << endl;
    solution << population->getFittest()->getFitness() << endl;
    return population->getFittest()->getFitness();
}

double
hybridGeneticAlgorithm(Graph *graph, int timeLimit, int populationSize, int newIndividuals, const string solutionPath) {
    ofstream solution;
    solution.open(solutionPath);

    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();

    auto population = new Population(populationSize);
    population->initPopMixed(graph, 1);

    int generations = 0;

    cout << "Mixed initial population with size " << populationSize << endl;
    cout << "Generating " << newIndividuals << " new individuals per generation" << endl;
    cout << "Running by " << timeLimit << " seconds" << endl;
    cout << "The fittest individual at 1th generation have " << population->getFittest()->getFitness() << " of fitness"
         << endl;

    solution << populationSize << " " << newIndividuals << " " << timeLimit << endl;
    solution << population->getFittest()->getFitness() << endl;

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < timeLimit) {
        Algorithm::hybridEvolvePopulation(population, newIndividuals);
        generations++;
        end = chrono::steady_clock::now();
    }

    cout << "The number of generations was " << generations << endl;
    solution << generations << endl;
    solution << population->getFittest()->getFitness() << endl;

    return population->getFittest()->getFitness();
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
            solution = defaultGeneticAlgorithm(graph, time, populationSize, newIndividuals, argv[6]);
        } else if (variation_ip == argv[1]) {
            solution = initialPopulationGeneticAlgorithm(graph, time, populationSize, newIndividuals, argv[6]);
        } else if (variation_mt == argv[1]) {
            solution = mutationGeneticAlgorithm(graph, time, populationSize, newIndividuals, argv[6]);
        } else if (variation_hy == argv[1]) {
            solution = hybridGeneticAlgorithm(graph, time, populationSize, newIndividuals, argv[6]);
        } else {
            cout << "Invalid variation of GA" << endl;
            exit(0);
        }


        cout << "The fitness of the fittest individual was " << solution << endl;
    }
    return 0;
}