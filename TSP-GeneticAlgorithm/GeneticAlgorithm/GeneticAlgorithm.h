#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "../DataManagement/InputData.h"
#include "../DataManagement/ConfigurationData.h"
#include "../Path/Path.h"
#include <random>

class GeneticAlgorithm
{
public:
	static Path* generateInitialPopulation(InputData inputData, int populationSize);
	static int eval(Path path, int** costMatrix);
	static void run(ConfigurationData config, InputData inputData);
	static void inversionMutation(Path* path);
	static void orderCrossover(Path parent1, Path parent2, Path* offspring1, Path* offspring2);
	static Path tournamentSelection(Path* population, int populationSize, int tournamentSize, std::mt19937_64& rng);
};

#endif