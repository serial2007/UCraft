#pragma once
#include <io.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <cstdio>
#include <Windows.h>
#include <stack>
#include "../Graph/BasicClass/lib/Renderer.h"
//inline Generation::Chunk* ImportChunk(int x, int y);

extern struct Pos
{
	int x, y, h = 0;
};




namespace Generation
{
	extern int seed;
	extern std::string dir;

	class Chunk
	{
	public:
		int x = 0, y = 0;
		unsigned short block[16][16][128];
		unsigned short biomeid[16][16];

		Chunk() { memset(block, 0, sizeof(block)); memset(biomeid, 0, sizeof(biomeid)); }
		Chunk(int _X, int _Y) { memset(block, 0, sizeof(block)); memset(biomeid, 0, sizeof(biomeid)); this->x = _X, this->y = _Y; }

	};

}
