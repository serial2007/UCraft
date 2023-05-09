#pragma once
#include <random>
#include <algorithm>
#include <queue>

extern int seed;

extern std::uniform_int_distribution<> distrib;

extern unsigned uRand(int p);

extern double uRandd(double minn, double maxn, int p = 0, int tm = 1);

extern int uRandi(int minn, int maxn, int p = 0);

struct ChanceFlag
{
public:
	double val, chan;
	ChanceFlag(double Val, double Chan);
	bool operator< (ChanceFlag con) const;
	bool operator> (ChanceFlag con) const;
};

namespace Random
{
	unsigned magic1(int a, int p = 1287514264, int q = 16136);
	unsigned magic2(int a, int b, int p = 1103515245, int q = 12345);
	unsigned magic3(int a, int b, int c, int p = 1472528274, int q = 14233);
}

class RandomMachine
{
public:
	RandomMachine(int);
	unsigned magic1(int a);
	unsigned magic2(int a, int b);
	unsigned magic3(int a, int b, int c);

	unsigned Rand(int p) { return uRand(p + this->magic1(p)); }
	unsigned Randd(double minn, double maxn, int p = 0, int tm = 1) { return uRandd((int)minn, (int)maxn, this->magic3(minn, maxn, p), tm); }
	unsigned Randi(int minn, int maxn, int p = 0) {return uRandd(minn, maxn, this->magic3(minn, p, maxn));}

	int id;
private:
	int p1, q1, p2, q2, p3, q3;
};

extern RandomMachine DefaultRandomMachine;