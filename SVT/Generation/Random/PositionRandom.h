#pragma once
#include "DispersedRatio.h"
#include "ContinuousRandom.h"
#include <vector>

namespace PositionRandom
{
	typedef std::pair<int, int> Pos2;

	// pΪ���������
	// qΪ�����м�������� ��֤��ͬһ�ط�������ֹ���ĵ�
	// ͬʱͨ�����ýϴ��chan���ϴ��q�����Լ�����������
	extern std::vector<Pos2> GenLocation(double chan, int minx, int maxx, int miny, int maxy, int p, int q, RandomMachine* rm);
	
};
