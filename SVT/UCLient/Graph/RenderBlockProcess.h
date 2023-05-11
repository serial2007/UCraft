#pragma once
#include "BasicClass/lib/Renderer.h"

void RenderBlockProcess();

namespace RenderBlock
{
	struct UBasic {
		int x, y, z;
		float tx, ty;
	};

	extern bool shouldUpdate;
	extern unsigned RendererN;

	extern RenderBlock::UBasic wh[8 * 256];

	extern unsigned indices[65537];

	extern unsigned offset;
	void RegisterBlock(int x, int y, int z, unsigned short sur = 0b111111);

	extern void ClearAll();
}