#pragma once
#include "../../../Generation/Basic.h"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
namespace test
{
	class testBiome : public Generation::Biome
	{
	public:
		void Divide(Generation::WorldUnit*);
		void Generate(Generation::WorldUnit*);
		unsigned id() { return 15; }
	};
}