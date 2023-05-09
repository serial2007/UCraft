#pragma once
//#include "probability/DispersedRatio.h"
#include "DispersedRatio.h"
#include "ContinuousRandom.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <Windows.h>
#include <vector>
#include <random>


struct Pos
{
	int x, y, h = 0;
};

//生成地形的基本参数
namespace WorldPara
{
	extern double maxh;
	extern double minh;

	extern int plx;
	extern int ply;

	extern int minx, maxx;
	extern int miny, maxy;

	extern int moveR;
	extern double moveDeglim;
	extern int sonN;


	extern int peek;
	extern double HillSteep;

	extern double HillEffect;
	extern int HillExtendLength;
	extern double HirzonWeight;

}

const int N = 300, M = 300;



extern double hi[N][N];