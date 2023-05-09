#include "ContinuousRandom.h"

void ContinuousRatio::Merge(double val, double chan)
{
	this->chance.push_back(ChanceFlag(val, chan));
	this->Adjusted = 0;
	this->Sorted = 0;
}

void ContinuousRatio::Adjust()
{
	if (this->Sorted == 0) this->Sort();

	double tot = this->TotalChance();

	for (int it = 0; it < this->chance.size(); ++it)
	{
		this->chance[it].chan /= tot;
	}

	this->Adjusted = 1;
}

double ContinuousRatio::Expectation()
{
	return Rand(0, 0.5);
}

double ContinuousRatio::TotalChance()
{
	if (this->Sorted == 0) this->Sort();

	double tot = 0;
	for (int it = 0; it < this->chance.size() - 1; ++it)
	{
		tot += (this->chance[it + 1].val - this->chance[it].val) * (this->chance[it].chan + this->chance[it + 1].chan) / 2.0f;
	}

	return tot;
}

void ContinuousRatio::Sort()
{
	std::sort(&(this->chance[0]), &(this->chance[0]) + this->chance.size());
	this->Sorted = 1;
}

double ContinuousRatio::Rand(int p, double ran )
{

	if (this->Sorted == 0) this->Sort();
	if (this->Adjusted == 0) this->Adjust();

	if (ran < 0)	ran = Randd(0, 1, p);
	int it;
	for(it = 0; it < this->chance.size() - 1; ++it)
	{
		double tmp = (this->chance[it + 1].val - this->chance[it].val) * (this->chance[it].chan + this->chance[it + 1].chan) / 2.0f;
		if (ran >= tmp) ran -= tmp;
		else
		{
			ran /= tmp;
			break;
		}
	}

	if (it >= this->chance.size() - 1)
	{
		return this->chance[this->chance.size() - 1].val;
	}

	double _A = (this->chance[it + 1].chan - this->chance[it].chan) / (this->chance[it + 1].val - this->chance[it].val);
	double _B = 2 * this->chance[it].chan;
	double _C = -(this->chance[it].chan + this->chance[it + 1].chan) * (this->chance[it + 1].val - this->chance[it].val) * ran;

	double tmp;
	if (std::abs(_A) > 1e-6)
	{
		tmp = std::sqrt(_B * _B - 4 * _A * _C);
		tmp = (-_B + tmp) / 2.0f / _A;
	}
	else
	{
		return -1;
		tmp = -_C / _B;
	}

	return tmp + this->chance[it].val;
}
