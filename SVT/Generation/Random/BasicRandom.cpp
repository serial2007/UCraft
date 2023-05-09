#include "BasicRandom.h"


int		seed = 0;



unsigned uRand(int p)
{
	std::uniform_int_distribution<> distrib(0, RAND_MAX);
	std::ranlux48 engine(seed + p);
	return distrib(engine);
}

double uRandd(double minn, double maxn, int p, int tm)
{
	if (maxn < minn) std::swap(maxn, minn);
	double gap, dif, res = minn;
	gap = dif = maxn - minn;
	while (tm--)
	{
		gap = dif / RAND_MAX;
		res += (double)(uRand(p)) * dif / RAND_MAX;
		dif = gap;
	}
	if (isinf(res)) res = maxn;
	return res;
}

int uRandi(int minn, int maxn, int p)
{
	if (maxn < minn) std::swap(maxn, minn);
	int gap, dif, res = minn;
	gap = dif = maxn - minn;
	while (dif)
	{
		gap = dif / RAND_MAX;
		res += uRand(p) * dif / RAND_MAX;
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

unsigned Random::magic1(int a, int p, int q)
{
	int value = a;
	a *= p;
	a += q;
	value = (a >> 16) & 0x07FF;

	value *= p;
	value += q;
	value = (a >> 16) & 0x07FF;

	a *= p;
	a += q;
	value = (a >> 16) & 0x07FF;

	return value;
}

unsigned Random::magic2(int a, int b, int p, int q)
{
	int value = a;

	//Use a linear congruential generator (LCG) to update the state of the PRNG
	a *= p;
	b += q;
	value = (b >> 16) & 0x07FF;

	b *= p;
	a += q;
	value <<= 10;
	value |= (a >> 16) & 0x03FF;

	b *= p;
	b += q;
	value <<= 10;
	value |= (b >> 16) & 0x03FF;
	return value;
}

unsigned Random::magic3(int a, int b, int c, int p, int q)
{
	int value = a;

	a *= p;
	b += q;
	value = (b >> 16) & 0x07FF;

	b *= p;
	a += q;
	//value <<= 10;
	value |= (a >> 16) & 0x03FF;

	b *= p;
	b += q;
	//value <<= 10;
	value |= (b >> 16) & 0x03FF;

	c *= p;
	c += q;
	value = (c >> 16) & 0x07FF;

	c *= p;
	a += c;
	//value <<= 10;
	value |= (c >> 16) & 0x03FF;

	b *= p;
	c += q;
	value <<= 10;
	value |= (b >> 16) & 0x03FF;

	return value;

}


RandomMachine::RandomMachine(int ID):
	id(ID)
{
	p1 = Random::magic1(id) % 1000000000;
	q1 = Random::magic2(id, p1 % 10000);
	p2 = Random::magic3(id, p1, q1) % 1000000000;
	q2 = Random::magic3(id, p1, p2) % 10000;
	p3 = Random::magic3(p2, q2, id) % 1000000000;
	q3 = Random::magic3(p3, id, q2) % 10000;
}

unsigned RandomMachine::magic1(int a)
{
	return Random::magic1(a, p1, q1);
}

unsigned RandomMachine::magic2(int a, int b)
{
	return Random::magic2(a, b, p2, q2);
}

unsigned RandomMachine::magic3(int a, int b, int c)
{
	return Random::magic3(a, b, c, p3, q3);
}

RandomMachine DefaultRandomMachine(0xfff);