#include "DivideBiomes.h"
#include "Random/Random.h"

void UBiomes::DivideBiomes(Generation::WorldUnit* unit)
{
	auto GenBiome = PositionRandom::GenLocation(0.03, unit->x * 256 - 1000, unit->x * 256 + 1255, unit->y * 256 - 1000, unit->y * 256 + 1255, 0, 31, &DefaultRandomMachine);

	for (int i = 0; i < GenBiome.size(); ++i)
	{
		GenBiome[i].first  -= unit->x * 256;
		GenBiome[i].second -= unit->y * 256;
	}

	int q = 0; int maxq, tmp;
	for(int i = -200; i < 456; ++i)
	for(int j = -200; j < 456; ++j)
	{
		maxq = 0xfffffff;
		for (auto& k : GenBiome)
		{
			tmp = (k.first - i) * (k.first - i) + (k.second - j) * (k.second - j);
			if (tmp < maxq)
			{
				maxq = tmp;
				*(unit->FindBiome(i, j)) = (
					(k.first + k.second + unit->x * 256 + unit->y * 256) % 2 ?
					4U : 70U
					);
			}
		}
	}
}
