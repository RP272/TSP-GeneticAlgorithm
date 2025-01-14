#include "PathSorter.h"

void PathSorter::swap(Path* population, int a, int b)
{
	Path tmp;
	tmp = population[a];
	population[a] = population[b];
	population[b] = tmp;
}

int PathSorter::partition(Path* population, int left, int right)
{
	int pivot = population[int((left + right) / 2)].cost;
	int l = left;
	int r = right;
	while (true) {
		while (population[l].cost < pivot) ++l;
		while (population[r].cost > pivot) --r;
		if (l < r) {
			swap(population, l, r);
			++l;
			--r;
		}
		else {
			if (r == right) r--;
			return r;
		}
	}
}

void PathSorter::sort(Path* population, int l, int p)
{
	if (l >= p) return;
	int m = partition(population, l, p);
	/*
	Below if statements are implemented in order to prevent stack overflow from happening, when input data is already sorted and edge elements are used as pivots.
	*/
	if (m - l < p - m - 1) {
		sort(population, l, m);
		sort(population, m + 1, p);
	}
	else {
		sort(population, m + 1, p);
		sort(population, l, m);
	}
}