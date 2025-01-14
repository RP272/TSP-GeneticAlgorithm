#include "GeneticAlgorithm.h"
#include "../Path/RandomPathGenerator.h"
#include "../Path/PathSorter.h"
#include "../DataManagement/RandomIntegerGenerator.h"
#include "../NearestNeighbor/NearestNeighbor.h"
#include <iostream>
#include <chrono>
#include <random>

int GeneticAlgorithm::eval(Path path, int** costMatrix)
{
	int cost = 0;
	for (int i = 0; i < path.length - 1; i++) {
		cost += costMatrix[path.cities[i]][path.cities[i + 1]];
	}
	cost += costMatrix[path.cities[path.length - 1]][path.cities[0]];
	return cost;
}

Path* GeneticAlgorithm::generateInitialPopulation(InputData inputData, int populationSize)
{
	
	/*if (populationSize < inputData.numberOfCities) {
		Path* population = new Path[populationSize];
		for (int i = 0; i < populationSize; i++) {
			population[i] = NearestNeighbor::solveTSP(inputData, i);
		}
		return population;
	}
	Path* population = new Path[populationSize];
	for (int i = 0; i < inputData.numberOfCities; i++) {
		population[i] = NearestNeighbor::solveTSP(inputData, i);
	}
	std::cout << std::endl;
	for (int i = 0; i < populationSize - inputData.numberOfCities; i++) {
		population[i + inputData.numberOfCities] = RandomPathGenerator::generate(inputData.numberOfCities);
	}
	return population;*/

	Path* population = new Path[populationSize];
	for (int i = 0; i < populationSize; i++) {
		population[i] = NearestNeighbor::solveTSP(inputData, i%inputData.numberOfCities);
	}
	for (int i = 0; i < populationSize - inputData.numberOfCities; i++) {
		inversionMutation(&population[i + inputData.numberOfCities]);
	}
	return population;


	/*Path* population = new Path[populationSize];
	for (int i = 0; i < populationSize; i++) {
		population[i] = RandomPathGenerator::generate(inputData.numberOfCities);
	}
	return population;*/
}

void GeneticAlgorithm::inversionMutation(Path* path)
{
	RandomIntegerGenerator rng(0, path->length - 1);
	int idx1, idx2, idx3, tmp;
	idx1 = rng.generate();
	idx2 = rng.generate();
	while (idx2 == idx1) idx2 = rng.generate();
	if (idx1 > idx2) {
		tmp = idx1;
		idx1 = idx2;
		idx2 = tmp;
	}
	//tmp = path->cities[idx1];
	//path->cities[idx1] = path->cities[idx2];
	//path->cities[idx2] = tmp;

	Path newPath;
	newPath.length = path->length;
	newPath.cities = new int[newPath.length];
	for (int i = 0; i < path->length; i++) newPath.cities[i] = path->cities[i];
	for (int i = 0; i < path->length; i++) {
		if (i < idx1 || i > idx2) {
			path->cities[i] = newPath.cities[i];
		}
		else {
			path->cities[i] = newPath.cities[idx2 - (i - idx1)];
		}
	}

	delete[] newPath.cities;
}

