#include "Basic.h"

int Generation::seed;


std::string Generation::dir;

Generation::Chunk* ImportChunk(int x, int y)
{
	Generation::Chunk* it = nullptr;
	std::ifstream fin(Generation::dir + "/chunks/" + std::to_string(x) + '.' + std::to_string(y) + ".chunk");
	if (!fin.good()) return nullptr;

	it = new Generation::Chunk();
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
		{
			fin >> it->biomeid[i][j];
			for (int k = 0; k < 128; ++k)
			{
				fin >> it->block[i][j][k];
			}
		}
	return it;
}

void Generation::Chunk::Save()
{
	std::string tmp = (Generation::dir + "/chunks/" + std::to_string(this->x) + '.' + std::to_string(this->y) + ".chunk");
	const char* f = tmp.c_str();
	DeleteFile((LPCWSTR)f);
	std::ofstream fout(f);
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
		{
			fout << this->biomeid[i][j] << ' ';
			for (int k = 0; k < 128; ++k)
			{
				fout << this->block[i][j][k] << ' ';
			}
		}
}

void Generation::Chunk::Biome2DOut()
{
	std::cout << "Chunk(" << this->x << ", " << this->y << ")\n";
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			std::cout << this->biomeid[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

inline void Generation::WorldUnit::Save()
{
	for(int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
		{
			this->chunk[i][j]->Save();
		}
}

Generation::BiomeMenu::BiomeMenu(Biome*& _currentBiome) :
	currentBiome(_currentBiome)
{
}

Generation::WorldUnit* Generation::BiomeMenu::DivideBiomes(WorldUnit* unit)
{

	for (auto& it : BiomeList)
	{
		currentBiome = it.second();
		currentBiome->Divide(unit);
	}

	for (auto& it : BiomeList)
	{
		currentBiome = it.second();
		currentBiome->Generate(unit);
	}

	return unit;
}

