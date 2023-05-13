#pragma once
#include "Basic.h"
#include <map>
#include "../Class/Biome/Test/test.h"
#include "../Class/Biome/Test/TestSon/testSon.h"
#include "../Class/Biome/Hill/hill.h"

;;;;;;;;;;;;;;;;;;;;;;;;;

namespace GenMain
{

	extern std::map<std::pair<int, int>, Generation::WorldUnit*> WorldUnitTmp;

	extern unsigned short* WorldBlock(int x, int y, int z);

	extern inline void RegisterBiomeMain();

	
}

extern  Generation::Biome* currentB;

extern  Generation::BiomeMenu* biomeMenu;

Generation::Chunk* Enquiry(int x, int y);

extern void SaveAll();