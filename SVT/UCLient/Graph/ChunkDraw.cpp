#include "ChunkDraw.h"
#include "../ImportInfo.h"
int DynamicBackup::tx = 0;
int DynamicBackup::ty = 0;

void UGraph::DrawChunk(Generation::Chunk* chunk, Generation::WorldUnit* unit, bool priority)
{
	if (chunk == nullptr)
	{
		std::cout << "Error : invalid chunk!\n";
		return;
	}
	
	if (!priority)
	{
		if (std::abs(DynamicBackup::tx - chunk->x) <= 1 && std::abs(DynamicBackup::ty - chunk->y) <= 1)
		{
			priority = 1;
		}
	}
	
	int I =  RenderBlock::cameraPos.x - chunk->x * 16;
	int J = -RenderBlock::cameraPos.z - chunk->y * 16;
	int K =  RenderBlock::cameraPos.y;
	I = max(0, I); I = min(15, I);
	J = max(0, J); J = min(15, J);
	K = max(0, K); K = min(127, K);


	for (int i = 0; i < I; ++i)		for (int j = 0; j < J; ++j)		for (int k = 0; k < K; ++k)			UGraph::DrawBlock(chunk, unit, i, j, k, priority);
	for (int i = 0; i < I; ++i)		for (int j = 0; j < J; ++j)		for (int k = 127; k >= K; --k)		UGraph::DrawBlock(chunk, unit, i, j, k, priority);
	for (int i = 0; i < I; ++i)		for (int j = 15; j >= J; --j)	for (int k = 0; k < K; ++k)			UGraph::DrawBlock(chunk, unit, i, j, k, priority);
	for (int i = 0; i < I; ++i)		for (int j = 15; j >= J; --j)	for (int k = 127; k >= K; --k)		UGraph::DrawBlock(chunk, unit, i, j, k, priority);
	for (int i = 15; i >= I; --i)	for (int j = 0; j < J; ++j)		for (int k = 0; k < K; ++k)			UGraph::DrawBlock(chunk, unit, i, j, k, priority);
	for (int i = 15; i >= I; --i)	for (int j = 0; j < J; ++j)		for (int k = 127; k >= K; --k)		UGraph::DrawBlock(chunk, unit, i, j, k, priority);
	for (int i = 15; i >= I; --i)	for (int j = 15; j >= J; --j)	for (int k = 0; k < K; ++k)			UGraph::DrawBlock(chunk, unit, i, j, k, priority);
	for (int i = 15; i >= I; --i)	for (int j = 15; j >= J; --j)	for (int k = 127; k >= K; --k)		UGraph::DrawBlock(chunk, unit, i, j, k, priority);
}

