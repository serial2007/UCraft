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
#include "Random/Random.h"
//#include "../Generation/lib/BasicHeaders.h"
#include "../UCLient/Graph/BasicClass/lib/BasicHeaders.h"
;

//inline Generation::Chunk* ImportChunk(int x, int y);

#define ReserveWorldUnit 64

#define REGISTER_BASE 0xfff
#define REGISTER_NULL 0x1000

extern struct Pos
{
	int x, y, h = 0;
	bool e = 0;
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
		float lit[16][16][128];

		Chunk(int _X, int _Y) { 
			memset(block, 0, sizeof(block)); 
			memset(biomeid, 0, sizeof(biomeid)); 
			memset(lit, 0, sizeof(lit));
			this->x = _X, this->y = _Y;
		}

		

		void Biome2DOut();
		void Biome3Dout();
	};

	

	class WorldUnit
	{
	public:
		int x = 0, y = 0;
		Chunk* chunk[16][16];
		void Save();

		WorldUnit(int _x, int _y):
			x(_x), y(_y) {}

		~WorldUnit() {
			for(int i = 0; i < 16; ++i)
			for(int j = 0; j < 16; ++j)
			{
				delete this->chunk[i][j];
			}
		}

		unsigned short* PosBiome(int x, int y) {
			if (x < 0 || x >= 256) {
				std::cout << "x = " << x << '\n';
				return nullptr;
			}
			if (y < 0 || y >= 256) {
				std::cout << "y = " << y << '\n';
				return nullptr;
			}

			return &(chunk[IntDiv(x, 16)][IntDiv(y, 16)]->biomeid[IntMod(x, 16)][IntMod(y, 16)]); }
		unsigned short* PosBlock(int x, int y, int z) {
			if (x < 0 || x >= 256) return nullptr;
			if (y < 0 || y >= 256) return nullptr;
			if (z < 0 || z >= 128) return nullptr;

			return &(chunk[IntDiv(x, 16)][IntDiv(y, 16)]->block[IntMod(x, 16)][IntMod(y, 16)][z]); }
		void NewChunks()
		{
			for (int i = 0; i < 16; ++i)
			{
				for (int j = 0; j < 16; ++j)
				{
					chunk[i][j] = new Chunk(x * 16 + i, y * 16 + j);
				}
			}
		}

	};

	class Biome
	{
	public:
		Biome() {}
		std::vector <std::pair<unsigned int, std::function<Biome* (void)> > >  son;
		virtual unsigned id() { return REGISTER_NULL; }
		virtual void Divide(WorldUnit*) {};
		virtual void Generate(WorldUnit*) {};

		//template<typename T> void RegisterSon(int p)
		//{
		//	son.push_back(std::make_pair(p, []() {return new T(); }));
		//	std::cout << "Registering son Biome (id = " << p << ")\n";
		//}

	};


	class BiomeMenu : public Biome
	{
	public:
		BiomeMenu(Biome*& _currentBiome);

		template<typename T> void uRegisterBiome(unsigned int p, unsigned int fa)
		{
			this->BiomeList.push_back(std::make_pair(p, []() {return new T(); }));

			std::cout << "id = " << std::setw(6) << p << "  | fa = " << std::setw(7) << fa << "  | ID = " << std::setw(6) << BiomeList.size() - 1 << "  | faID = ";
			if (Mapid[fa] == REGISTER_NULL)			std::cout << " <None>" << '\n';
			else if (Mapid[fa] == REGISTER_BASE)	std::cout << "[START]" << '\n';
			else std::cout << std::setw(6) << Mapid[fa] << '\n';

			sonid[Mapid[fa]].push_back(BiomeList.size() - 1);
			Mapid[p] = BiomeList.size() - 1;
		}
		
	
		Generation::WorldUnit* DivideBiomes(WorldUnit*);
		Generation::Chunk* Enquiry(int x, int y);

		

	
		Biome*& currentBiome;
		std::vector <std::pair<unsigned int, std::function<Biome* (void)> > >  BiomeList;
		//std::stack <std::pair<unsigned int, std::function<Biome* (void)> > >  BiomeStack;

		unsigned int Mapid[REGISTER_NULL + 1];
		std::vector<unsigned int> sonid[REGISTER_NULL + 1];
	};

}

Generation::WorldUnit* ImportWorldUnit(int x, int y);

