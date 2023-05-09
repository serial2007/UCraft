#pragma once
#include "../../../Generation/Random/Random.h"


namespace GenHill
{
	struct Hill
	{
		int x = 0, y = 0;
		double h = 0;
		int deg = 0;
	};

	extern inline void GenSonHill(GenHill::Hill& con);

	extern inline void ValidateHill(GenHill::Hill& con);

	extern std::vector<Hill> WorldHill;

}