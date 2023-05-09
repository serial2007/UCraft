#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <vector>
#include <map>
#include <string>
#include <Windows.h>
#include <stack>
#include "Random/Random.h"
//inline Generation::Chunk* ImportChunk(int x, int y);

#define ReserveWorldUnit 64

#define REGISTER_BASE 0xfff
#define REGISTER_NULL 0x1000





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

			std::cout << "Registering Biome (id = " << p << ", fa = " << fa << ", ID = " << BiomeList.size() - 1 << ", faID = " << Mapid[fa] << ")\n";

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

Generation::Chunk* ImportChunk(int, int);