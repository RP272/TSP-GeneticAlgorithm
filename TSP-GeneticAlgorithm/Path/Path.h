#ifndef PATH_H
#define PATH_H

struct Path
{
	int length;
	int* cities;
	int cost;
	float selectionProbability;
	float cumulativeProbability;
};

#endif