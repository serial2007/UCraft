#include "Entry.h"


Generation::Biome* currentB = nullptr;
Generation::BiomeMenu* biomeMenu = nullptr;

std::map<std::pair<int, int>, Generation::WorldUnit*> GenMain::WorldUnitTmp;
Generation::Chunk* Enquiry(int x, int y)
{
	Generation::Chunk* chunk = ImportChunk(x, y);
	if (chunk == nullptr)
	{
		std::cout << "Unexisted chunk\n";

		auto pr = std::make_pair(x / 16, y / 16);
		if (GenMain::WorldUnitTmp.count(pr) <= 0)
		{
			std::cout << "Unexisted worldunit\n";
			Generation::WorldUnit* unit = new Generation::WorldUnit(x / 16, y / 16);
			biomeMenu->DivideBiomes(unit);
			GenMain::WorldUnitTmp[std::make_pair(x / 16, y / 16)] = unit;
			return unit->chunk[x % 16][y % 16];
		}
		
		else
		{
			return GenMain::WorldUnitTmp[std::make_pair(x / 16, y / 16)]->chunk[x % 16][y % 16];
		}
	}
	else return chunk;
	return nullptr;
}

#define RegisterBiome(x, y) uRegisterBiome<x>((new x)->id(), y)



void GenMain::RegisterBiomeMain()
{
	


	currentB = nullptr;
	biomeMenu = new Generation::BiomeMenu(currentB);
	currentB = biomeMenu;

	for(int i = 0; i <= REGISTER_NULL; ++i)
		biomeMenu->Mapid[i] = REGISTER_NULL;

	biomeMenu->Mapid[REGISTER_BASE] = REGISTER_BASE;


	biomeMenu->RegisterBiome(test::testBiome,		0xfffU);
	biomeMenu->RegisterBiome(testson::testBiome,	15U);

	

}

int G = 0;