#include "hill.h"
#include "../../../Generation/WorldPara.h"

inline void HILL::Hill::GenSonHill(Generation::WorldUnit* h, HILL::Single& con, int x, int y)
{
	if ((con.x < x - WorldPara::HillExtendLength) || (con.x >= x + 128 + WorldPara::HillExtendLength))	return;
	if ((con.y < y - WorldPara::HillExtendLength) || (con.y >= y + 128 + WorldPara::HillExtendLength))	return;

	this->ValidateHill(h, con, x, y);

	if (con.deg <= 0) return;	

	HILL::Single tmp;
	for (int i = Randi(0, WorldPara::sonN, con.x + con.y); i >= 1; --i)
	{
		tmp.x = con.x + Randi(-WorldPara::moveR, WorldPara::moveR, con.x * i + seed);
		tmp.y = con.y + Randi(-WorldPara::moveR, WorldPara::moveR, con.x - con.y * i);


		int dx = tmp.x - con.x;
		int dy = tmp.y - con.y;

		double MaxPossibleh = con.h * std::pow(dx * dx + dy * dy, WorldPara::HillSteep / 2.0f);
		tmp.h = Randd(0, MaxPossibleh, con.x * con.y);
		tmp.deg = con.deg - 1;

		this->GenSonHill(h, tmp, x, y);
	}

	return;
}

void HILL::Hill::ValidateHill(Generation::WorldUnit* h, HILL::Single& con, int x, int y)
{
	std::cout << con.x << ' ' << con.y << ' ' << con.h;

	int cx = con.x - x;
	int cy = con.y - y;

	if (cx < 0 || cx >= 128) { std::cout << "No\n"; return; }
	if (cy < 0 || cy >= 128) { std::cout << "No\n"; return; }

	this->WorldHill.push_back(con);
	{ std::cout << "Yes\n"; return; }
}

inline void HILL::Hill::Genmain(Generation::WorldUnit* h, int x, int y)
{
	int peekn = WorldPara::peek;
	HILL::Single peek;
	while (peekn--)
	{
		peek.x = Randi(x, x + 127, (long long)(std::sin(peekn) * INT_MAX) % 10000000);
		peek.y = Randi(x, y + 127, (long long)(std::log(peekn) * INT_MAX) % 10000000);
		peek.h = Randd(0, WorldPara::maxh, (long long)(std::cos(peekn) * INT_MAX) % 10000000);

		peek.deg = (int)(std::log(peek.h) * WorldPara::moveDeglim / std::log(WorldPara::maxh));

		this->GenSonHill(h, peek, x, y);

	}
}

void HILL::Hill::Divide(Generation::WorldUnit*)
{
}

void HILL::Hill::Generate(Generation::WorldUnit* h)
{
	this->Genmain(h, h->x * 16, h->y * 16);
	std::cout << "HILLGEN\n";
}

inline void HILL::Hill::Print(int x, int y)
{
	for (int i = 0; i < 256; ++i)
		for (int j = 0; j < 256; ++j)
			hi[i][j] = WorldPara::minh;

	this->PrintHill(x, y);

	for (int k = 0; k < this->WorldHill.size(); ++k)
	{
		std::cout << this->WorldHill[k].x - x << ' ' << this->WorldHill[k].y - y << std::endl;
	}
}

double HILL::Hill::SmoothHill(double dis2, int p = 0)
{
	if (abs(dis2) < 1.5) return 1;
	dis2 = sqrt(dis2);
	dis2 /= Randd(7, 35, p);
	double tmp1 = 1.0 - std::tanh(dis2 - 0.35);
	double tmp2 = log10(dis2 + 0.65);
	double tmp3 = dis2 - 0.35;
	return tmp1 / tmp2 * tmp3 / 2.5;
}

inline void HILL::Hill::PrintHill(int x, int y)
{
	for (int k = 0; k < this->WorldHill.size(); ++k)
	{
		this->LocTmp[k].x = this->WorldHill[k].x - x;
		this->LocTmp[k].y = this->WorldHill[k].y - y;
		if (isinf(this->WorldHill[k].h)) this->WorldHill[k].h = 0;
		std::cout << "H" << this->WorldHill[k].h << '\n';

		this->LocTmp[k].h = 100000 * this->WorldHill[k].x * this->WorldHill[k].y;
	}


	for (int i = 0; i < 256; ++i)
		for (int j = 0; j < 256; ++j)
		{
			for (int k = 0; k < this->WorldHill.size(); ++k)
			{
				int dx = this->LocTmp[k].x - i;
				int dy = this->LocTmp[k].y - j;


				hi[i][j] += this->SmoothHill(dx * dx + dy * dy, this->LocTmp[k].h) * this->WorldHill[k].h;
			}
			if (isnan(hi[i][j]) || isinf(hi[i][j]))
				hi[i][j] = *(&hi[i][j] - 1);
		}
}