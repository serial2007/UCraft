#pragma once
#include "../../../../Generation/Basic.h"
#include "../../../../Generation/Random/Random.h"
#include "../../../../Generation/Entry.h"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
namespace HILL
{
	class Tree : public Generation::Biome
	{
	public:
		void Divide(Generation::WorldUnit*);
		void Generate(Generation::WorldUnit*);
		unsigned id() { return 50; }
	private:
		RandomMachine rm = RandomMachine(50);
		void GenLeaves(Generation::WorldUnit*, int x, int y, int z, float gen);
	};
}