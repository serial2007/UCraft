#include "Generation/Random/Random.h"
#include "Generation/Entry.h"
#include "UCLient/Graph/RenderBlockProcess.h"
#include "UCLient/Graph/imgui/imgui.h"
#include "UCLient/Graph/ChunkDraw.h"
#include <thread>
#include <cstdio>

int main()
{
	Renderer::ActivateImgui = 1;

	std::thread Ren(RenderBlockProcess);
	seed = 100;
	
	GenMain::RegisterBiomeMain();


	auto p = Enquiry(0, 0);
	UGraph::DrawChunk(p);
	//p->Biome3Dout();

	RenderBlock::RegisterBlock(0, 0, 5);
	RenderBlock::RegisterBlock(1, 1, 5, 0b1);


	//system("pause");
	while (1)
	{

		RenderBlock::shouldUpdate = 1;
	}
	std::cout << "save?\n";
	system("pause");
	SaveAll();
	return 0;
}
