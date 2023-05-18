#include "hill.h"
#include "../../../Generation/WorldPara.h"

inline void HILL::Hill::GenSonHill(Generation::WorldUnit* h, HILL::Single& con, int x, int y)
{
	if ((con.x < x - WorldPara::HillExtendLength) || (con.x >= x + 256 + WorldPara::HillExtendLength))	return;
	if ((con.y < y - WorldPara::HillExtendLength) || (con.y >= y + 256 + WorldPara::HillExtendLength))	return;

	this->ValidateHill(h, con, x, y);

	if (con.deg <= 0) return;	

	HILL::Single tmp;
	for (int i = rm.Randi(0, WorldPara::sonN, con.x + con.y); i >= 1; --i)
	{


		tmp.x = con.x + rm.Randi(-WorldPara::moveR, WorldPara::moveR, con.x * i + seed);
		tmp.y = con.y + rm.Randi(-WorldPara::moveR, WorldPara::moveR, con.x - con.y * i);


		int dx = tmp.x - con.x;
		int dy = tmp.y - con.y;

		double MaxPossibleh = con.h * std::pow(dx * dx + dy * dy, WorldPara::HillSteep / 2.0f);
		tmp.h = rm.Randd(0, MaxPossibleh, con.x * con.y);
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


	this->WorldHill.push_back(con);
	{ std::cout << "Yes\n"; return; }
}

inline void HILL::Hill::Genmain(Generation::WorldUnit* h, int x, int y)
{
	int peekn = WorldPara::peek;
	HILL::Single peek;
	auto mt = PositionRandom::GenLocation(0.0003f, x - WorldPara::HillExtendLength, x + 255 + WorldPara::HillExtendLength, y - WorldPara::HillExtendLength, y + 255 + WorldPara::HillExtendLength, 0, 1, &DefaultRandomMachine);
	std::cout << mt.size();
	//system("pause");
	for (int i = 0; i < mt.size(); ++i)
	{
		peek.x = mt[i].first;
		peek.y = mt[i].second;

		bool CanGen = 1;
		for (int _i = -20; _i <= 20; ++_i)
		{
			for (int _j = -20; _j <= 20; ++_j)
			{
				auto P = h->FindBiome(_i + peek.x - x, _j + peek.y - y);
				if (P == nullptr)
				{
					std::cout << "Invalid " << _i + peek.x - x << ' ' << _j + peek.y - y << '\n';
				}
				if (P != nullptr && *P != 4U)
				{
					CanGen = 0;
					break;
				}
			}
			if (!CanGen) break;
		}
		if (!CanGen) continue;


		peek.h = DefaultRandomMachine.magic2(peek.x, peek.y) % 16 + 8;

		if(*(h->FindBiome(peek.x - x, peek.y - y)) == 4)
		{
			peek.deg = (int)(std::log(peek.h) * WorldPara::moveDeglim / std::log(WorldPara::maxh));
			this->GenSonHill(h, peek, x, y);
		}
	}


	this->Print(h, x, y);
}

void HILL::Hill::Generate(Generation::WorldUnit* h)
{
	this->Genmain(h, h->x * 256, h->y * 256);
	std::cout << "HILLGEN\n";
}

inline void HILL::Hill::Print(Generation::WorldUnit* h, int x, int y)
{
	for (int i = 0; i < 256; ++i)
		for (int j = 0; j < 256; ++j)
			hi[i][j] = WorldPara::minh;

	this->PrintHill(h, x, y);

	for (int k = 0; k < this->WorldHill.size(); ++k)
	{
		std::cout << "HILL" << this->WorldHill[k].h << ' ';
		std::cout << this->WorldHill[k].x - x << ' ' << this->WorldHill[k].y - y << std::endl;
	}
}

double HILL::Hill::SmoothHill(double dis2, int p = 0)
{
	if (abs(dis2) < 1.5) return 1;
	//dis2 = sqrt(dis2);
	dis2 /= rm.Randd(60, 200, p);
	return tanhf(dis2) / dis2;
}

inline void HILL::Hill::PrintHill(Generation::WorldUnit* h, int x, int y)
{
	std::cout << "PrintHill\n";
	if (h == nullptr)
	{
		std::cout << "Empty World Unit";
		system("pause");
	}


	for (int k = 0; k < this->WorldHill.size(); ++k)
	{
		this->LocTmp[k].x = this->WorldHill[k].x - x;
		this->LocTmp[k].y = this->WorldHill[k].y - y;
		if (isinf(this->WorldHill[k].h)) this->WorldHill[k].h = 0;

		this->LocTmp[k].h = 100000 * this->WorldHill[k].x * this->WorldHill[k].y;
	}

	for (int i = 0; i < 256; ++i)
		for (int j = 0; j < 256; ++j)
		{
			for (int k = 0; k < this->WorldHill.size(); ++k)
			{
				int dx = this->LocTmp[k].x - i;
				int dy = this->LocTmp[k].y - j;

				int r = dx * dx + dy * dy;
				if(r < 5000)
				hi[i][j] += this->SmoothHill(r, this->WorldHill[k].h) * this->WorldHill[k].h;
			}
			if ((isnan(hi[i][j]) || isinf(hi[i][j])))
			{
				if (i == 0 && j == 0)
					hi[i][j] = WorldPara::minh;
				else 	hi[i][j] = *(&hi[i][j] - 1);
			}
			if (hi[i][j] > WorldPara::minh) *(h->FindBiome(i, j)) = 4;

			int mx = std::min((int)hi[i][j], 127);
			for (int k = 0; k <= mx; ++k)
			{
			
				auto it = h->PosBlock(i, j, k);
				if (it == nullptr)
				{
					std::cout << "nullptr block (" << i << ", " << j << ", " << k << ")\n";
					Sleep(100);
				}
				else
				{
					if (k == mx) *it = 2U;
					else *it = 3U;
				}
			}
		}
}