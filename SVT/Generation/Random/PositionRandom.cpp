#include "PositionRandom.h"
#include <iostream>



std::vector<PositionRandom::Pos2> PositionRandom::GenLocation(double chan, int minx, int maxx, int miny, int maxy, int p = 0, int q = 1, RandomMachine* rm = &DefaultRandomMachine)
{
	std::vector<PositionRandom::Pos2> res;

	for (int i = minx; i <= maxx; i += q)
	{
		for (int j = miny; j <= maxy; j += q)
		{
			unsigned mj = rm->magic3(i, j, p);
			//std::cout << "mj = " << mj << '\n';
			auto m = uRandd(0, 1, mj);
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


