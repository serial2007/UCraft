#include "Entry.h"
#include <iostream>
#include <iomanip>
#include "../UCLient/Graph/BasicClass/lib/BasicHeaders.h"
#include "../Class/Biome/Hill/Tree/tree.h"
#include "../Class/Biome/Desert/desert.h"
#include "../UCLient/Graph/GenBasicLight.h"
Generation::Biome* currentB = nullptr;
Generation::BiomeMenu* biomeMenu = nullptr;

std::map<std::pair<int, int>, Generation::WorldUnit*> GenMain::WorldUnitTmp;
Generation::Chunk* Enquiry(int x, int y)
{
	if (GenMain::WorldUnitTmp.find(std::make_pair(IntDiv(x, 16), IntDiv(y, 16))) != GenMain::WorldUnitTmp.end())
	{
		return GenMain::WorldUnitTmp[std::make_pair(IntDiv(x, 16), IntDiv(y, 16))]->chunk[IntMod(x, 16)][IntMod(y, 16)];
	}
	else
	{
		Generation::WorldUnit* unit = ImportWorldUnit(IntDiv(x, 16), IntDiv(y, 16));
		if (unit == nullptr)
		{
			unit = new Generation::WorldUnit(IntDiv(x, 16), IntDiv(y, 16));
			unit->NewChunks();
			biomeMenu->DivideBiomes(unit);

		}

		GenMain::WorldUnitTmp[std::make_pair(IntDiv(x, 16), IntDiv(y, 16))] = unit;
		for(int i = 0; i < 16; ++i)
		for(int j = 0; j < 16; ++j)
		{
			ULight::GenBsLight(unit->chunk[i][j]);
		}
		return unit->chunk[IntMod(x, 16)][IntMod(y, 16)];
		
	}
}

#define RegisterBiome(x, y) std::cout << std::setw(20) << #x << "  |  "; biomeMenu->uRegisterBiome<x>((new x)->id(), y);



unsigned short* GenMain::WorldBlock(float x, float y, float z)
{
	std::swap(y, z); y = -y;
	if (WorldUnitTmp.find(std::make_pair(IntDiv(x, 256), IntDiv(y, 256))) == WorldUnitTmp.end())
	{
		//std::cout << "No Unit" << IntDiv(x, 256) << ' ' << IntDiv(y, 256) << '\n';
		return nullptr;
	}
	auto unit = WorldUnitTmp[std::make_pair(IntDiv(x, 256), IntDiv(y, 256))];

	x = IntMod(x, 256);
	y = IntMod(y, 256);
	int _x = floorf(x);
	int _y = floorf(y);
	int _z = floorf(z);
	auto chunk = unit->chunk[_x / 16][_y / 16];
	//if (chunk == nullptr) return nullptr;
	_x = _x % 16;
	_y = _y % 16;
	if (_x < 0 || _x >= 16 || _y < 0 || _y >= 16 || _z < 0 || _z >= 128) {
		//std::cout << "Invalid Pos " << x << ' ' << y << ' ' << z << '\n';
		return nullptr;
	}
	return &(chunk->block[_x][_y][_z]);
}


unsigned int* GenMain::WorldNbt(float x, float y, float z)
{
	std::swap(y, z); y = -y;
	if (WorldUnitTmp.find(std::make_pair(IntDiv(x, 256), IntDiv(y, 256))) == WorldUnitTmp.end())
	{
		//std::cout << "No Unit" << IntDiv(x, 256) << ' ' << IntDiv(y, 256) << '\n';
		return nullptr;
	}
	auto unit = WorldUnitTmp[std::make_pair(IntDiv(x, 256), IntDiv(y, 256))];

	x = IntMod(x, 256);
	y = IntMod(y, 256);
	int _x = floorf(x);
	int _y = floorf(y);
	int _z = floorf(z);
	auto chunk = unit->chunk[_x / 16][_y / 16];
	//if (chunk == nullptr) return nullptr;
	_x = _x % 16;
	_y = _y % 16;
	if (_x < 0 || _x >= 16 || _y < 0 || _y >= 16 || _z < 0 || _z >= 128) {
		//std::cout << "Invalid Pos " << x << ' ' << y << ' ' << z << '\n';
		return nullptr;
	}
	return &(chunk->blockstate[_x][_y][_z]);
}

float* GenMain::WorldLit(int x, int y, int z)
{
	//std::swap(y, z); y = -y;
	if (WorldUnitTmp.find(std::make_pair(IntDiv(x, 256), IntDiv(y, 256))) == WorldUnitTmp.end())
	{
		return nullptr;
	}
	auto unit = WorldUnitTmp[std::make_pair(IntDiv(x, 256), IntDiv(y, 256))];

	x = IntMod(x, 256);
	y = IntMod(y, 256);

	auto chunk = unit->chunk[x / 16][y / 16];
	//if (chunk == nullptr) return nullptr;
	x = x % 16;
	y = y % 16;
	if (x < 0 || x >= 16 || y < 0 || y >= 16 || z < 0 || z >= 128) {
		//std::cout << "Invalid Pos " << x << ' ' << y << ' ' << z << '\n';
		return nullptr;
	}
	return &(chunk->lit[x][y][z]);
}

void GenMain::RegisterBiomeMain()
{
	


	currentB = nullptr;
	biomeMenu = new Generation::BiomeMenu(currentB);
	currentB = biomeMenu;

	for(int i = 0; i <= REGISTER_NULL; ++i)
		biomeMenu->Mapid[i] = REGISTER_NULL;

	biomeMenu->Mapid[REGISTER_BASE] = REGISTER_BASE;



	RegisterBiome(HILL::Hill,			0xfffU);
	RegisterBiome(HILL::Tree,			4U);
	RegisterBiome(UBiome::Desert, 0xfffU);
}

void SaveAll()
{
	for (auto& it : GenMain::WorldUnitTmp)
	{
		it.second->Save();
	}
}