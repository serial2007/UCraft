#include "BasicRandom.h"


int		seed = 0;

std::uniform_int_distribution<> distrib(0, RAND_MAX);

unsigned Rand(int p)
{
	std::ranlux48 engine(seed + p);
	return distrib(engine);
}

double Randd(double minn, double maxn, int p, int tm)
{
	if (maxn < minn) std::swap(maxn, minn);
	double gap, dif, res = minn;
	gap = dif = maxn - minn;
	while (tm--)
	{
		gap = dif / RAND_MAX;
		res += (double)(Rand(p)) * dif / RAND_MAX;
		dif = gap;
	}
	if (isinf(res)) res = maxn;
	return res;
}

int Randi(int minn, int maxn, int p)
{
	if (maxn < minn) std::swap(maxn, minn);
	int gap, dif, res = minn;
	gap = dif = maxn - minn;
	while (dif)
	{
		gap = dif / RAND_MAX;
		res += Rand(p) * dif / RAND_MAX;
		dif = gap;
	}
	if (res > maxn) res = maxn;
	if (res < minn) res = minn;
	return res;
}

ChanceFlag::ChanceFlag(double Val, double Chan)
{
	this->val = Val;
	this->chan = Chan;
}

bool ChanceFlag::operator<(ChanceFlag con) const
{
	return this->val < con.val;
}

bool ChanceFlag::operator>(ChanceFlag con) const
{
	return this->val > con.val;
}