void GeneticAlgorithm::orderCrossover(Path parent1, Path parent2, Path* offspring1, Path* offspring2)
{
	/*if (parent1.cost == parent2.cost) {
		std::cout << " NO WAY " << std::endl;
	}
	std::cout << "parent1: ";*/
	//for (int i = 0; i < parent1.length; i++) {
	//	std::cout << parent1.cities[i] << "->";
	//}
	//std::cout << parent1.cities[0] << std::endl;
	//std::cout << "parent2: ";
	/*for (int i = 0; i < parent2.length; i++) {
		std::cout << parent2.cities[i] << "->";
	}
	std::cout << parent2.cities[0] << std::endl;*/

	for (int i = 0; i < parent1.length; i++) {
		offspring1->cities[i] = -1;
		offspring2->cities[i] = -1;
	}
	RandomIntegerGenerator rng(0, parent1.length - 1);
	int idx1, idx2, tmp;
	idx1 = rng.generate();
	idx2 = rng.generate();
	while (idx2 == idx1) idx2 = rng.generate();
	if (idx1 > idx2) {
		tmp = idx1;
		idx1 = idx2;
		idx2 = tmp;
	}
	//std::cout << "idx1: " << idx1 << " idx2: " << idx2 << std::endl;
	for (int i = idx1; i <= idx2; i++) {
		offspring1->cities[i] = parent1.cities[i];
		offspring2->cities[i] = parent2.cities[i];
	}
	int offspring1Idx = (idx2 + 1) % parent1.length;
	int firstParentIdx = (idx2 + 1) % parent1.length;
	int offspring2Idx = (idx2 + 1) % parent2.length;
	int secondParentIdx = (idx2 + 1) % parent2.length;
	int offspring1Left = parent1.length - (idx2 - idx1 + 1);
	int offspring2Left = parent1.length - (idx2 - idx1 + 1);
	int otherParentValue;
	bool valueFound;
	while (offspring1Left > 0) {
		valueFound = false;
		otherParentValue = parent2.cities[secondParentIdx];
		for (int i = 0; i < offspring1->length; i++) {
			if (offspring1->cities[i] == otherParentValue) {
				valueFound = true;
				break;
			}
		}
		if (valueFound == false) {
			offspring1->cities[offspring1Idx] = otherParentValue;
			offspring1Left--;
			offspring1Idx = (offspring1Idx + 1) % parent1.length;
		}
		secondParentIdx = (secondParentIdx + 1) % parent1.length;
	}

	while (offspring2Left > 0) {
		valueFound = false;
		otherParentValue = parent1.cities[firstParentIdx];
		for (int i = 0; i < offspring2->length; i++) {
			if (offspring2->cities[i] == otherParentValue) {
				valueFound = true;
				break;
			}
		}
		if (valueFound == false) {
			offspring2->cities[offspring2Idx] = otherParentValue;
			offspring2Left--;
			offspring2Idx = (offspring2Idx + 1) % parent1.length;
		}
		firstParentIdx = (firstParentIdx + 1) % parent1.length;
	}
	/*std::cout << "offspring1: ";
	for (int i = 0; i < offspring1->length; i++) {
		std::cout << offspring1->cities[i] << "->";
	}
	std::cout << offspring1->cities[0] << std::endl;
	std::cout << "offspring2: ";
	for (int i = 0; i < parent2.length; i++) {
		std::cout << offspring2->cities[i] << "->";
	}
	std::cout << offspring2->cities[0] << std::endl;*/
}

