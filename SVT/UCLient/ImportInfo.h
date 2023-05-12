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

	struct Texture4 {
		int stx = 0, sty = 0, endx = 16, endy = 16;
	};

	extern std::map<int, BlockInfo> binfo;

	extern void StartImport();
}