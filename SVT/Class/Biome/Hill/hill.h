#pragma once
#include "../../../Generation/Random/Random.h"
#include "../../../Generation/Entry.h"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
namespace HILL
{
	struct Single
	{
		int x = 0, y = 0;
		double h = 0;
		int deg = 0;
	};
	class Hill : public Generation::Biome
	{
	public:
		void Divide(Generation::WorldUnit*);
		void Generate(Generation::WorldUnit*);
		unsigned id() { return 4; }
		Hill() :rm(1145){}

	private:

		struct Pos
		{
			int x, y, h = 0;
		};

		std::vector<Single> WorldHill;

		inline void GenSonHill(Generation::WorldUnit* h, HILL::Single& con, int x, int y);
		inline void ValidateHill(Generation::WorldUnit* h, HILL::Single& con, int x, int y);
		inline void Genmain(Generation::WorldUnit* h, int x, int y);
	
		inline void PrintHill(Generation::WorldUnit*, int, int);
		inline void Print(Generation::WorldUnit*, int, int);
		double WeighTmp[100001];
		Pos LocTmp[100001];
		double SmoothHill(double dis2, int p);
		double hi[256][256];

		RandomMachine rm = RandomMachine(1145);
	};
	
}