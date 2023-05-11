#include "ChunkDraw.h"

void UGraph::DrawChunk(Generation::Chunk* chunk)
{
	if (chunk == nullptr)
	{
		std::cout << "Error : invalid chunk!\n";
		return;
	}
	for(int i = 0; i < 16; ++i)
	for(int j = 0; j < 16; ++j)
	{
		for (int k = 0; k < 128; ++k)
		{
			if (chunk->block[i][j][k] == 0)
				continue;
			bool nd = 0;
			if (i > 0 && chunk->block[i - 1][j][k] == 0) nd = 1;
			else if (i < 15 && chunk->block[i + 1][j][k] == 0) nd = 1;
			else if (j > 0 && chunk->block[i][j - 1][k] == 0) nd = 1;
			else if (j < 15 && chunk->block[i][j + 1][k] == 0) nd = 1;
			else if (k > 0 && chunk->block[i][j][k - 1] == 0) nd = 1;
			else if (k < 127 && chunk->block[i][j][k + 1] == 0) nd = 1;

			if (nd)
			{
				RenderBlock::RegisterBlock(i + chunk->x * 16, j + chunk->y * 16, k);
				std::cout << "Rendered block (" << i + chunk->x * 16 << ", " << j + chunk->y * 16 << ", " << k << ") with id = " << chunk->block[i][j][k] << '\n';
			}
		}
	}
}
