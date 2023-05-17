#include "GenBasicLight.h"
#include "../ImportInfo.h"
#define DecayLight 0.0625f

float ULight::SunLight = 1.0f;

void ULight::GenBsLight(Generation::Chunk* chunk)
{
	if (chunk == nullptr)
	{
		std::cout << "Invalid Chunk\n";
		return;
	}

	unsigned sunh[16][16];
	memset(sunh, 0, sizeof(sunh));

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			for (int k = 127; k >= 0; --k)
			{
				if (chunk->block[i][j][k] != 0 && ImportInfo::IsTransmit[chunk->block[i][j][k]] == 0) {
					sunh[i][j] = k;
					break;
				}
				chunk->lit[i][j][k] = ULight::SunLight;
			}
		}
	}
	unsigned int U = 16;
	while (U--)
	{
		for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j)
		for(int k = sunh[i][j] - 1; k >= 0; --k)
		{
			if (chunk->block[i][j][k] != 0 && ImportInfo::IsTransmit[chunk->block[i][j][k]] == 0) continue;
			for (unsigned m = 0; m < 6; ++m)
			{
				int _i = i;
				int _j = j;
				int _k = k;
				switch (m)
				{
				case 0:
					_i -= 1;
					break;
				case 1:
					_j -= 1;
					break;
				case 2:
					_k -= 1;
					break;
				case 3:
					_i += 1;
					break;
				case 4:
					_j += 1;
					break;
				case 5:
					_k += 1;
					break;
				}
				if (_k < 0 || _k >= 128) continue;
				if (_i < 0 || _i >= 16 || _j < 0 || _j >= 16)
				{
					auto p = GenMain::WorldLit(_i + chunk->x * 16, _j + chunk->y * 16, _k);
					if (p == nullptr) {
				//		std::cout << "NULLPTR";
						continue;
					}
					chunk->lit[i][j][k] = std::max(chunk->lit[i][j][k], *p - DecayLight);
			//		std::cout << "Invalid Data" << i << ' ' << j << ' ' << _i << ' ' << _j << '\n';
				}
				else
				{
					chunk->lit[i][j][k] = std::max(chunk->lit[i][j][k], chunk->lit[_i][_j][_k] - DecayLight);
				}
			}
		}
	}	
}
