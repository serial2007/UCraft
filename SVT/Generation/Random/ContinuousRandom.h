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

	//���ָ��ram�������Ϊ������ran�ĸ���
	double Rand(int p, double ran = -1, RandomMachine* rm = &DefaultRandomMachine);
};