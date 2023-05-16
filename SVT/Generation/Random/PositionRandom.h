#pragma once
#include "DispersedRatio.h"
#include "ContinuousRandom.h"
#include <vector>
#include "../Basic.h"

namespace PositionRandom
{
	typedef std::pair<int, int> Pos2;

	// p为随机数种子
	// q为方格中间隔的数， 保证在同一地方不会出现过多的点
	// 同时通过设置较大的chan，较大的q，可以减少性能消耗
	extern std::vector<Pos2> GenLocation(double chan, int minx, int maxx, int miny, int maxy, int p, int q, RandomMachine* rm);
	
	extern void SpreadBiome(double chan, Generation::WorldUnit* unit, int id, int p = 0);
};

