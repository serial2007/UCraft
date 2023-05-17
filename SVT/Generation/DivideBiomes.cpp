#include "DivideBiomes.h"
#include "Random/Random.h"

void UBiomes::DivideBiomes(Generation::WorldUnit* unit)
{
	auto GenBiome = PositionRandom::GenLocation(0.02f, unit->x * 256 - 500, unit->x * 256 + 755, unit->y * 256 - 500, unit->y * 256 + 755, 0, 90, &DefaultRandomMachine);

	for (auto& i : GenBiome)
	{
		i.first -= unit->x * 256;
		i.second -= unit->y * 256;
	}

	unsigned q = 0; long long maxq, tmp;
	for(long long i = -200; i < 456; ++i)
	for(long long j = -200; j < 456; ++j)
	{
		q = 0;
		maxq = LLONG_MAX;
		for (auto& k : GenBiome)
		{
			tmp = (k.first - i) * (k.first - i) + (k.second - j) * (k.second - j);
			if (tmp < maxq)
			{
				maxq = tmp;
				*(unit->FindBiome(i, j)) = (
					q % 2 ?
					4U : 70U
					);
			}
			++q;
		}
	}
}
