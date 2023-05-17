#include "SmoothLight.h"
#include "../ImportInfo.h"
#define SHADOW_THICKNESS 0.25f

ULight::SurfLight ULight::GenLight(int x, int y, int z, Generation::WorldUnit* unit, unsigned short w)
{
	if (unit == nullptr)
	{
		return SurfLight();
	}
	x -= unit->x * 256;
	y -= unit->y * 256;

	SurfLight lit;
	
	switch (w)
	{
	case 0b100000:
		if (x > 0)
		{
			if(z > 0)
			if (*unit->PosBlock(x - 1, y, z - 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x - 1, y, z - 1)] == 0)
			{
				lit.l[0][0] -= SHADOW_THICKNESS;
				lit.l[0][1] -= SHADOW_THICKNESS;
			}
			if(z < 127)
			if (*unit->PosBlock(x - 1, y, z + 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x - 1, y, z + 1)] == 0)
			{
				lit.l[1][0] -= SHADOW_THICKNESS;
				lit.l[1][1] -= SHADOW_THICKNESS;
			}
			if(y < 255)
			if (*unit->PosBlock(x - 1, y + 1, z) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x - 1, y + 1, z)] == 0)
			{
				lit.l[0][1] -= SHADOW_THICKNESS;
				lit.l[1][1] -= SHADOW_THICKNESS;
			}
			if (y > 0)
			if (*unit->PosBlock(x - 1, y - 1, z) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x - 1, y - 1, z)] == 0)
			{
				lit.l[0][0] -= SHADOW_THICKNESS;
				lit.l[1][0] -= SHADOW_THICKNESS;
			}
		}
		else return SurfLight();
		break;
	case 0b010000:
		if (y > 0)
		{
			if (z > 0)
				if (*unit->PosBlock(x, y - 1, z - 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x, y - 1, z - 1)] == 0)
				{
					lit.l[0][0] -= SHADOW_THICKNESS;
					lit.l[0][1] -= SHADOW_THICKNESS;
				}
			if (z < 127)
				if (*unit->PosBlock(x, y - 1, z + 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x, y - 1, z + 1)] == 0)
				{
					lit.l[1][0] -= SHADOW_THICKNESS;
					lit.l[1][1] -= SHADOW_THICKNESS;
				}
			if (x < 255)
				if (*unit->PosBlock(x + 1, y - 1, z) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x + 1, y - 1, z)] == 0)
				{
					lit.l[0][1] -= SHADOW_THICKNESS;
					lit.l[1][1] -= SHADOW_THICKNESS;
				}
			if (x > 0)
				if (*unit->PosBlock(x - 1, y - 1, z) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x - 1, y - 1, z)] == 0)
				{
					lit.l[0][0] -= SHADOW_THICKNESS;
					lit.l[1][0] -= SHADOW_THICKNESS;
				}
		}
		else return SurfLight();
		break;
	case 0b001000:
		if (z > 0)
		{
			if (x > 0)
				if (*unit->PosBlock(x - 1, y, z - 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x - 1, y, z - 1)] == 0)
				{
					lit.l[0][0] -= SHADOW_THICKNESS;
					lit.l[0][1] -= SHADOW_THICKNESS;
				}
			if (x < 255)
				if (*unit->PosBlock(x + 1, y, z - 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x + 1, y, z - 1)] == 0)
				{
					lit.l[1][0] -= SHADOW_THICKNESS;
					lit.l[1][1] -= SHADOW_THICKNESS;
				}
			if (y < 255)
				if (*unit->PosBlock(x, y + 1, z - 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x, y + 1, z - 1)] == 0)
				{
					lit.l[0][1] -= SHADOW_THICKNESS;
					lit.l[1][1] -= SHADOW_THICKNESS;
				}
			if (y > 0)
				if (*unit->PosBlock(x, y - 1, z - 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x, y - 1, z - 1)] == 0)
				{
					lit.l[0][0] -= SHADOW_THICKNESS;
					lit.l[1][0] -= SHADOW_THICKNESS;
				}
		}
		else return SurfLight();
		break;
	case 0b000100:
		if (x < 255)
		{
			if (z > 0)
				if (*unit->PosBlock(x + 1, y, z - 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x + 1, y, z - 1)] == 0)
				{
					lit.l[0][0] -= SHADOW_THICKNESS;
					lit.l[0][1] -= SHADOW_THICKNESS;
				}
			if (z < 127)
				if (*unit->PosBlock(x + 1, y, z + 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x + 1, y, z + 1)] == 0)
				{
					lit.l[1][0] -= SHADOW_THICKNESS;
					lit.l[1][1] -= SHADOW_THICKNESS;
				}
			if (y < 127)
				if (*unit->PosBlock(x + 1, y + 1, z) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x + 1, y + 1, z)] == 0)
				{
					lit.l[0][1] -= SHADOW_THICKNESS;
					lit.l[1][1] -= SHADOW_THICKNESS;
				}
			if (y > 0)
				if (*unit->PosBlock(x + 1, y - 1, z) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x + 1, y - 1, z)] == 0)
				{
					lit.l[0][0] -= SHADOW_THICKNESS;
					lit.l[1][0] -= SHADOW_THICKNESS;
				}
		}
		else return SurfLight();
		break;
	case 0b000010:
		if (y < 255)
		{
			if (z > 0)
				if (*unit->PosBlock(x, y + 1, z - 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x, y + 1, z - 1)] == 0)
				{
					lit.l[0][0] -= SHADOW_THICKNESS;
					lit.l[0][1] -= SHADOW_THICKNESS;
				}
			if (z < 127)
				if (*unit->PosBlock(x, y + 1, z + 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x, y + 1, z + 1)] == 0)
				{
					lit.l[1][0] -= SHADOW_THICKNESS;
					lit.l[1][1] -= SHADOW_THICKNESS;
				}
			if (x < 255)
				if (*unit->PosBlock(x + 1, y + 1, z) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x + 1, y + 1, z)] == 0)
				{
					lit.l[0][1] -= SHADOW_THICKNESS;
					lit.l[1][1] -= SHADOW_THICKNESS;
				}
			if (x > 0)
				if (*unit->PosBlock(x - 1, y + 1, z) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x - 1, y + 1, z)] == 0)
				{
					lit.l[0][0] -= SHADOW_THICKNESS;
					lit.l[1][0] -= SHADOW_THICKNESS;
				}
		}
		else return SurfLight();
		break;
	case 0b000001:
		if (z < 127)
		{
			if (x > 0)
				if (*unit->PosBlock(x - 1, y, z + 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x - 1, y, z + 1)] == 0)
				{
					lit.l[0][0] -= SHADOW_THICKNESS;
					lit.l[0][1] -= SHADOW_THICKNESS;
				}
			if (x < 255)
				if (*unit->PosBlock(x + 1, y, z + 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x + 1, y, z + 1)] == 0)
				{
					lit.l[1][0] -= SHADOW_THICKNESS;
					lit.l[1][1] -= SHADOW_THICKNESS;
				}
			if (y < 255)
				if (*unit->PosBlock(x, y + 1, z + 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x, y + 1, z + 1)] == 0)
				{
					lit.l[0][1] -= SHADOW_THICKNESS;
					lit.l[1][1] -= SHADOW_THICKNESS;
				}
			if (y > 0)
				if (*unit->PosBlock(x, y - 1, z + 1) != 0 && ImportInfo::IsTransmit[*unit->PosBlock(x, y - 1, z + 1)] == 0)
				{
					lit.l[0][0] -= SHADOW_THICKNESS;
					lit.l[1][0] -= SHADOW_THICKNESS;
				}
		}
		else return SurfLight();
		break;
	}
	return lit;
}
