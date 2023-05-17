#include "desert.h"
#include "../../../Generation/Random/Random.h"
#include "../../../Generation/WorldPara.h"

void UBiome::Desert::Generate(Generation::WorldUnit* unit)
{
	RandomMachine rm(70);
	auto p = PositionRandom::GenLocation(0.02, unit->x * 256 - 180, unit->x * 256 + 255 + 180, unit->y * 256 - 180, unit->y * 256 + 255 + 180, 0, 4, &rm);
	for (auto& k : p)
	{
		k.first -= unit->x * 256;
		k.second -= unit->y * 256;

		bool CanU = 1;
		for (int I = -10; I <= 10; ++I)
		{
			for (int J = -10; J <= 10; ++J)
				if (*(unit->FindBiome(k.first + I, k.second + J)) != 70) {
					CanU = 0;
					break;
				}
			if (CanU == 0) break;
		}
		if (CanU == 0) {
			k.first = k.second = 0xfffffff;
		}
	}
	

	int hi;
	int h[256][256];
	memset(h, 0, sizeof(h));
	for(int i = 0; i < 256; ++i)
	for(int j = 0; j < 256; ++j)
	{
		if (*(unit->FindBiome(i, j)) != 70) continue;
		hi = 0;
		for (auto& k : p)
		{
			if (k.first == 0xfffffff) continue;
			if ((i - k.first) * (i - k.first) + (j - k.second) * (j - k.second) < 400)
			{
				++hi;
			}
		}
		h[i][j] = std::min(int(WorldPara::minh) + hi, 127);
		for (int k = 0; k <= h[i][j]; ++k)
		{
			*(unit->PosBlock(i, j, k)) = 5U;
		}

	}

	p = PositionRandom::GenLocation(0.03, unit->x * 256, unit->x * 256 + 255, unit->y * 256, unit->y * 256 + 255, 1, 6, &rm);
	for (auto& k : p)
	{
		int x = k.first - unit->x * 256;
		int y = k.second - unit->y * 256;
		if (*(unit->FindBiome(x, y)) != 70) continue;
		if (x >= 0 && x < 256 && y >= 0 && y < 256)
		{
			int l = rm.magic2(k.first, k.second) % 4 + 2;
			for (int i = h[x][y]; i <= std::min(h[x][y] + l, 127); ++i)
			{
				*(unit->PosBlock(x, y, i)) = 77U;
			}
		}
	}

	std::cout << "DESERT GENERATED\n";
}
