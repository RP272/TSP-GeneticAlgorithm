#ifndef CONFIGURATIONDATA_H
#define CONFIGURATIONDATA_H
#include <string>

struct ConfigurationData
{
	std::string filename;
	int stopAfterNSeconds; 
	std::string outputFilename;

	int populationSize;
	float crossoverRate;
	float mutationRate;
	
	int numberOfMeasurements;
};

#endif
