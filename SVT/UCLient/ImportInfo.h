#pragma once
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <array>

namespace ImportInfo
{
	typedef std::array<int, 24> BlockInfo;

	struct UnitModel {
		int TexStartX, TexStartY, TexEndX, TexEndY;
		float PosStartX, PosStartY, PosStartZ;
		float PosMidX, PosMidY, PosMidZ;
		float PosEndX, PosEndY, PosEndZ;
		short layer;
	};

	typedef std::array<std::vector<UnitModel>, 7> SpecialBlockInfo;

	struct Texture4 {
		int stx = 0, sty = 0, endx = 16, endy = 16;
	};

	extern std::map<int, BlockInfo> binfo;
	extern std::map<int, SpecialBlockInfo> spbinfo;

	extern void StartImport();
	extern bool IsSpecialModel[1024];
	extern bool IsNotSolid[1024];
	extern bool IsTransmit[1024];
	extern bool CanWalkThrough[1024];
}