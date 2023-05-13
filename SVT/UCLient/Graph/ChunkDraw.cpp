#include "ChunkDraw.h"

void UGraph::DrawChunk(Generation::Chunk* chunk, Generation::WorldUnit* unit)
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
			//if (chunk->)
			try
			{

				if (chunk->block[i][j][k] == 0)
					continue;
			}
			catch (...)
			{
				return;
			}
			unsigned nd = 0;
			if (i > 0 && chunk->block[i - 1][j][k] == 0)	nd += 0b100000;
			if (i < 15 && chunk->block[i + 1][j][k] == 0)	nd += 0b000100;
			if (j > 0 && chunk->block[i][j - 1][k] == 0)	nd += 0b010000;
			if (j < 15 && chunk->block[i][j + 1][k] == 0)	nd += 0b000010;
			if (k > 0 && chunk->block[i][j][k - 1] == 0)	nd += 0b001000;
			if (k < 127 && chunk->block[i][j][k + 1] == 0)	nd += 0b000001;

			if (i == 0)		nd += 0b100000;
			if (i == 15)	nd += 0b000100;
			if (j == 0)		nd += 0b010000;
			if (j == 15)	nd += 0b000010;

			if (nd)
			{
				if(chunk->block[i][j][k] == 51)
					RenderBlock::RegisterBlock(i + chunk->x * 16, j + chunk->y * 16, k, nd, chunk->block[i][j][k], 1, unit);
				else RenderBlock::RegisterBlock(i + chunk->x * 16, j + chunk->y * 16, k, nd, chunk->block[i][j][k], 0, unit);
				//std::cout << "Rendered block (" << i + chunk->x * 16 << ", " << j + chunk->y * 16 << ", " << k << ") with id = " << chunk->block[i][j][k] << '\n';
			}
		}
	}
}
