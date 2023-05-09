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

	//如果指明ram，则输出为从左到右ran的概率
	double Rand(int p, double ran = -1, RandomMachine* rm = &DefaultRandomMachine);
};