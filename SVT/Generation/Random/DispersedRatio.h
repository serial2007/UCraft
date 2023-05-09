#pragma once
#include <vector>
#include "BasicRandom.h"

class DispersedRatio
{
private:
	bool Adjusted = 0;
	std::vector<ChanceFlag> chance;

public:	
	DispersedRatio() {}
	void Merge(double val, double chan);
	void Adjust();
	double Expectation();
	double TotalChance() const;


	double Rand(int p, RandomMachine* rm);
};