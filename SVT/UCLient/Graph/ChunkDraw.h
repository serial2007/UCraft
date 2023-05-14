#pragma once
#include "RenderBlockProcess.h"
#include "../../Generation/Entry.h"



namespace UGraph
{
	extern void DrawChunk(Generation::Chunk* chunk, Generation::WorldUnit* unit, bool priority = 0);
}

namespace DynamicBackup {
	extern int tx, ty;
}