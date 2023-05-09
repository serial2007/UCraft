#pragma once
#include "../../../Generation/Basic.h"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
namespace test
{
	extern unsigned int id;
	class testBiome : public Generation::Biome
	{
	public:
		void Divide(Generation::WorldUnit*);
		void Generate(Generation::WorldUnit*);
	};
}