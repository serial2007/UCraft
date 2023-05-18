#include "Basic.h"

int Generation::seed;
#define WORLDUNIT_BLOCKS 8388608

std::string Generation::dir = "D:/UCraft";

bool FloatEqual(float x, float y)
{
	int _x = floorf(x);
	int _y = floorf(y);
	return (_x == _y);
}

Generation::WorldUnit* ImportWorldUnit(int x, int y)
{
	Generation::WorldUnit* it = nullptr;
	std::string f = Generation::dir + "/world/" + std::to_string(x) + '.' + std::to_string(y) + ".unit";
	std::ifstream inFile(f.c_str(), std::ios::in | std::ios::binary);
	try
	{
		if (!inFile.good()) return nullptr;
		it = new Generation::WorldUnit(x, y);
		inFile.read((char*)it->biomeid, sizeof(it->biomeid));
		for (int I = 0; I < 16; ++I)
			for (int J = 0; J < 16; ++J)
			{
				it->chunk[I][J] = new Generation::Chunk(x * 16 + I, y * 16 + J);
				inFile.read((char*)it->chunk[I][J]->block, sizeof(it->chunk[I][J]->block));
				inFile.read((char*)it->chunk[I][J]->blockstate, sizeof(it->chunk[I][J]->blockstate));
			}
		inFile.close();
	}
	catch (...)
	{
		std::cout << "Import WorldUnit Failed " << f << '\n';
		remove(f.c_str());
		return nullptr;
	}
	return it;
}

void Generation::Chunk::Biome3Dout()
{
	std::cout << "Chunk(" << this->x << ", " << this->y << ")\n";
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			for(int k = 0; k < 128; ++k)
			std::cout << "(" << i << ", " << j << ", " << k << ") = " <<this->block[i][j][k] << '\n';
		}
		//std::cout << std::endl;
	}
}


void Generation::WorldUnit::Save()
{
	std::string k = (Generation::dir + "/world/" + std::to_string(x) + '.' + std::to_string(y) + ".unit");
	remove(k.c_str());
	std::ofstream outFile(k.c_str(), std::ios::out | std::ios::binary);

	outFile.write((char*)this->biomeid, sizeof(this->biomeid));
	for (int I = 0; I < 16; ++I)
	for (int J = 0; J < 16; ++J)
	{
		outFile.write((char*)this->chunk[I][J]->block, sizeof(this->chunk[I][J]->block));
		outFile.write((char*)this->chunk[I][J]->blockstate, sizeof(this->chunk[I][J]->blockstate));
	}
	outFile.close();
}

#include "../Generation/DivideBiomes.h"
Generation::BiomeMenu::BiomeMenu(Biome*& _currentBiome) :
	currentBiome(_currentBiome)
{
}

Generation::WorldUnit* Generation::BiomeMenu::DivideBiomes(WorldUnit* unit)
{
	std::cout << "Divide Biomes (" << unit->x << ", " << unit->y << ")\n";
	UBiomes::DivideBiomes(unit);


	std::stack<unsigned int> stk;
	stk.push(0xfff);
	while (!stk.empty())
	{
		unsigned p = stk.top();
		std::cout << "p = " << p << '\n';
		stk.pop();
		if (p != 0xfff)
		{
			Generation::Biome* biome = this->BiomeList[p].second();
			biome->Generate(unit);
		}


		for (int i = sonid[p].size() - 1; i >= 0; --i)
		{
			stk.push(sonid[p][i]);
		}
	}
	std::cout << "END\n";
	return unit;
}
