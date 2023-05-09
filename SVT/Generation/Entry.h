#pragma once
#include "Basic.h"
#include <map>
#include "../Class/Biome/Test/test.h"

;;;;;;;;;;;;;;;;;;;;;;;;;

namespace GenMain
{

	extern std::map<std::pair<int, int>, Generation::WorldUnit*> WorldUnitTmp;

	

	extern inline void RegisterBiomeMain();


}

extern  Generation::Biome* currentB;

extern  Generation::BiomeMenu* biomeMenu;

Generation::Chunk* Enquiry(int x, int y);

extern int G;