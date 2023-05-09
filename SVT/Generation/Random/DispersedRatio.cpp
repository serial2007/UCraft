#include "DispersedRatio.h"

void DispersedRatio::Merge(double val, double chan)
{
	this->chance.push_back(ChanceFlag(val, chan));
	this->Adjusted = 0;
}

void DispersedRatio::Adjust()
{
	double tot = this->TotalChance();
	for (int i = 0; i < this->chance.size(); ++i)
	{
		this->chance[i].chan /= tot;
	}
	this->Adjusted = 1;
}

double DispersedRatio::Expectation()
{
	if (this->Adjusted == 0) this->Adjust();
	double res = 0;
	for (int i = 0; i < this->chance.size(); ++i)
	{
		res += this->chance[i].val * this->chance[i].chan;
	}
	return res;
}

double DispersedRatio::TotalChance() const
{
	double res = 0;
	for (int i = 0; i < this->chance.size(); ++i)
	{
		res += this->chance[i].chan;
	}
	return res;
}

double DispersedRatio::Rand(int p = 0)
{
	if (this->Adjusted == 0) this->Adjust();

	double ran = Randd(0, 1, p);
	unsigned long long it = 1;
	while (ran > this->chance[it].chan)
	{
		ran -= this->chance[it].chan;
		it++;
	}
	it = std::min(it, this->chance.size() - 1);
	return this->chance[it].val;
}
