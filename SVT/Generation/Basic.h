#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include <string>
#include <Windows.h>

#include "Random/PosStruct.h"
//inline Generation::Chunk* ImportChunk(int x, int y);

#define ReserveWorldUnit 64







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
		void Save();

		void Biome2DOut();
	};

	

	class WorldUnit
	{
	public:
		int x = 0, y = 0;
		Chunk* chunk[16][16];
		inline void Save();

		WorldUnit(int _x, int _y):
			x(_x), y(_y) {}

		inline unsigned short* PosBiome(int x, int y) { return &(chunk[x / 16][y / 16]->biomeid[x % 16][y % 16]); }
		inline unsigned short* PosBlock(int x, int y, int z) { return &(chunk[x / 16][y / 16]->block[x % 16][y % 16][z]); }
	};

	class Biome
	{
	public:
		Biome() {}

		virtual void Divide(WorldUnit*) {};
		virtual void Generate(WorldUnit*) {};
	};


	class BiomeMenu : public Biome
	{
	public:
		BiomeMenu(Biome*& _currentBiome);

		template<typename T> void RegisterBiome(int p)
		{
			BiomeList.push_back(std::make_pair(p, []() {return new T(); }));
			std::cout << "Registering Biome (id = " << p << ")\n";
		}
	
		Generation::WorldUnit* DivideBiomes(WorldUnit*);
		Generation::Chunk* Enquiry(int x, int y);

		

	private:
		Biome*& currentBiome;
		std::vector <std::pair<int, std::function<Biome* (void)> > >  BiomeList;


	};

}

Generation::Chunk* ImportChunk(int, int);