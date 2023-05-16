#include "PositionRandom.h"
#include <iostream>
#include "../../UCLient/Graph/BasicClass/lib/BasicHeaders.h"



std::vector<PositionRandom::Pos2> PositionRandom::GenLocation(double chan, int minx, int maxx, int miny, int maxy, int p = 0, int q = 1, RandomMachine* rm = &DefaultRandomMachine)
{
	std::vector<PositionRandom::Pos2> res;

	minx -= IntMod(minx, q);
	miny -= IntMod(miny, q);


	for (int i = minx; i <= maxx; i += q)
	{
		for (int j = miny; j <= maxy; j += q)
		{
			unsigned mj = rm->magic3(i, j, p) + rm->magic2(i, j);
			//std::cout << "mj = " << mj << '\n';
			auto m = uRandd(0, 1, mj ^ seed ^ i ^ j);
			//std::cout << "m = " << m << '\n';
			//std::cout << mj << "                " << m << '\n';
			if (m > chan) continue;
			auto th = std::make_pair(i, j);

			if (q > 1)
			{
				th.first +=  rm->Randi(0, q + 1, rm->magic3(i, j, mj));
				th.second += rm->Randi(0, q + 1, rm->magic3(mj, th.first, i));
				if (th.first < minx || th.first > maxx) continue;
				if (th.second < miny || th.second > maxy) continue;
			}

			res.push_back(th);
		}
	}

	return res;
}

void PositionRandom::SpreadBiome(double chan, Generation::WorldUnit* unit, int id, int p)
{

}


