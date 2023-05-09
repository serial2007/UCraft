#include "Basic.h"

int Generation::seed;
#define WORLDUNIT_BLOCKS 8388608

std::string Generation::dir = "D:/Ucraft";

Generation::WorldUnit* ImportWorldUnit(int x, int y)
{
	Generation::WorldUnit* it = nullptr;
	std::string f = Generation::dir + "/world/" + std::to_string(x) + '.' + std::to_string(y) + ".unit";
	std::ifstream fin(f.c_str());
	try
	{
		if (!fin.good()) return nullptr;

		
		std::stringstream buf;
		buf << fin.rdbuf();
		std::string tmp(buf.str());


		unsigned int q = 0;
		it = new Generation::WorldUnit(x, y);
		for (int I = 0; I < 16; ++I)
			for (int J = 0; J < 16; ++J)
			{
				it->chunk[I][J] = new Generation::Chunk(x * 16 + I, y * 16 + J);
				
				for (int i = 0; i < 16; ++i)
					for (int j = 0; j < 16; ++j)
					{
						it->chunk[I][J]->biomeid[i][j] = tmp[q] - 1;
						++q;
						for (int k = 0; k < 128; ++k)
						{
							it->chunk[I][J]->block[i][j][k] = tmp[q] - 1;
							++q;
						}
					}
			}
		fin.close();
	}
	catch (...)
	{
		std::cout << "Import WorldUnit Failed " << f << '\n';
		remove(f.c_str());
		return nullptr;
	}
	return it;
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

void Generation::WorldUnit::Save()
{
	std::string k = (Generation::dir + "/world/" + std::to_string(x) + '.' + std::to_string(y) + ".unit");
	remove(k.c_str());
	std::ofstream fout(k.c_str());

	std::string tmp;
	for (int I = 0; I < 16; ++I)
	for (int J = 0; J < 16; ++J)
	{
		for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
		{
			tmp += char(this->chunk[I][J]->biomeid[i][j] + 1);
			for (int k = 0; k < 128; ++k)
			{
				tmp += char(this->chunk[I][J]->block[i][j][k]);
			}	
		}
	}

	fout << tmp;

	fout.close();
}


Generation::BiomeMenu::BiomeMenu(Biome*& _currentBiome) :
	currentBiome(_currentBiome)
{
}

Generation::WorldUnit* Generation::BiomeMenu::DivideBiomes(WorldUnit* unit)
{
	std::cout << "Divide Biomes (" << unit->x << ", " << unit->y << ")\n";
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
			biome->Divide(unit);
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