inline void UGraph::DrawBlock(Generation::Chunk* chunk, Generation::WorldUnit* unit, int i, int j, int k, bool priority)
{
	if (chunk->block[i][j][k] == 0 && (chunk->blockstate[i][j][k] & 0xe0000000U) == 0)
		return;

	unsigned nd = 0;
	if (i > 0 &&	(chunk->block[i - 1][j][k] == 0	|| ImportInfo::IsNotSolid[chunk->block[i - 1][j][k]]) )		nd |= 0b100000U;
	if (i < 15 &&	(chunk->block[i + 1][j][k] == 0	|| ImportInfo::IsNotSolid[chunk->block[i + 1][j][k]]) )		nd |= 0b000100U;
	if (j > 0 &&	(chunk->block[i][j - 1][k] == 0	|| ImportInfo::IsNotSolid[chunk->block[i][j - 1][k]]) )		nd |= 0b010000U;
	if (j < 15 &&	(chunk->block[i][j + 1][k] == 0	|| ImportInfo::IsNotSolid[chunk->block[i][j + 1][k]]) )		nd |= 0b000010U;
	if (k > 0 &&	(chunk->block[i][j][k - 1] == 0	|| ImportInfo::IsNotSolid[chunk->block[i][j][k - 1]]) )		nd |= 0b001000U;
	if (k < 127 &&	(chunk->block[i][j][k + 1] == 0	|| ImportInfo::IsNotSolid[chunk->block[i][j][k + 1]]) )		nd |= 0b000001U;

	if (i == 0)		nd |= 0b100000;
	if (i == 15)	nd |= 0b000100;
	if (j == 0)		nd |= 0b010000;
	if (j == 15)	nd |= 0b000010;

	if (chunk->blockstate[i][j][k] & 0xe0000000U) {
		unsigned waternd = 0;
		if (i > 0 &&	chunk->block[i - 1][j][k] == 0 && (chunk->blockstate[i - 1][j][k] & 0xe0000000U) == 0)	waternd |= 0b100000U;
		if (i < 15 &&	chunk->block[i + 1][j][k] == 0 && (chunk->blockstate[i + 1][j][k] & 0xe0000000U) == 0)	waternd |= 0b000100U;
		if (j > 0 &&	chunk->block[i][j - 1][k] == 0 && (chunk->blockstate[i][j - 1][k] & 0xe0000000U) == 0)	waternd |= 0b010000U;
		if (j < 15 &&	chunk->block[i][j + 1][k] == 0 && (chunk->blockstate[i][j + 1][k] & 0xe0000000U) == 0)	waternd |= 0b000010U;
		if (k > 0 &&	chunk->block[i][j][k - 1] == 0 && (chunk->blockstate[i][j][k - 1] & 0xe0000000U) == 0)	waternd |= 0b001000U;
		if (k < 127 &&	chunk->block[i][j][k + 1] == 0 && (chunk->blockstate[i][j][k + 1] & 0xe0000000U) == 0 )	waternd |= 0b000001U;

		if (i == 0	&& GenMain::WorldNbt(i - 1 + chunk->x * 16, k, j + chunk->y * 16) != nullptr && (*GenMain::WorldNbt(i - 1 + chunk->x * 16, k, j + chunk->y * 16) & 0xe0000000U) == 0 && *GenMain::WorldBlock(i - 1 + chunk->x * 16, k, j + chunk->y * 16) == 0)	waternd |= 0b100000U;
		if (i == 15 && GenMain::WorldNbt(i + 1 + chunk->x * 16, k, j + chunk->y * 16) != nullptr && (*GenMain::WorldNbt(i + 1 + chunk->x * 16, k, j + chunk->y * 16) & 0xe0000000U) == 0 && *GenMain::WorldBlock(i + 1 + chunk->x * 16, k, j + chunk->y * 16) == 0)	waternd |= 0b000100U;
		if (j == 0 && GenMain::WorldNbt(i + chunk->x * 16, k, j + chunk->y * 16 - 1) != nullptr && (*GenMain::WorldNbt(i + chunk->x * 16, k, j + chunk->y * 16 - 1) & 0xe0000000U) == 0	 && *GenMain::WorldBlock(i + chunk->x * 16, k, j + chunk->y * 16 - 1) == 0)	waternd |= 0b010000U;
		if (j == 15 && GenMain::WorldNbt(i + chunk->x * 16, k, j + chunk->y * 16 + 1) != nullptr && (*GenMain::WorldNbt(i + chunk->x * 16, k, j + chunk->y * 16 + 1) & 0xe0000000U) == 0 && *GenMain::WorldBlock(i + chunk->x * 16, k, j + chunk->y * 16 + 1) == 0)	waternd |= 0b000010U;


		if(waternd & 0b000001U)		RenderBlock::RegisterBlock(i + chunk->x * 16, j + chunk->y * 16, k, waternd, 32,  1 + priority * 2, unit, 0U);
		else						RenderBlock::RegisterBlock(i + chunk->x * 16, j + chunk->y * 16, k, waternd, 555, 1 + priority * 2, unit, 0U);
		if (chunk->block[i][j][k] == 0)
			return;
	}

	if (nd)
	{
		if (chunk->block[i][j][k] == 51)
			 RenderBlock::RegisterBlock(i + chunk->x * 16, j + chunk->y * 16, k, nd, chunk->block[i][j][k], 1 + priority * 2, unit, chunk->blockstate[i][j][k]);
		else RenderBlock::RegisterBlock(i + chunk->x * 16, j + chunk->y * 16, k, nd, chunk->block[i][j][k],     priority * 2, unit, chunk->blockstate[i][j][k]);
		//std::cout << "Rendered block (" << i + chunk->x * 16 << ", " << j + chunk->y * 16 << ", " << k << ") with id = " << chunk->block[i][j][k] << '\n';
	}
}
