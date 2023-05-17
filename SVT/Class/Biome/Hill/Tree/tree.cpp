#include "tree.h"

void HILL::Tree::Divide(Generation::WorldUnit* unit)
{
	std::cout << "Divide\n";


}

void HILL::Tree::Generate(Generation::WorldUnit* unit)
{
	std::cout << "Generate\n";

	auto arr = PositionRandom::GenLocation(0.05, unit->x * 256, unit->x * 256 + 255, unit->y * 256, unit->y * 256 + 255, 0, 2, &(this->rm));
	for (auto& p : arr)
	{
		int lx = p.first  - unit->x * 256;
		int ly = p.second - unit->y * 256;

		/*if (unit->chunk[lx / 16][ly / 16]->biomeid[lx % 16][ly % 16] != 4) continue;*/
		if (*(unit->FindBiome(lx, ly)) != 4) continue;
		int h = this->rm.Randi(4, 7, this->rm.magic2(p.first, p.second));
		bool skp = 0;
		int i;
		for (i = 127; i > 0; --i)
		{
			auto bl = *(unit->PosBlock(p.first - unit->x * 256, p.second - unit->y * 256, i));
			if (bl != 0) {
				if (bl != 2)
				skp = 1;
				break;
			}
		}
		if (skp) continue;
		int ht = h;
		while (ht--)
		{
			*(unit->PosBlock(p.first - unit->x * 256, p.second - unit->y * 256, ++i)) = 50;
		}

		this->GenLeaves(unit, p.first - unit->x * 256, p.second - unit->y * 256, i + 1, h + 3);
	}
}


void HILL::Tree::GenLeaves(Generation::WorldUnit* unit, int x, int y, int z, float gen)
{
	gen -= this->rm.Randd(1.3f, 2.7f, rm.magic3(x, y, z));
	if (gen <= 0) return;
	auto bl = unit->PosBlock(x, y, z);
	if (bl == nullptr) return;
	if (*bl != 0) return;
	*bl = 51;

	{
		HILL::Tree::GenLeaves(unit, x - 1, y, z, gen);
		HILL::Tree::GenLeaves(unit, x + 1, y, z, gen);
		HILL::Tree::GenLeaves(unit, x, y - 1, z, gen);
		HILL::Tree::GenLeaves(unit, x, y + 1, z, gen);
		HILL::Tree::GenLeaves(unit, x, y, z - 1, gen - 0.5f);
		HILL::Tree::GenLeaves(unit, x, y, z + 1, gen - 0.5f);
		//HILL::Tree::GenLeaves(unit, x - 1, y - 1, z - 1, gen);
		//HILL::Tree::GenLeaves(unit, x + 1, y - 1, z - 1, gen);
		//HILL::Tree::GenLeaves(unit, x - 1, y + 1, z - 1, gen);
		//HILL::Tree::GenLeaves(unit, x + 1, y + 1, z - 1, gen);
		//HILL::Tree::GenLeaves(unit, x - 1, y - 1, z + 1, gen);
		//HILL::Tree::GenLeaves(unit, x + 1, y - 1, z + 1, gen);
		//HILL::Tree::GenLeaves(unit, x - 1, y + 1, z + 1, gen);
		//HILL::Tree::GenLeaves(unit, x + 1, y + 1, z + 1, gen);
	}
}
