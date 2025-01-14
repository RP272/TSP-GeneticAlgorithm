#ifndef PATHSORTER_H
#define PATHSORTER_H

#include "Path.h"
class PathSorter
{
public:
	static void swap(Path* population, int a, int b);
	static int partition(Path* population, int left, int right);
	static void sort(Path* population, int l, int p);
};

#endif