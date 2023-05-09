#pragma once
#include "BasicRandom.h"
#include <vector>

class ContinuousRatio
{
private:
	bool Adjusted = 0, Sorted = 0;
	std::vector<ChanceFlag> chance;

public:
	ContinuousRatio() {}
	void Merge(double val, double chan);
	void Adjust();
	double Expectation();
	double TotalChance();
	void Sort();

	double Rand(int p, double ran = -1);
};