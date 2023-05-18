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
#include "UCLient/UserAction.h"
#include "UCLient/Graph/GenBasicLight.h"
#include "UCLient/BlockUpdate.h"
#pragma omp parallel for


int main()
{
	seed = 1;
	ImportInfo::StartImport();
	Renderer::ActivateImgui = 1;

	std::thread Ren(RenderBlockProcess);
	
	
	GenMain::RegisterBiomeMain();

	std::thread Dyn(DynamicRegister);
	while (RenderBlock::window == nullptr) {}
	std::thread User(UserAction::PLayerMain);
	std::thread Tick(UserAction::update);
	Sleep(1000);
	while(!RenderBlock::ProgramEnd)
	{
		/*for (auto& p : GenMain::WorldUnitTmp)
		{
			for(int i = 0; i < 16; ++i)
			for(int j = 0; j < 16; ++j)
			{
				ULight::GenBsLight(p.second->chunk[i][j]);
			}
		}*/
		Sleep(1000);
	}
	return 0;
}