void GeneticAlgorithm::run(ConfigurationData config, InputData inputData)
{
	std::mt19937_64 rng;
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	std::uniform_real_distribution<double> unif(0, 1);
	int stopCriteriumInMS = config.stopAfterNSeconds * 1000;
	auto begin = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> timer = end - begin;
	int bestCost = INT_MAX;

	// Wygeneruj populację początkową.
	Path* population = GeneticAlgorithm::generateInitialPopulation(inputData, config.populationSize);
	Path* newPopulation = new Path[config.populationSize];
	for (int i = 0; i < config.populationSize; i++) {
		newPopulation[i].cities = new int[inputData.numberOfCities];
	}

	Path* mixed = new Path[config.populationSize * 2];
	for (int c = 0; c < config.populationSize * 2; c++) {
		mixed[c].cities = new int[inputData.numberOfCities];
	}

	// Oblicz koszt dla każdego osobnika.
	for (int i = 0; i < config.populationSize; i++) {
		population[i].cost = eval(population[i], inputData.costMatrix);
	}

	// Posortuj populację względem kosztu.
	PathSorter::sort(population, 0, config.populationSize - 1);

	// Oblicz prawdopodobieństwo i dystrybuantę każdego osobnika.
	float fitnessSum = 0.0;
	for (int i = 0; i < config.populationSize; i++) {
		population[i].selectionProbability = (1.0 / (float)population[i].cost);
		fitnessSum += population[i].selectionProbability;
	}
	for (int i = 0; i < config.populationSize; i++) {
		population[i].selectionProbability /= fitnessSum;
	}
	float cumulativeProb = 0.0;
	for (int i = 0; i < config.populationSize; i++) {
		cumulativeProb += population[i].selectionProbability;
		population[i].cumulativeProbability = cumulativeProb;
	}
	int newPopulationCounter;
	Path offspring1, offspring2;
	offspring1.cities = new int[inputData.numberOfCities];
	offspring1.length = inputData.numberOfCities;
	offspring2.cities = new int[inputData.numberOfCities];
	offspring2.length = inputData.numberOfCities;

	while (timer.count() < stopCriteriumInMS) {
		newPopulationCounter = 0;
		while (newPopulationCounter < config.populationSize) {

			// Wybierz parę rodziców na podstawie dystrybuanty.
			double prob = unif(rng);
			int parentNo1Index = 0, parentNo2Index = 1;
			for (int i = 0; i < config.populationSize; i++) {
				if (prob <= population[i].cumulativeProbability) {
					parentNo1Index = i;
					break;
				}
			}
			prob = unif(rng);
			for (int i = 0; i < config.populationSize; i++) {
				if (prob <= population[i].cumulativeProbability) {
					parentNo2Index = i;
					break;
				}
			}

			// Krzyżowanie.
			if (unif(rng) <= config.crossoverRate) {
				orderCrossover(population[parentNo1Index], population[parentNo2Index], &offspring1, &offspring2);
			}
			else {
				offspring1.cost = population[parentNo1Index].cost;
				offspring1.cumulativeProbability = population[parentNo1Index].cumulativeProbability;
				offspring1.length = population[parentNo1Index].length;
				offspring1.selectionProbability = population[parentNo1Index].selectionProbability;
				for (int j = 0; j < inputData.numberOfCities; j++) {
					offspring1.cities[j] = population[parentNo1Index].cities[j];
					offspring2.cities[j] = population[parentNo2Index].cities[j];
				}
				/*for (int j = 0; j < inputData.numberOfCities; j++) {
					std::cout << offspring1.cities[j] << "->";
				}
				std::cout << offspring1.cities[0] << std::endl;
				for (int j = 0; j < inputData.numberOfCities; j++) {
					std::cout << offspring2.cities[j] << "->";
				}
				std::cout << offspring2.cities[0] << std::endl;*/
				offspring2.cost = population[parentNo2Index].cost;
				offspring2.cumulativeProbability = population[parentNo2Index].cumulativeProbability;
				offspring2.length = population[parentNo2Index].length;
				offspring2.selectionProbability = population[parentNo2Index].selectionProbability;
			}
			// Mutacja
			if ((unif(rng) <= config.mutationRate)) {
				inversionMutation(&offspring1);
			}
			if ((unif(rng) <= config.mutationRate)) {
				inversionMutation(&offspring2);
			}
			newPopulation[newPopulationCounter].cost = offspring1.cost;
			newPopulation[newPopulationCounter].length = offspring1.length;
			newPopulation[newPopulationCounter].cumulativeProbability = offspring1.cumulativeProbability;
			newPopulation[newPopulationCounter].selectionProbability = offspring1.selectionProbability;
			for (int b = 0; b < inputData.numberOfCities; b++) {
				newPopulation[newPopulationCounter].cities[b] = offspring1.cities[b];
			}
			newPopulationCounter++;
			if (newPopulationCounter < config.populationSize) {
				newPopulation[newPopulationCounter].cost = offspring2.cost;
				newPopulation[newPopulationCounter].length = offspring2.length;
				newPopulation[newPopulationCounter].cumulativeProbability = offspring2.cumulativeProbability;
				newPopulation[newPopulationCounter].selectionProbability = offspring2.selectionProbability;
				for (int b = 0; b < inputData.numberOfCities; b++) {
					newPopulation[newPopulationCounter].cities[b] = offspring2.cities[b];
				}
				newPopulationCounter++;
			}
		}
		for (int i = 0; i < config.populationSize; i++) {
			newPopulation[i].cost = eval(newPopulation[i], inputData.costMatrix);
			if (newPopulation[i].cost < bestCost) {
				bestCost = newPopulation[i].cost;
				std::cout << "new bost cost: " << bestCost << std::endl;
			}
		}

		PathSorter::sort(newPopulation, 0, config.populationSize - 1);

		for (int i = 0; i < config.populationSize; i++) {
			mixed[i].cost = population[i].cost;
			mixed[i].cumulativeProbability = population[i].cumulativeProbability;
			mixed[i].length = population[i].length;
			mixed[i].selectionProbability = population[i].selectionProbability;
			for (int b = 0; b < inputData.numberOfCities; b++) {
				mixed[i].cities[b] = population[i].cities[b];
			}
		}
		for (int i = config.populationSize; i < 2 * config.populationSize; i++) {
			mixed[i].cost = newPopulation[i - config.populationSize].cost;
			mixed[i].cumulativeProbability = newPopulation[i - config.populationSize].cumulativeProbability;
			mixed[i].length = newPopulation[i - config.populationSize].length;
			mixed[i].selectionProbability = newPopulation[i - config.populationSize].selectionProbability;
			for (int b = 0; b < inputData.numberOfCities; b++) {
				mixed[i].cities[b] = newPopulation[i - config.populationSize].cities[b];
			}
		}

		PathSorter::sort(mixed, 0, 2 * config.populationSize - 1);

		for (int a = 0; a < config.populationSize; a++) {
			population[a].cost = mixed[a].cost;
			population[a].cumulativeProbability = mixed[a].cumulativeProbability;
			population[a].length = mixed[a].length;
			population[a].selectionProbability = mixed[a].selectionProbability;
			for (int b = 0; b < inputData.numberOfCities; b++) {
				population[a].cities[b] = mixed[a].cities[b];
			}
		}

		/*float fitnessSum = 0.0;
		for (int i = 0; i < config.populationSize; i++) {
			newPopulation[i].selectionProbability = (1.0 / (float)newPopulation[i].cost);
			fitnessSum += newPopulation[i].selectionProbability;
		}
		for (int i = 0; i < config.populationSize; i++) {
			newPopulation[i].selectionProbability /= fitnessSum;
		}
		float cumulativeProb = 0.0;
		for (int i = 0; i < config.populationSize; i++) {
			cumulativeProb += newPopulation[i].selectionProbability;
			newPopulation[i].cumulativeProbability = cumulativeProb;
		}*/

		/*for (int a = 0; a < config.populationSize; a++) {
			population[a].cost = newPopulation[a].cost;
			population[a].cumulativeProbability = newPopulation[a].cumulativeProbability;
			population[a].length = newPopulation[a].length;
			population[a].selectionProbability = newPopulation[a].selectionProbability;
			for (int b = 0; b < inputData.numberOfCities; b++) {
				population[a].cities[b] = newPopulation[a].cities[b];
			}
		}*/

		float fitnessSum = 0.0;
		for (int i = 0; i < config.populationSize; i++) {
			population[i].selectionProbability = (1.0 / (float)population[i].cost);
			fitnessSum += population[i].selectionProbability;
		}
		for (int i = 0; i < config.populationSize; i++) {
			population[i].selectionProbability /= fitnessSum;
		}
		float cumulativeProb = 0.0;
		for (int i = 0; i < config.populationSize; i++) {
			cumulativeProb += population[i].selectionProbability;
			population[i].cumulativeProbability = cumulativeProb;
		}
		end = std::chrono::high_resolution_clock::now();
		timer = end - begin;
		/*for (int i = 0; i < config.populationSize; i++) {
			std::cout << population[i].cost << " fitness: " << population[i].selectionProbability << std::endl;
		}*/
	}
	for (int i = 0; i < config.populationSize; i++) {
		std::cout << population[i].cost << " fitness: " << population[i].selectionProbability << std::endl;
	}
}