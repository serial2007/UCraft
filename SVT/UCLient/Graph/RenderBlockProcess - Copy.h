#pragma once
#include "./Graph/BasicClass/lib/Renderer.h"

void RenderBlockProcess();

namespace RenderBlock
{
	extern bool shouldUpdate;
	extern unsigned RendererN;

	extern float wh[8 * 32];

	extern unsigned indices[65537];

	extern unsigned offset;
	void RegisterBlock(int x, int y, int z);
}