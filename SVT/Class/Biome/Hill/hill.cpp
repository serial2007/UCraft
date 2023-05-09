#include "hill.h"

std::vector<GenHill::Hill> GenHill::WorldHill;

inline void GenHill::GenSonHill(GenHill::Hill& con)
{
	if ((con.x < WorldPara::plx - WorldPara::HillExtendLength) || (con.x >= WorldPara::plx + N + WorldPara::HillExtendLength))	return;
	if (con.y < WorldPara::ply - WorldPara::HillExtendLength || con.y >= WorldPara::ply + M + WorldPara::HillExtendLength)		return;

	GenHill::ValidateHill(con);



	if (con.deg <= 0) return;	

	GenHill::Hill tmp;
	for (int i = Randi(0, WorldPara::sonN, con.x + con.y); i >= 1; --i)
	{
		tmp.x = con.x + Randi(-WorldPara::moveR, WorldPara::moveR, con.x * i + seed);
		tmp.y = con.y + Randi(-WorldPara::moveR, WorldPara::moveR, con.x - con.y * i);


		int dx = tmp.x - con.x;
		int dy = tmp.y - con.y;

		double MaxPossibleh = con.h * std::pow(dx * dx + dy * dy, WorldPara::HillSteep / 2.0f);
		tmp.h = Randd(0, MaxPossibleh, con.x * con.y);
		tmp.deg = con.deg - 1;

		GenHill::GenSonHill(tmp);
	}

	return;
}

void GenHill::ValidateHill(GenHill::Hill& con)
{
	std::cout << con.x << ' ' << con.y << ' ' << con.h;

	int cx = con.x - WorldPara::plx;
	int cy = con.y - WorldPara::ply;

	if (cx < 0 || cx >= N) { std::cout << "No\n"; return; }
	if (cy < 0 || cy >= M) { std::cout << "No\n"; return; }

	GenHill::WorldHill.push_back(con);
	{ std::cout << "Yes\n"; return; }
}
