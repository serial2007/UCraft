#pragma once
#include "../../../Generation/Basic.h"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
namespace UBiome
{
	class Desert : public Generation::Biome
	{
	public:
		void Generate(Generation::WorldUnit*);
		unsigned id() { return 70; }
	};
}