#pragma once
#include <random>
#include <algorithm>
#include <queue>

extern int seed;

extern std::uniform_int_distribution<> distrib;

extern unsigned Rand(int p);

extern double Randd(double minn, double maxn, int p = 0, int tm = 1);

extern int Randi(int minn, int maxn, int p = 0);

struct ChanceFlag
{
public:
	double val, chan;
	ChanceFlag(double Val, double Chan);
	bool operator< (ChanceFlag con) const;
	bool operator> (ChanceFlag con) const;
};