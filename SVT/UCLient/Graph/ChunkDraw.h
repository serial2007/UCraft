#pragma once
#include "RenderBlockProcess.h"
#include "../../Generation/Entry.h"



namespace UGraph
{
	extern void DrawChunk(Generation::Chunk* chunk, Generation::WorldUnit* unit, bool priority = 0);
	extern void DrawBlock(Generation::Chunk* chunk, Generation::WorldUnit* unit, int i, int j, int k, bool priority);
}

namespace DynamicBackup {
	extern int tx, ty;
}