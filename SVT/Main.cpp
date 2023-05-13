#include "Generation/Random/Random.h"
#include "Generation/Entry.h"
#include "UCLient/Graph/RenderBlockProcess.h"
#include "UCLient/Graph/imgui/imgui.h"
#include "UCLient/Graph/ChunkDraw.h"
#include <thread>
#include <cstdio>
#include <json/json.h>
#include "UCLient/ImportInfo.h"
#include "UCLient/Graph/DynamicRegister.h"
#pragma omp parallel for


int main()
{
	ImportInfo::StartImport();
	Renderer::ActivateImgui = 1;

	std::thread Ren(RenderBlockProcess);
	seed = 1000;
	
	GenMain::RegisterBiomeMain();

	std::thread Dyn(DynamicRegister);


	//auto p1 = Enquiry(0, 0);
	//auto p2 = Enquiry(1, 0);
	//auto p3 = Enquiry(-1, 0);
	//auto p4 = Enquiry(-2, 0);
	////auto p5 = Enquiry(-1, -1);
	////auto p6 = Enquiry(-1, -1);
	//UGraph::DrawChunk(p1);
	//UGraph::DrawChunk(p2);
	//UGraph::DrawChunk(p3);
	//UGraph::DrawChunk(p4);
	////UGraph::DrawChunk(p5);
	////UGraph::DrawChunk(p6);
	////p->Biome3Dout();



	////system("pause");
	//while (1)
	//{

	//	RenderBlock::shouldUpdate = 1;
	//	while(RenderBlock::shouldUpdate) {}

	//}
	/*std::cout << "Save?\n";
	system("pause");
	SaveAll();*/
	while(!RenderBlock::ProgramEnd)
	{
		Sleep(200);
	}
	return 0;
}

//int main()
//{
//	system("pause");
//	//HILL::Hill p;
//	for (int i = 0; i < 10; ++i)
//	{
//		std::cout << thHill(i, 1) << std::endl;
//	}
//	system("pause");
//}